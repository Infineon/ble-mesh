/******************************************************************************
* File Name:   mesh_application.c
*
* Description: This file contains mesh application handles and callback
*
* Related Document: See README.md
*
*******************************************************************************
* $ Copyright 2021-YEAR Cypress Semiconductor $
*******************************************************************************/
#include "cy_retarget_io.h"
#include "cybsp_bt_config.h"
#include "wiced_bt_types.h"
#include "wiced_bt_stack.h"
#include "wiced_bt_mesh_core.h"
#include "wiced_bt_mesh_models.h"
#include "wiced_bt_mesh_provision.h"
#include "wiced_bt_mesh_app.h"
#include "wiced_memory.h"
#include "bt_app.h"
#include "bt_utils.h"
#include "mesh_app.h"
#include "mesh_application.h"
#include "mesh_platform_utils.h"
#include "p_256_ecc_pp.h"

static void	mesh_adv_report(wiced_bt_ble_scan_results_t *p_scan_result, uint8_t *p_adv_data);

/******************************************************
 *          Structures
 ******************************************************/

wiced_bt_cfg_settings_t* p_wiced_bt_mesh_cfg_settings;
wiced_bt_mesh_core_received_msg_handler_t p_app_model_message_handler = NULL;
wiced_bt_mesh_core_received_msg_handler_t p_proxy_status_message_handler = NULL;

wiced_bool_t mesh_node_authenticated = WICED_FALSE;
wiced_bool_t pb_gatt_in_progress = WICED_FALSE;

// NVRAM IDs used by mesh models library
uint16_t wiced_bt_mesh_scene_register_nvram_id;
uint16_t wiced_bt_mesh_scene_nvram_id_start;
uint16_t wiced_bt_mesh_scene_nvram_id_end;
uint16_t wiced_bt_mesh_scheduler_nvram_id_start;
uint16_t wiced_bt_mesh_default_trans_time_nvram_id_start;
uint16_t wiced_bt_mesh_power_level_nvram_id_start;
uint16_t wiced_bt_mesh_power_onoff_nvram_id_start;
uint16_t wiced_bt_mesh_light_lightness_nvram_id_start;
uint16_t wiced_bt_mesh_light_xyl_nvram_id_start;
uint16_t wiced_bt_mesh_light_ctl_nvram_id_start;
uint16_t wiced_bt_mesh_light_hsl_nvram_id_start;
uint16_t wiced_bt_mesh_light_lc_run_nvram_id_start;
uint16_t wiced_bt_mesh_light_lc_nvram_id_start;

uint8_t  wiced_bt_mesh_scene_max_num;
uint8_t  wiced_bt_mesh_scheduler_events_max_num;

uint16_t mesh_nvm_idx_seq;

EC curve_p256;
extern EC* p_curve_p256;

wiced_timer_t app_timer;

#pragma pack(1)
#ifndef PACKED
#define PACKED
#endif

// represents data saved in the NVRAM with index mesh_nvm_idx_seq
typedef PACKED struct
{
    uint32_t    seq;        // self SEQ
    uint16_t    size;       // Size of the RPL - number of RPL items
} mesh_app_rpl_init_t;

#define MESH_APP_RPL_ITEM_PREV_IVI_FLAG 0x8000
// Represents one RPL entry in the NVRAM
typedef PACKED struct
{
    uint16_t    addr;       // Bit MESH_APP_RPL_ITEM_PREV_IVI_FLAG means previous IVI
    uint8_t     seq[3];     // latest SEQ for SRC addr
} mesh_app_rpl_item_t;

#pragma pack()

// current value of own SEQ and RPL size
static mesh_app_rpl_init_t mesh_app_rpl_init = { 0 };

/*
 * Application implements that function to handle received messages. Call each library that this device needs to support.
 */
wiced_bt_mesh_core_received_msg_handler_t mesh_message_handler_callback(uint16_t company_id, uint16_t opcode, uint16_t *p_model_id, uint8_t *p_rpl_delay)
{
    wiced_bt_mesh_core_received_msg_handler_t p_message_handler = NULL;
    uint8_t                                   idx_elem, idx_model;
    wiced_bt_mesh_event_t                     temp_event;
    uint16_t                                  model_id = 0xffff;

    printf("mesh_message_handler_callback: company_id:%x opcode:%x\r\n", company_id, opcode);

    // Each model present on the element contains the callback which will be executed.
    // If message handler returns that callback, that means that Opcode is for that model.
    // The special case is Proxy Status messages, which do not belong to any model.
    if ((company_id == MESH_COMPANY_ID_UNUSED) && (p_proxy_status_message_handler != NULL))
    {
        p_message_handler = p_proxy_status_message_handler;
    }
    else
    {
        temp_event.company_id = company_id;
        temp_event.opcode = opcode;
        temp_event.model_id = 0xffff;   // it is a sign of special mode for model to just return true if the opcode is for that model, without message handling.
        temp_event.status.rpl_delay = 5;  // By default save RPL after 5 seconds delay to reduce number of NVRAM writes if messages are coming too fast

        for (idx_elem = 0; idx_elem < mesh_config.elements_num; idx_elem++)
        {
            for (idx_model = 0; idx_model < mesh_config.elements[idx_elem].models_num; idx_model++)
            {
                if (((company_id != MESH_COMPANY_ID_BT_SIG) && (mesh_config.elements[idx_elem].models[idx_model].company_id == MESH_COMPANY_ID_BT_SIG)) ||
                    ((company_id == MESH_COMPANY_ID_BT_SIG) && (mesh_config.elements[idx_elem].models[idx_model].company_id != MESH_COMPANY_ID_BT_SIG)))
                    continue;
                p_message_handler = (wiced_bt_mesh_core_received_msg_handler_t)mesh_config.elements[idx_elem].models[idx_model].p_message_handler;
                if (p_message_handler == NULL)
                    continue;
                if (!p_message_handler(&temp_event, NULL, 0))
                    continue;
                model_id = mesh_config.elements[idx_elem].models[idx_model].model_id;
                if (p_model_id)
                    *p_model_id = model_id;
                if (p_rpl_delay)
                    *p_rpl_delay = temp_event.status.rpl_delay;
                break;
            }
            if (idx_model < mesh_config.elements[idx_elem].models_num)
                break;
        }
        if (idx_elem >= mesh_config.elements_num)
            p_message_handler = NULL;
    }
    // Application may overwrite models library and receive raw core data.  Do not allow
    // to overwrite certain profiles though.
    if ((p_app_model_message_handler != NULL) &&
        ((company_id != MESH_COMPANY_ID_BT_SIG) ||
            ((model_id != WICED_BT_MESH_CORE_MODEL_ID_CONFIG_CLNT) &&
                (model_id != WICED_BT_MESH_CORE_MODEL_ID_REMOTE_PROVISION_SRV) &&
                (model_id != WICED_BT_MESH_CORE_MODEL_ID_REMOTE_PROVISION_CLNT) &&
                (model_id != WICED_BT_MESH_CORE_MODEL_ID_GENERIC_DEFTT_SRV) &&
                (model_id != WICED_BT_MESH_CORE_MODEL_ID_GENERIC_DEFTT_CLNT))))
    {
        p_message_handler = p_app_model_message_handler;
    }
    if (p_message_handler == NULL)
    {
        printf("mesh_message_handler_callback: ignored\r\n");
    }
    return p_message_handler;
}


/*
 * Application implements that function to start/stop scanning as requested by the core
 */
void mesh_start_stop_scan_callback(wiced_bool_t start, wiced_bool_t is_scan_active)
{
    static wiced_bool_t observer_started = WICED_FALSE;
    static wiced_bool_t observer_active = WICED_FALSE;
    uint16_t local_addr = wiced_bt_mesh_core_get_local_addr();
    // printf("scan callback: start:%d active:%d local_addr:%x pb_gatt:%d\n", start, is_scan_active, local_addr, pb_gatt_in_progress);

    if ((observer_started == start) && (observer_active == is_scan_active))
        return;

    // check if the request is to stop the scan, or scan type is different
    if ((observer_started && !start) || (start && (observer_active != is_scan_active)))
    {
        printf("mesh_start_stop_scan_callback:observer stop\r\n");
        // request to stop scanning
        wiced_bt_ble_observe(0, 0, mesh_adv_report);

        observer_started = WICED_FALSE;
        if (!start)
            return;
    }

    wiced_bt_cfg_scan_settings.scan_mode = is_scan_active ? BTM_BLE_SCAN_MODE_ACTIVE : BTM_BLE_SCAN_MODE_PASSIVE;
    wiced_bt_cfg_scan_settings.low_duty_scan_window = 96;
    // If node is LPN in unprovisioned state before PB-ADV start then use low duty scan to save the battery. High dury scan will be restores on PB-ADV start.
    if ((mesh_config.features & WICED_BT_MESH_CORE_FEATURE_BIT_LOW_POWER) != 0 && local_addr == 0 && !pb_gatt_in_progress)
        wiced_bt_cfg_scan_settings.low_duty_scan_window = 48;
    observer_started = WICED_TRUE;
    observer_active = is_scan_active;
    printf("mesh_start_stop_scan_callback:observer start active:%d\n", observer_active);

    wiced_bt_ble_observe(start, 0, mesh_adv_report);
}


void mesh_state_changed_cb(wiced_bt_mesh_core_state_type_t type, wiced_bt_mesh_core_state_t *p_state)
{
    switch (type)
    {
    case WICED_BT_MESH_CORE_STATE_TYPE_CLR_RPL:
        // Clear RPL. On error do factory reset
        if(!mesh_application_rpl_clr())
            mesh_application_factory_reset();
        break;

    case WICED_BT_MESH_CORE_STATE_TYPE_SEQ:
        // save own SEQ or RPL entry. On error do factory reset
        if (!mesh_application_seq_save(&p_state->seq))
        {
            mesh_application_factory_reset();
            break;
        }

        // only provisioner client needs to send these events to MCU
        if (!mesh_config_client)
            break;

        break;

    case WICED_BT_MESH_CORE_STATE_NODE_STATE:
        printf("mesh_state_changed_cb: authenticated:%d provisioned:%d proxy_on:%d pb_adv:%d\n",
																mesh_node_authenticated,
																p_state->node_state.provisioned,
																p_state->node_state.proxy_on,
																p_state->node_state.pb_adv);
        // If node just got provisioned in the connected state then it is end of PB_GATT.
        // Make proxy advert interval 30 ms to make first connection more reliable - core will use this value to start advertising after disconnection.
        if (bt_app_gatt_is_connected() && !mesh_node_authenticated && p_state->node_state.provisioned)
            wiced_bt_mesh_core_proxy_adv_interval = 48;

        pb_gatt_in_progress = p_state->node_state.pb_adv;
        // If node is LPN in unprovisioned state(!mesh_node_authenticated) then restart scan on PB-ADV start or on provisioning end to switch scan from low duty to high duty
        if ((mesh_config.features & WICED_BT_MESH_CORE_FEATURE_BIT_LOW_POWER) != 0
            && !mesh_node_authenticated
            && ((!p_state->node_state.provisioned && p_state->node_state.pb_adv) || p_state->node_state.provisioned))
        {
            mesh_start_stop_scan_callback(WICED_FALSE, WICED_FALSE);
            mesh_start_stop_scan_callback(WICED_TRUE, WICED_FALSE);
        }
        // exit if provisioned state is not changed
        if ((mesh_node_authenticated && p_state->node_state.provisioned)
            || (!mesh_node_authenticated && !p_state->node_state.provisioned))
            break;

        mesh_node_authenticated = p_state->node_state.provisioned;

        bt_app_gatt_db_init(mesh_node_authenticated);

        if (wiced_bt_mesh_app_func_table.p_mesh_app_init != NULL)
        {
            wiced_bt_mesh_app_func_table.p_mesh_app_init(mesh_node_authenticated);
        }
        break;

    case WICED_BT_MESH_CORE_STATE_LPN_FRIENDSHIP:
        printf("mesh_state_changed_cb:LPN_FRIENDSHIP: established:%d addr:%x/%x\n",
                                                    p_state->lpn.established, p_state->lpn.addr,
                                                    wiced_bt_mesh_core_lpn_get_friend_addr());
        break;

    case WICED_BT_MESH_CORE_STATE_LPN_SCAN:
        // it can happen only on LPN with established friendship
        printf("mesh_state_changed_cb:LPN_SCAN: scan:%d\n", p_state->lpn_scan);
#ifndef PTS
        mesh_start_stop_scan_callback(p_state->lpn_scan, WICED_FALSE);
#endif
        break;

    case WICED_BT_MESH_CORE_STATE_LPN_SLEEP:
        printf("mesh_state_changed_cb:LPN_SLEEP: timeout:%d\n", (int)p_state->lpn_sleep);
        if (wiced_bt_mesh_app_func_table.p_mesh_app_lpn_sleep)
        {
            wiced_bt_mesh_app_func_table.p_mesh_app_lpn_sleep(p_state->lpn_sleep);
        }
        break;

    case WICED_BT_MESH_CORE_STATE_FRND_FRIENDSHIP:
        printf("mesh_state_changed_cb:FRND_FRIENDSHIP: established:%d addr:%x\n",
                                    p_state->frnd.established, p_state->frnd.addr);
        break;

    default:
        break;
    }
}

// Handle received adv packet
void mesh_adv_report(wiced_bt_ble_scan_results_t *p_scan_result, uint8_t *p_adv_data)
{

    if (p_scan_result == NULL)
        return;

#ifdef NEW_WICED_STACK
    /* New WICED stack puts the total ADV data length as the first byte - skip it for mesh. */
    p_adv_data++;
#endif

    if (p_scan_result->ble_evt_type == BTM_BLE_EVT_CONNECTABLE_ADVERTISEMENT)
    {
        // if we do not want to provision new devices over GATT, only pass nonconnectable adverts to RPR
#if defined REMOTE_PROVISION_SERVER_SUPPORTED && defined REMOTE_PROVISION_OVER_GATT_SUPPORTED
        if (wiced_bt_mesh_remote_provisioning_connectable_adv_packet(p_scan_result, p_adv_data))
            return;
#endif
#if defined GATT_PROXY_CLIENT_SUPPORTED
        wiced_bt_mesh_gatt_client_process_connectable_adv(p_scan_result->remote_bd_addr, p_scan_result->ble_addr_type, p_scan_result->rssi, p_adv_data);
#endif
    }
    else if (p_scan_result->ble_evt_type == BTM_BLE_EVT_SCAN_RSP)
    {
#if defined REMOTE_PROVISION_SERVER_SUPPORTED && defined REMOTE_PROVISION_OVER_GATT_SUPPORTED
        if (wiced_bt_mesh_remote_provisioning_scan_rsp(p_scan_result, p_adv_data))
            return;
#endif
    }
    else if (p_scan_result->ble_evt_type == BTM_BLE_EVT_NON_CONNECTABLE_ADVERTISEMENT)
    {
#if REMOTE_PROVISION_SERVER_SUPPORTED
        if (wiced_bt_mesh_remote_provisioning_nonconnectable_adv_packet(p_scan_result, p_adv_data))
            return;
#endif // if defined REMOTE_PROVISION_SERVER_SUPPORTED
#ifdef PRIVATE_PROXY_SUPPORTED
        if (wiced_bt_mesh_proxy_solicitation_nonconnectable_adv_packet(p_scan_result, p_adv_data))
            return;
#endif // if defined PRIVATE_PROXY_SUPPORTED
        wiced_bt_mesh_core_adv_packet(p_scan_result->rssi, p_adv_data, p_scan_result->remote_bd_addr);
    }
}

void mesh_application_gen_uuid(uint8_t *uuid)
{

    // Generate version 4 UUID (Random) per rfc4122:
    // - Set the two most significant bits(bits 6 and 7) of the
    //   clock_seq_hi_and_reserved to zero and one, respectively.
    // - Set the four most significant bits(bits 12 through 15) of the
    //   time_hi_and_version field to the 4 - bit version number.
    // - Set all the other bits to randomly(or pseudo - randomly) chosen values.
    *(uint32_t*)&uuid[0] =  mesh_util_rand_gen_num();
    *(uint32_t*)&uuid[4] =  mesh_util_rand_gen_num();
    *(uint32_t*)&uuid[8] =  mesh_util_rand_gen_num();
    *(uint32_t*)&uuid[12] = mesh_util_rand_gen_num();
    // The version field is 4.
    uuid[6] = (uuid[6] & 0x0f) | 0x40;
    // The variant field is 10B
    uuid[8] = (uuid[8] & 0x3f) | 0x80;
    //printf("mesh_application_gen_uuid: UUID:%d\r\n", mesh_node_authenticated);
}


wiced_bool_t mesh_application_seq_save(wiced_bt_mesh_core_state_seq_t *p_seq)
{
    wiced_result_t          result;

    // WICED_BT_TRACE("Save SRC:%x SEQ:%d prev_ivi:%d idx:%d rpl_size:%d\n", p_seq->addr, p_seq->seq, p_seq->previous_iv_idx, p_seq->rpl_entry_idx, mesh_app_rpl_init.size);

    // if it is own SEQ then save it into NVRAM
    if (p_seq->addr == 0)
    {
        // write only multiples of thousands values to prevent too often NVRAM writes. Write each change if node doesn't send messages often.
        mesh_app_rpl_init.seq = p_seq->seq;
        if ((mesh_app_rpl_init.seq % 1000) == 0)
        {
             if (sizeof(mesh_app_rpl_init) != mesh_nvram_access(WICED_TRUE, mesh_nvm_idx_seq, (uint8_t*)&mesh_app_rpl_init, sizeof(mesh_app_rpl_init), &result) || result != 0)
                return WICED_FALSE;
        }
    }
    else if (p_seq->addr == 0xffff)
    {
        // 0xffff means delete all RPL entries - all other fields are ignored in that case.
        mesh_app_rpl_init.size = 0;
        if (sizeof(mesh_app_rpl_init) != mesh_nvram_access(WICED_TRUE, mesh_nvm_idx_seq, (uint8_t*)&mesh_app_rpl_init, sizeof(mesh_app_rpl_init), &result) || result != 0)
            return WICED_FALSE;
    }
    else
    {
        // save RPL entry
        uint8_t                 *p;
        mesh_app_rpl_item_t     rpl_item;
        rpl_item.addr = p_seq->addr;
        if (p_seq->previous_iv_idx)
            rpl_item.addr |= MESH_APP_RPL_ITEM_PREV_IVI_FLAG;
        p = rpl_item.seq;
        UINT24_TO_STREAM(p, p_seq->seq);
        if (sizeof(rpl_item) != mesh_nvram_access(WICED_TRUE, mesh_nvm_idx_seq - 1 - p_seq->rpl_entry_idx, (uint8_t*)&rpl_item, sizeof(rpl_item), &result) || result != 0)
            return WICED_FALSE;
        // If new record is added
        if (p_seq->rpl_entry_idx >= mesh_app_rpl_init.size)
        {
            // delete probably existing records for unknown indices. It is possible because we save RPL entries on successfull message handling
            while (mesh_app_rpl_init.size < p_seq->rpl_entry_idx)
                mesh_nvram_access(WICED_TRUE, mesh_nvm_idx_seq - 1 - mesh_app_rpl_init.size++, NULL, 0, &result);
            // update RPL_INIT NVRAM data with new RPL size
            if (sizeof(mesh_app_rpl_init) != mesh_nvram_access(WICED_TRUE, mesh_nvm_idx_seq, (uint8_t*)&mesh_app_rpl_init, sizeof(mesh_app_rpl_init), &result) || result != 0)
                return WICED_FALSE;
        }
    }
    return WICED_TRUE;
}

wiced_bool_t mesh_application_seq_init(void)
{
    wiced_result_t      result;
    mesh_app_rpl_item_t rpl_item;
    uint8_t             *p;
    uint32_t            i, seq;

    printf("mesh_application_seq_init: mesh_node_authenticated:%d\r\n", mesh_node_authenticated);
    // if node is provisioned then read own SEQ and RPL size from NVRAM, update SEQ, save it in the NVRAM and set it to the node
    // otherwise just save 0-initialized value in NVRAM
    if (mesh_node_authenticated)
    {
        if (sizeof(mesh_app_rpl_init) != mesh_nvram_access(WICED_FALSE, mesh_nvm_idx_seq, (uint8_t*)&mesh_app_rpl_init, sizeof(mesh_app_rpl_init), &result) || result != 0)
            return WICED_FALSE;
        // add 1000 because we write only multiples of thousands values. It is not needed if node doesn't send messages often.
        mesh_app_rpl_init.seq += 1000;
    }
    if (sizeof(mesh_app_rpl_init) != mesh_nvram_access(WICED_TRUE, mesh_nvm_idx_seq, (uint8_t*)&mesh_app_rpl_init, sizeof(mesh_app_rpl_init), &result) || result != 0)
        return WICED_FALSE;
    if (!wiced_bt_mesh_core_set_seq(0, mesh_app_rpl_init.seq, WICED_FALSE))
        return WICED_FALSE;

    // read RPL items from NVRAM and set them to the node

    for (i = 0; i < mesh_app_rpl_init.size; i++)
    {
       if (sizeof(rpl_item) != mesh_nvram_access(WICED_FALSE, mesh_nvm_idx_seq - 1 - i, (uint8_t*)&rpl_item, sizeof(rpl_item), &result) || result != 0)
            continue;
        p = rpl_item.seq;
        STREAM_TO_UINT24(seq, p);
        if (!wiced_bt_mesh_core_set_seq(rpl_item.addr & (~MESH_APP_RPL_ITEM_PREV_IVI_FLAG), seq, (rpl_item.addr & MESH_APP_RPL_ITEM_PREV_IVI_FLAG) != 0 ? WICED_TRUE : WICED_FALSE))
            return WICED_FALSE;
    }

    return WICED_TRUE;
}

// saves own SEQ or RPL entry
wiced_bool_t mesh_application_rpl_clr(void)
{
    wiced_result_t          result;
    printf("mesh_application_rpl_clr: size:%d\r\n", mesh_app_rpl_init.size);
    mesh_app_rpl_init.size = 0;
    if (sizeof(mesh_app_rpl_init) != mesh_nvram_access(WICED_TRUE, mesh_nvm_idx_seq, (uint8_t*)&mesh_app_rpl_init, sizeof(mesh_app_rpl_init), &result) || result != 0)
        return WICED_FALSE;
    return WICED_TRUE;
}


/*
 * The function goes through all models of all elements and returns number of elements on which specified model is present
 */
uint8_t mesh_application_get_element_count(uint16_t model_id)
{
    uint8_t element_idx;
    uint8_t model_idx;
    uint8_t num_elements_with_model = 0;

    for (element_idx = 0; element_idx < mesh_config.elements_num; element_idx++)
    {
        for (model_idx = 0; model_idx < mesh_config.elements[element_idx].models_num; model_idx++)
        {
            if ((mesh_config.elements[element_idx].models[model_idx].company_id == MESH_COMPANY_ID_BT_SIG) &&
                (mesh_config.elements[element_idx].models[model_idx].model_id == model_id))
            {
                num_elements_with_model++;
                break;
            }
        }
    }
    return (num_elements_with_model);
}


void mesh_application_factory_reset(void)
{
    // uncomment following lines to delete UUID from the NV. This will change UUID on factory reset.
    // wiced_result_t result;
    // mesh_nvram_access(WICED_TRUE, NVRAM_ID_LOCAL_UUID, NULL, 0, &result);

    mesh_core_reset(WICED_FALSE, WICED_TRUE);

    printf("mesh_application_factory_reset: done\r\n");

    if (wiced_bt_mesh_app_func_table.p_mesh_app_factory_reset)
    {
        wiced_bt_mesh_app_func_table.p_mesh_app_factory_reset();
    }
}

void mesh_application_nvram_id_init(void)
{
    // Allocate one additional NVRAM ID for configuration data in case it will increased on FW upgrade
    uint16_t cfg_data_len = wiced_bt_mesh_get_node_config_size(&mesh_config) + WICED_BT_MESH_CORE_NVRAM_CHUNK_MAX_SIZE;

    wiced_bt_mesh_light_lc_nvram_id_start = FLASH_MEMORY_END - mesh_util_get_number_of_elements(WICED_BT_MESH_CORE_MODEL_ID_LIGHT_LC_SRV);
    wiced_bt_mesh_light_lc_run_nvram_id_start = wiced_bt_mesh_light_lc_nvram_id_start - mesh_util_get_number_of_elements(WICED_BT_MESH_CORE_MODEL_ID_LIGHT_LC_SRV);
    wiced_bt_mesh_light_hsl_nvram_id_start = wiced_bt_mesh_light_lc_run_nvram_id_start - mesh_util_get_number_of_elements(WICED_BT_MESH_CORE_MODEL_ID_LIGHT_HSL_SRV);
    wiced_bt_mesh_light_ctl_nvram_id_start = wiced_bt_mesh_light_hsl_nvram_id_start - mesh_util_get_number_of_elements(WICED_BT_MESH_CORE_MODEL_ID_LIGHT_CTL_SRV);
    wiced_bt_mesh_light_xyl_nvram_id_start = wiced_bt_mesh_light_ctl_nvram_id_start - mesh_util_get_number_of_elements(WICED_BT_MESH_CORE_MODEL_ID_LIGHT_XYL_SRV);
    wiced_bt_mesh_light_lightness_nvram_id_start = wiced_bt_mesh_light_xyl_nvram_id_start - mesh_util_get_number_of_elements(WICED_BT_MESH_CORE_MODEL_ID_LIGHT_LIGHTNESS_SRV);
    wiced_bt_mesh_power_level_nvram_id_start = wiced_bt_mesh_light_lightness_nvram_id_start - mesh_util_get_number_of_elements(WICED_BT_MESH_CORE_MODEL_ID_GENERIC_POWER_LEVEL_SRV);
    wiced_bt_mesh_power_onoff_nvram_id_start = wiced_bt_mesh_power_level_nvram_id_start - mesh_util_get_number_of_elements(WICED_BT_MESH_CORE_MODEL_ID_GENERIC_POWER_ONOFF_SRV);
    wiced_bt_mesh_default_trans_time_nvram_id_start = wiced_bt_mesh_power_onoff_nvram_id_start - 1;
    wiced_bt_mesh_scheduler_nvram_id_start = wiced_bt_mesh_default_trans_time_nvram_id_start - wiced_bt_mesh_scheduler_events_max_num;
    wiced_bt_mesh_scene_nvram_id_end = wiced_bt_mesh_scheduler_nvram_id_start - 1;
    wiced_bt_mesh_scene_nvram_id_start = wiced_bt_mesh_scene_nvram_id_end - wiced_bt_mesh_scene_max_num;
    wiced_bt_mesh_scene_register_nvram_id = wiced_bt_mesh_scene_nvram_id_start - 1;

    wiced_bt_mesh_core_nvm_idx_node_data = wiced_bt_mesh_scene_register_nvram_id - 1;
    wiced_bt_mesh_core_nvm_idx_virt_addr = wiced_bt_mesh_core_nvm_idx_node_data - 1;
    wiced_bt_mesh_core_nvm_idx_frnd_state = wiced_bt_mesh_core_nvm_idx_virt_addr - 1;
    wiced_bt_mesh_core_nvm_idx_net_key_begin = wiced_bt_mesh_core_nvm_idx_frnd_state - wiced_bt_mesh_core_net_key_max_num;
    wiced_bt_mesh_core_nvm_idx_app_key_begin = wiced_bt_mesh_core_nvm_idx_net_key_begin - wiced_bt_mesh_core_app_key_max_num;
    wiced_bt_mesh_core_nvm_idx_health_state = wiced_bt_mesh_core_nvm_idx_app_key_begin - 1;
    wiced_bt_mesh_core_nvm_idx_cfg_data = wiced_bt_mesh_core_nvm_idx_health_state - ((cfg_data_len + (WICED_BT_MESH_CORE_NVRAM_CHUNK_MAX_SIZE - 1)) / WICED_BT_MESH_CORE_NVRAM_CHUNK_MAX_SIZE);
    mesh_nvm_idx_seq                                = wiced_bt_mesh_core_nvm_idx_cfg_data - 1;

    wiced_bt_mesh_core_nvm_idx_fw_distributor       = mesh_nvm_idx_seq - 1000;

    wiced_bt_mesh_core_nvm_idx_df_config            = mesh_nvm_idx_seq - 1000;

    printf("mesh_application_nvram_id_init: done\r\n");

    /* Initialization of curve */
    p_curve_p256 = &curve_p256;
}




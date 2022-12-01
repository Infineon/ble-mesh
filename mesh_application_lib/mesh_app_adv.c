/*
 * $ Copyright 2016-YEAR Cypress Semiconductor $
 */

/** @file
 *
 * Mesh advertisements related functionality
 *
 */

#include "wiced_bt_types.h"
#include "wiced_bt_gatt.h"
#include "wiced_bt_ble.h"
#include "wiced_bt_cfg.h"
#include "wiced_memory.h"
#include "wiced_bt_ota_firmware_upgrade.h"

#ifndef MESH_HOST_MODE
#include "wiced_hal_puart.h"
#include "wiced_hal_wdog.h"
#endif

#include "wiced_bt_stack.h"
#include "wiced_bt_mesh_app.h"
#include "wiced_bt_uuid.h"
#include "wiced_bt_mesh_core.h"
#include "wiced_bt_mesh_models.h"
#include "mesh_application.h"
#include "hci_control_api.h"
#include "wiced_timer.h"
#include "wiced_bt_trace.h"


#ifdef MESH_HOST_MODE
#include "mesh_auto_flags.h"
#endif


#ifdef MESH_OVER_GATT_ONLY
// ADV bearer is disabled. Create empty function.
wiced_bool_t mesh_app_adv_send_callback(uint8_t instance, uint8_t transmissions, uint32_t interval, uint8_t type,
    const uint8_t* data, uint8_t data_len, const uint8_t* scan_rsp_data, uint8_t scan_rsp_data_len)
{
    return WICED_FALSE;
}
#else // MESH_OVER_GATT_ONLY

#if defined(CYW43012) || defined(CYW4343W)
#if (defined(WICEDX_LINUX) || defined (MESH_HOST_MODE))
// Following macro enables one shot adv
#define MESH_CORE_ONE_SHOT_ADV
#else
#define MESH_CORE_ONE_SHOT_ADV
#endif
#endif

//#define MESH_APP_ADV_DEBUG_TRACE

#ifdef MESH_CORE_ONE_SHOT_ADV
#define MESH_APP_ADV_MAX_NUM_INSTANCES                  16
// Min value for Net and Relay retransmit interval is 10ms (16)
#define MESH_APP_ADV_MIN_INTERVAL                       16
#define MESH_APP_ADV_MAX_INTERVAL                       0x40000000
#else
#ifdef NEW_WICED_STACK
#define MULTI_ADV_MAX_NUM_INSTANCES LE_MULTI_ADV_MAX_NUM_INSTANCES
#endif
#define MESH_APP_ADV_MAX_NUM_INSTANCES                  MULTI_ADV_MAX_NUM_INSTANCES
// Min value for Net and Relay retransmit interval is 10ms (16) but device supports 20ms (32)
#define MESH_APP_ADV_MIN_INTERVAL                       32
// Max supported value is 0x4000 - it is defined in mpaf\btstack\stack\include\btm_ble_api.h
#define MESH_APP_ADV_MAX_INTERVAL                       0x4000
#endif

#ifdef MESH_CORE_ONE_SHOT_ADV

#define HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA              31
#define HCI_BRCM_ONESHOT_ADV                            (0x01DD | HCI_GRP_VENDOR_SPECIFIC)
#define MESH_APP_STD_ADV_STOP_DELAY                     100     // Stop SDT advert with that delay after start to have one adv sent

// Param for VSC HCI_BRCM_ONESHOT_ADV
typedef struct
{
    wiced_bt_ble_multi_advert_type_t               adv_type;
    wiced_bt_ble_address_type_t                    own_address_type;
    wiced_bt_device_address_t                      ownAddr;
    wiced_bt_ble_address_type_t                    peerAddrType;
    wiced_bt_device_address_t                      peerAddr;
    wiced_bt_ble_advert_chnl_map_t                 advertising_channel_map;
    wiced_bt_ble_multi_advert_filtering_policy_t   advertising_filter_policy;
    uint8_t                                        advDataLen;
    uint8_t                                        advData[HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA];
    uint8_t                                        scanrspDataLen;
    uint8_t                                        scanrspData[HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA];
    int8_t                                         transmit_power;
}one_shot_adv_param_t;
#define HCIC_PARAM_SIZE_SET_ONESHOT_ADV_PARAM    sizeof(one_shot_adv_param_t)

// Represents adv data to keep till end of advert transmissions
typedef struct
{
    uint64_t                next_time;          // for one shot adv: if is active_instance (its index == one_shot_adv_state_t::active_instance) then it is ignored.
                                                //                   otherwise it is a time to send next adv
                                                // for std adv: if is active_instance (its index == one_shot_adv_state_t::active_std_instance) then it is time to stop adv
                                                //              otherwise it is a time to start next adv
    uint32_t                interval;           // Transmissions interval in ms
    one_shot_adv_param_t    param;              // params of the VSC for that instance
    uint8_t                 transmissions;      // 0 means that instance is available for advertisements assignment. In that case all other members are ignored.
} one_shot_adv_t;

// State of the one shot adverts
typedef struct
{
    one_shot_adv_t  adv[MESH_APP_ADV_MAX_NUM_INSTANCES];
    uint8_t         active_instance;        // Instance of the one shot adv with sent VSC without complete received. If it is != 0 then it can't be same as active_std_instance
    uint8_t         active_std_instance;    // Instance of the standard adv started but not stopped. If it is != 0 then it can't be same as active_instance
    wiced_timer_t   timer;
    uint64_t        timer_end;
    wiced_bool_t    timer_initialized;
} one_shot_adv_state_t;

#ifndef MESH_HOST_MODE
typedef struct
{
    uint16_t    opcode;
    uint16_t    param_len;
    uint8_t     *p_param_buf;
} tBTM_VSC_CMPL;

typedef void (tBTM_VSC_CMPL_CB)(tBTM_VSC_CMPL *p1);

typedef uint8_t tBTM_STATUS;

#endif

#ifndef BTM_SUCCESS
#define BTM_SUCCESS                             0
#endif

#ifndef BTM_CMD_STARTED
#define BTM_CMD_STARTED                         1
#endif

tBTM_STATUS BTM_VendorSpecificCommand(uint16_t opcode, uint8_t param_len, uint8_t *p_param_buf, tBTM_VSC_CMPL_CB *p_cb);
void mesh_app_adv_send_cmpl_cb(tBTM_VSC_CMPL* p1);
void one_shot_adv_timer_callback(TIMER_PARAM_TYPE arg);

// State of the one shot adverts
one_shot_adv_state_t one_shot_adv_state = { 0 };

// Initializes of the meash adverts
void mesh_app_adv_init(void)
{
    WICED_BT_TRACE("Init one shot adv: timer_initialized:%d\n", one_shot_adv_state.timer_initialized);
    if (!one_shot_adv_state.timer_initialized)
    {
        memset(&one_shot_adv_state, 0, sizeof(one_shot_adv_state));
        one_shot_adv_state.timer_initialized = WICED_TRUE;
        wiced_init_timer(&one_shot_adv_state.timer, one_shot_adv_timer_callback, 0, WICED_MILLI_SECONDS_TIMER);
    }
}

// Resets of the mesh adverts
void mesh_app_adv_reset(void)
{
    wiced_bool_t    is_timer_in_use;
    wiced_result_t  res;
    WICED_BT_TRACE("Reset one shot adv: timer_initialized:%d active_std_instance:%d\n", one_shot_adv_state.timer_initialized, one_shot_adv_state.active_std_instance);
    if (one_shot_adv_state.timer_initialized)
    {
        is_timer_in_use = wiced_is_timer_in_use(&one_shot_adv_state.timer);
        WICED_BT_TRACE(" is_timer_in_use:%d\n", is_timer_in_use);
        if (is_timer_in_use)
            wiced_stop_timer(&one_shot_adv_state.timer);
        wiced_deinit_timer(&one_shot_adv_state.timer);
        // Stop std adv if it is active
        if (one_shot_adv_state.active_std_instance)
        {
            res = wiced_bt_start_advertisements(BTM_BLE_ADVERT_OFF, BLE_ADDR_RANDOM, NULL);
            WICED_BT_TRACE("stopped std adv. res:0x%x\n", res);
        }
        memset(&one_shot_adv_state, 0, sizeof(one_shot_adv_state));
    }
}

wiced_bool_t is_connectable(uint8_t adv_type)
{
    return adv_type == MULTI_ADVERT_CONNECTABLE_UNDIRECT_EVENT
        || adv_type == MULTI_ADVERT_CONNECTABLE_DIRECT_EVENT ? WICED_TRUE : WICED_FALSE;
}

// Start timer if needed.
void one_shot_adv_start_timer(uint64_t timer_end, uint64_t current_time)
{
    uint32_t delay;

#ifdef MESH_APP_ADV_DEBUG_TRACE
    WICED_BT_TRACE("one_shot_adv_start_timer: active_instance:%d active_std_instance:%d current_time:%x timer_end:%x active timer_end:%x\n",
        one_shot_adv_state.active_instance, one_shot_adv_state.active_std_instance, current_time, timer_end, (uint32_t)one_shot_adv_state.timer_end);
#endif

    // Make sure timer_end > current_time
    if (timer_end <= current_time)
        timer_end = current_time + 1;

    // If timer is active
    if (one_shot_adv_state.timer_end != 0)
    {
        // Do nothing if timer ends before requested time end. Otherwise stop timer.
        if (one_shot_adv_state.timer_end <= timer_end)
            return;
        wiced_stop_timer(&one_shot_adv_state.timer);
    }

    // Start timer
    one_shot_adv_state.timer_end = timer_end;
    delay = (uint32_t)(timer_end - current_time);
#ifdef MESH_APP_ADV_DEBUG_TRACE
    WICED_BT_TRACE("start one shot adv timer: delay:%d\n", delay);
#endif
    wiced_start_timer(&one_shot_adv_state.timer, delay);
}

// Converts adv data to the array of elements for wiced_bt_ble_set_raw_advertisement_data() and wiced_bt_ble_set_raw_scan_response_data()
// Returns written num elements. Returns 0 on error
uint8_t one_shot_adv_data_to_elements(uint8_t data_len, uint8_t* p_data, UINT8 num_elem_max, wiced_bt_ble_advert_elem_t* p_elements)
{
    uint8_t         num_elem = 0;
    uint8_t*        p = p_data;

    while (num_elem < num_elem_max && p < p_data + data_len)
    {
        p_elements[num_elem].len = *p++ - 1;
        if (p + 1 + p_elements[num_elem].len > p_data + data_len)
            break;
        p_elements[num_elem].advert_type = *p++;
        p_elements[num_elem].p_data = p;
        p += p_elements[num_elem].len;
        num_elem++;
    }
    if (p < p_data + data_len)
        num_elem = 0;
    return num_elem;
}

/**
* Delay timer callback function for function wiced_init_timer().
* It does nothing if there is active instance (started but not complete VSC)
* Finds soonest adv and sends VSC if its time is came. Otherwise starts timer.
*
* Parameters:
*   arg:    not used
*
* Return:   None
*/
void one_shot_adv_timer_callback(TIMER_PARAM_TYPE arg)
{
    uint8_t                 i, i_next, i_next_std;
    uint64_t                current_time;
    wiced_bt_dev_status_t   status;
    one_shot_adv_t*         p_adv;
    wiced_result_t          res;

#ifdef MESH_APP_ADV_DEBUG_TRACE
    WICED_BT_TRACE("one_shot_adv_timer_callback: active_instance:%d active_std_instance:%d timer_end:%x\n",
        one_shot_adv_state.active_instance, one_shot_adv_state.active_std_instance, (uint32_t)one_shot_adv_state.timer_end);
#endif

    // Mark timer as stopped
    one_shot_adv_state.timer_end = 0;

    current_time = wiced_bt_mesh_core_get_tick_count();

    // If std adv is active and it is time to stop it then stop it
    if (one_shot_adv_state.active_std_instance != 0)
    {
        p_adv = &one_shot_adv_state.adv[one_shot_adv_state.active_std_instance - 1];
        if (p_adv->next_time <= current_time)
        {
            one_shot_adv_state.active_std_instance = 0;
            p_adv->next_time += p_adv->interval - MESH_APP_STD_ADV_STOP_DELAY;
            res = wiced_bt_start_advertisements(BTM_BLE_ADVERT_OFF, BLE_ADDR_RANDOM, NULL);
#ifdef MESH_APP_ADV_DEBUG_TRACE
            WICED_BT_TRACE("stopped std adv. res:0x%x\n", res);
#endif
        }
    }

    // Find soonest one-shot adv and soonest std adv to send
    i_next = 0xff;      //unassigned index of the one-shot adv to send
    i_next_std = 0xff;  //unassigned index of the std adv to send
    for (i = 0; i < MESH_APP_ADV_MAX_NUM_INSTANCES; i++)
    {
        p_adv = &one_shot_adv_state.adv[i];
        // skip free item
        if (p_adv->transmissions == 0)
            continue;
        // if it is one shot(non-connectable) adv
        if (!is_connectable(p_adv->param.adv_type))
        {
            // if no active one-shot adv then update index of the soonest one-shot adv. Otherwise ignore that adv
            if (one_shot_adv_state.active_instance == 0)
            {
                if (i_next == 0xff || p_adv->next_time < one_shot_adv_state.adv[i_next].next_time)
                    i_next = i;
            }
        }
        // if it is std(connectable) adv and no active std adv then update index of the soonest std adv. Otherwise ignore that adv
        else if (one_shot_adv_state.active_std_instance == 0)
        {
            if (i_next_std == 0xff || p_adv->next_time < one_shot_adv_state.adv[i_next_std].next_time)
                i_next_std = i;
        }
    }
    // if we have one-shot adv to send
    if (i_next != 0xff)
    {
        p_adv = &one_shot_adv_state.adv[i_next];
        // If it is time to send that one shot adv then send VSC
        if (p_adv->next_time <= current_time)
        {
            status = BTM_VendorSpecificCommand(HCI_BRCM_ONESHOT_ADV, HCIC_PARAM_SIZE_SET_ONESHOT_ADV_PARAM, (uint8_t*)&p_adv->param, mesh_app_adv_send_cmpl_cb);
#ifdef MESH_APP_ADV_DEBUG_TRACE
            WICED_BT_TRACE(" sent VSC for instance:%d. status:%d\n", i_next + 1, status);
#endif
            // On success it becomes active one-shot instance
            if (status == BTM_CMD_STARTED)
                one_shot_adv_state.active_instance = i_next + 1;
            else
            {
                // on error remove that adv and notify mesh_core_lib as if it ended
                p_adv->transmissions = 0;
                wiced_bt_mesh_core_adv_end(i_next + 1);
                // handle adverts with 1 ms delay in the context of the timer callback
                one_shot_adv_start_timer(current_time + 1, current_time);
            }
        }
        else
        {
            // It is not time to send that adv - set timer for it
            one_shot_adv_start_timer(p_adv->next_time, current_time);
        }
    }

    // if we have std adv to send
    if (i_next_std != 0xff)
    {
        p_adv = &one_shot_adv_state.adv[i_next_std];
        // If it is time to send that std (connectable) adv then send it
        if (p_adv->next_time <= current_time)
        {
            wiced_bt_ble_advert_elem_t  elements[3];
            uint8_t                     num_elem_data, num_elem_scan;
            wiced_bt_dev_status_t       status_scan;
            wiced_result_t              res_data;

            num_elem_data = sizeof(elements) / sizeof(elements[0]);
            num_elem_data = one_shot_adv_data_to_elements(p_adv->param.advDataLen, p_adv->param.advData, num_elem_data, elements);
            res_data = wiced_bt_ble_set_raw_advertisement_data(num_elem_data, elements);
            num_elem_scan = sizeof(elements) / sizeof(elements[0]);
            num_elem_scan = one_shot_adv_data_to_elements(p_adv->param.scanrspDataLen, p_adv->param.scanrspData, num_elem_scan, elements);
            status_scan = wiced_bt_ble_set_raw_scan_response_data(num_elem_scan, elements);
            res = wiced_bt_start_advertisements(BTM_BLE_ADVERT_UNDIRECTED_HIGH, p_adv->param.own_address_type, p_adv->param.ownAddr);
            one_shot_adv_state.active_std_instance = i_next_std + 1;
            p_adv->next_time = current_time + MESH_APP_STD_ADV_STOP_DELAY;
            one_shot_adv_start_timer(p_adv->next_time, current_time);
#ifdef MESH_APP_ADV_DEBUG_TRACE
            WICED_BT_TRACE("started std adv: instance:%d num_elem_data:%d res_data:0x%x num_elem_scan:%d status_scan:0x%x res:0x%x\n",
                one_shot_adv_state.active_std_instance, num_elem_data, res_data, num_elem_scan, status_scan, res);
#endif
        }
        else
        {
            // It is not time to send that adv - set timer for it
            one_shot_adv_start_timer(p_adv->next_time, current_time);
        }
    }
}

void mesh_app_adv_send_cmpl_cb(tBTM_VSC_CMPL* p1)
{
    one_shot_adv_t* p_adv;
    uint8_t         instance;
    uint64_t        current_time;

    // If there is no active instance then return - it should never happen.
    if (one_shot_adv_state.active_instance == 0)
    {
        WICED_BT_TRACE("ADV transmitted: there is no active instance\n");
        return;
    }

    uint8_t *p_param_buf = p1->p_param_buf;
    uint8_t retval = p_param_buf[0];
    WICED_BT_TRACE(" One Shot ADV support:%d\n", retval);

    instance = one_shot_adv_state.active_instance;
    one_shot_adv_state.active_instance = 0;
    p_adv = &one_shot_adv_state.adv[instance - 1];

#ifdef MESH_APP_ADV_DEBUG_TRACE
    // print trace on first 32 callbacks and then once in 16 callbacks
    if (p_adv->transmissions < 32 || p_adv->transmissions == 0xff)
    {
        WICED_BT_TRACE("ADV transmitted: instance:%d transmissions:%d\n", instance, p_adv->transmissions);
    }
#endif

    // if it is invalid instance then just return - it should never happen.
    if (p_adv->transmissions == 0)
        return;

    p_adv->transmissions--;

    // If it is forever-adv(beacon) then keep it sending - we use 0xf0 amd 0xff values to print adv info once in 16 callbacks
    if (p_adv->transmissions == 0xf0)
        p_adv->transmissions = 0xff;

    current_time = wiced_bt_mesh_core_get_tick_count();

    // If last transmission has been sent then notify mesh_core_lib
    if (p_adv->transmissions == 0)
    {
        wiced_bt_mesh_core_adv_end(instance);
        // handle adverts with 1 ms delay in the context of the timer callback
        one_shot_adv_start_timer(current_time + 1, current_time);
        return;
    }

    // Not last transmission has been sent. Then set next transmission time.
    p_adv->next_time = current_time + p_adv->interval;
    one_shot_adv_start_timer(p_adv->next_time, current_time);
}

wiced_bool_t mesh_app_adv_send_callback(uint8_t instance, uint8_t transmissions, uint32_t interval, uint8_t type, uint8_t* own_bd_addr,
    const uint8_t* data, uint8_t data_len, const uint8_t* scan_rsp_data, uint8_t scan_rsp_data_len)
{
    wiced_bt_dev_status_t   status;
    one_shot_adv_t*         p_adv;
    // Translate transmit_power to dbm
    const int8_t transmit_power_to_dbm[] = { -12,   // BTM_BLE_ADV_TX_POWER_MIN
                                             -8,    // BTM_BLE_ADV_TX_POWER_LOW
                                             -4,    // BTM_BLE_ADV_TX_POWER_MID
                                             0,     // BTM_BLE_ADV_TX_POWER_UPPER
                                             4 };   // BTM_BLE_ADV_TX_POWER_MAX
#ifdef MESH_APP_ADV_DEBUG_TRACE
    WICED_BT_TRACE("Send or Stop Adv: instance:%d transmissions:%d interval:%d type:%d data_len:%d scan_rsp_data_len:%d active_instance:%d active_std_instance:%d timer_initialized:%d\n",
        instance, transmissions, interval, type, data_len, scan_rsp_data_len, one_shot_adv_state.active_instance, one_shot_adv_state.active_std_instance, one_shot_adv_state.timer_initialized);
#endif

    // Initialize adverts state if it is first call after startup or after reset
    if (!one_shot_adv_state.timer_initialized)
        mesh_app_adv_init();

    // 0 instance means reset
    if (instance == 0)
    {
        mesh_app_adv_reset();
        return WICED_TRUE;
    }

    // Validate instance
    if (instance > MESH_APP_ADV_MAX_NUM_INSTANCES)
        return WICED_FALSE;

    p_adv = &one_shot_adv_state.adv[instance - 1];

#ifdef MESH_APP_ADV_DEBUG_TRACE
    WICED_BT_TRACE(" active transmissions:%d\n", p_adv->transmissions);
#endif

    // 0 transmissions means stop adv
    if (transmissions == 0)
    {
        if (p_adv->transmissions == 0)
            return WICED_FALSE;
        p_adv->transmissions = 0;
        // if it is active std(connectable) adv then stop it
        if (one_shot_adv_state.active_std_instance == instance)
        {
            status = wiced_bt_start_advertisements(BTM_BLE_ADVERT_OFF, BLE_ADDR_RANDOM, NULL);
#ifdef MESH_APP_ADV_DEBUG_TRACE
            WICED_BT_TRACE("stopped std adv. status:0x%x\n", status);
#endif
        }
        return WICED_TRUE;
    }

    // Make sure that instance isn't busy
    if (p_adv->transmissions != 0)
        return WICED_FALSE;

    // Update interval in accordance with restrictions
    if (interval < MESH_APP_ADV_MIN_INTERVAL)
        interval = MESH_APP_ADV_MIN_INTERVAL;
    if (interval > MESH_APP_ADV_MAX_INTERVAL)
        interval = MESH_APP_ADV_MAX_INTERVAL;

    // Validate all parameters
    if ((wiced_bt_mesh_core_adv_tx_power < MULTI_ADV_TX_POWER_MIN) || (wiced_bt_mesh_core_adv_tx_power > MULTI_ADV_TX_POWER_MAX)
        || (data_len > HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA)
        || (scan_rsp_data_len > HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA))
    {
        WICED_BT_TRACE(" invalid param\n");
        return WICED_FALSE;
    }

    p_adv->transmissions = transmissions;
    p_adv->interval = interval;
    p_adv->next_time = wiced_bt_mesh_core_get_tick_count();

    // Prepara param for VSC
    p_adv->param.adv_type = type;
    p_adv->param.own_address_type = BLE_ADDR_RANDOM;
    memcpy(p_adv->param.ownAddr, own_bd_addr, BD_ADDR_LEN);
    p_adv->param.peerAddrType = BLE_ADDR_PUBLIC;
    memset(p_adv->param.peerAddr, 0, BD_ADDR_LEN);
    p_adv->param.advertising_channel_map = BTM_BLE_DEFAULT_ADVERT_CHNL_MAP;
    p_adv->param.advertising_filter_policy = MULTI_ADVERT_FILTER_POLICY_NOT_USED;
    p_adv->param.advDataLen = data_len;
    if (data_len > 0)
        memcpy(p_adv->param.advData, data, data_len);
    if (data_len < HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA)
        memset(&p_adv->param.advData[data_len], 0, HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA - data_len);
    p_adv->param.scanrspDataLen = scan_rsp_data_len;
    if (scan_rsp_data_len > 0)
        memcpy(p_adv->param.scanrspData, scan_rsp_data, scan_rsp_data_len);
    if (scan_rsp_data_len < HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA)
        memset(&p_adv->param.scanrspData[scan_rsp_data_len], 0, HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA - scan_rsp_data_len);
    p_adv->param.transmit_power = transmit_power_to_dbm[wiced_bt_mesh_core_adv_tx_power];

    // handle adverts with 1 ms delay in the context of the timer callback
    one_shot_adv_start_timer(p_adv->next_time + 1, p_adv->next_time);
    return WICED_TRUE;
}

#elif !defined(CYW20829) && defined (MESH_HOST_MODE)
// Use multi ADV

// Number of microseconds before the ADV the notification is to be sent. Will be rounded down to
// the nearest 1.25mS. Has to be an even multiple of 625uS.
#define MESH_ADVANCE_NOTICE_FOR_ADV                     1250

// Flags of the initialized notifications
uint16_t mesh_app_ntf_init_flags = 0;
// Remaining number of retransmissions
uint8_t mesh_app_transmissions[MESH_APP_ADV_MAX_NUM_INSTANCES] = { 0 };

void mesh_app_adv_init(void)
{
    WICED_BT_TRACE("Init App Adv: ntf_init_flags:%x\n", mesh_app_ntf_init_flags);
    // If it is first call
    if(mesh_app_ntf_init_flags == 0)
        memset(mesh_app_transmissions, 0, sizeof(mesh_app_transmissions));
}

void mesh_app_adv_reset(void)
{
    WICED_BT_TRACE("Reset App Adv: ntf_init_flags:%x\n", mesh_app_ntf_init_flags);
    mesh_app_ntf_init_flags = 0;
    memset(mesh_app_transmissions, 0, sizeof(mesh_app_transmissions));
}

void mesh_app_adv_stop(uint8_t instance)
{
    wiced_result_t  result;
#ifdef MESH_APP_ADV_DEBUG_TRACE
    WICED_BT_TRACE("Stop Adv: instance:%d\n", instance);
#endif

    result = wiced_start_multi_advertisements(MULTI_ADVERT_STOP, instance);
    if (result)
    {
        WICED_BT_TRACE(" wiced_start_multi_advertisements(stop) failed. result:%d\n", result);
    }
}


uint32_t mesh_num_transmitted_advs = 0;

/**
* Advertising transmission callback before and after an adv is sent. It is called not in the app thread.
*
* Parameters:
*   status:     LSB - Type of the notification (before or after)
*               MSB - Advertisment instance.
*
* Return:   None
*
*/
void mesh_app_adv_notifyAdvPacketTransmissions_cb(uint32_t status)
{
    uint8_t     type, instance;
    type = (status & 0xFF);
    instance = (status >> 8) & 0xFF;

    // handle only DONE notifications with correct instance
    if (type == WICED_BT_ADV_NOTIFICATION_DONE && instance != 0 && instance <= sizeof(mesh_app_transmissions))
    {
#ifdef MESH_APP_ADV_DEBUG_TRACE
        // print trace on first 32 callbacks and then once in 16 callbacks
        if (mesh_app_transmissions[instance - 1] < 32 || mesh_app_transmissions[instance - 1] == 0xff)
        {
            WICED_BT_TRACE("ADV transmitted: instance:%d transmissions:%d\n", instance, mesh_app_transmissions[instance - 1]);
        }
#endif

        // Sum to keep track of total # of ADV transmissions confirmed by the controller
        // This is count the number of transmissions during RELAY - not concerned about beacons etc.
        if(mesh_app_transmissions[instance - 1] < 10)
            mesh_num_transmitted_advs += mesh_app_transmissions[instance - 1];

        // if it is invalid instance then stop adv has been lost or not handled yet in the FW. Stop that advert again.
        if (mesh_app_transmissions[instance - 1] == 0)
        {
            mesh_app_adv_stop(instance);
            return;
        }

        mesh_app_transmissions[instance - 1]--;

        // If it is forever-adv(beacon) then keep it sending - we use 0xf0 amd 0xff values to print adv info once in 16 callbacks
        if (mesh_app_transmissions[instance - 1] == 0xf0)
            mesh_app_transmissions[instance - 1] = 0xff;

        // If last advert has been sent then stop it and notify mesh_core_lib
        if (mesh_app_transmissions[instance - 1] == 0)
        {
            mesh_app_transmissions[instance - 1] = 0;
            mesh_app_adv_stop(instance);
            wiced_bt_mesh_core_adv_end(instance);
        }
    }
}

wiced_bool_t mesh_app_adv_send_callback(uint8_t instance, uint8_t transmissions, uint32_t interval, uint8_t type, uint8_t* own_bd_addr,
    const uint8_t* data, uint8_t data_len, const uint8_t* scan_rsp_data, uint8_t scan_rsp_data_len)
{
    wiced_result_t  result;
    uint16_t        ntf_flag;
#ifdef MESH_APP_ADV_DEBUG_TRACE
    WICED_BT_TRACE("Send or Stop Adv: instance:%d transmissions:%d interval:%d type:%d data_len:%d scan_rsp_data_len:%d ntf_init_flags:%x\n",
        instance, transmissions, interval, type, data_len, scan_rsp_data_len, mesh_app_ntf_init_flags);
#endif

    // Initialize adverts state if it is first call after startup or after reset
    if (mesh_app_ntf_init_flags == 0)
        mesh_app_adv_init();
    // 0 instance means reset
    if (instance == 0)
    {
        mesh_app_adv_reset();
        return WICED_TRUE;
    }

    // Validate instance
    if (instance > MESH_APP_ADV_MAX_NUM_INSTANCES)
        return WICED_FALSE;

#ifdef MESH_APP_ADV_DEBUG_TRACE
    WICED_BT_TRACE(" active transmissions:%d\n", mesh_app_transmissions[instance - 1]);
#endif

    // 0 transmissions means stop adv
    if (transmissions == 0)
    {
        if (mesh_app_transmissions[instance - 1] == 0)
            return WICED_FALSE;
        mesh_app_transmissions[instance - 1] = 0;
        mesh_app_adv_stop(instance);
        return WICED_TRUE;
    }

    // Make sure that instance isn't busy
    if (mesh_app_transmissions[instance - 1] != 0)
        return WICED_FALSE;

    // Update interval in accordance with restrictions
    if (interval < MESH_APP_ADV_MIN_INTERVAL)
        interval = MESH_APP_ADV_MIN_INTERVAL;
    if (interval > MESH_APP_ADV_MAX_INTERVAL)
        interval = MESH_APP_ADV_MAX_INTERVAL;

    // initialize notifications once for each instance
    ntf_flag = ((uint16_t)1) << (instance - 1);
    if ((mesh_app_ntf_init_flags & ntf_flag) == 0)
    {
#ifdef MESH_APP_ADV_DEBUG_TRACE
        WICED_BT_TRACE("adv_send_callback: initialize notifications %d\n", instance);
#endif
        mesh_app_ntf_init_flags |= ntf_flag;
        // Set the callback function before and after an adv is sent
        if (!wiced_bt_notify_multi_advertisement_packet_transmissions(instance, mesh_app_adv_notifyAdvPacketTransmissions_cb, MESH_ADVANCE_NOTICE_FOR_ADV))
        {
            WICED_BT_TRACE("mesh_app_notify_multi_adv_transmissions failed\n");
            return WICED_FALSE;
        }
    }

    result = wiced_set_multi_advertisement_data((uint8_t*)data, data_len, instance);
    if (result)
    {
        WICED_BT_TRACE("wiced_set_multi_advertisement_data failed. result:%d\n", result);
        return WICED_FALSE;
    }

#if ( defined(CYW20735B1) || defined(CYW20835B1) || defined(CYW20819A1) || defined(CYW20721B2) || defined(CYW20719B2)) || defined (NEW_WICED_STACK)
    {
        wiced_bt_ble_multi_adv_params_t params = {
            .adv_int_min = interval,
            .adv_int_max = interval,
            .adv_type = type,
            .channel_map = BTM_BLE_DEFAULT_ADVERT_CHNL_MAP,
            .adv_filter_policy = MULTI_ADVERT_FILTER_POLICY_NOT_USED, //TODO this define not the latest
            .adv_tx_power = wiced_bt_mesh_core_adv_tx_power,
            .peer_addr_type = BLE_ADDR_PUBLIC,
            .own_addr_type = BLE_ADDR_RANDOM
        };
        memset(params.peer_bd_addr, 0, sizeof(params.peer_bd_addr));
        memcpy(params.own_bd_addr, own_bd_addr, sizeof(params.own_bd_addr));
        result = wiced_set_multi_advertisement_params(instance, &params);
    }
#else
    {
        uint8_t null_bda_[6] = { 0 };    // 0-filled BDA
        result = wiced_set_multi_advertisement_params(interval, interval, type,
            BLE_ADDR_RANDOM, own_bd_addr, BLE_ADDR_PUBLIC, null_bda_,
            BTM_BLE_DEFAULT_ADVERT_CHNL_MAP, MULTI_ADVERT_FILTER_POLICY_FILTER_ACCEPT_LIST_NOT_USED,
            instance, wiced_bt_mesh_core_adv_tx_power);
    }
#endif
    if (result)
    {
        WICED_BT_TRACE("set_multi_advertisement_params failed. result:%d\n", result);
        return WICED_FALSE;
    }
    // if scan response has been requested for that instance
    if (scan_rsp_data_len != 0)
    {
        result = wiced_set_multi_advertisement_scan_response_data((uint8_t*)scan_rsp_data, scan_rsp_data_len, instance);
        if (result)
        {
            WICED_BT_TRACE("set_multi_advertisement_scan_response_data failed. result:%d\n", result);
            return WICED_FALSE;
        }
    }

    // Start advertisement
    result = wiced_start_multi_advertisements(MULTI_ADVERT_START, instance);
    if (result)
    {
        WICED_BT_TRACE("start_multi_advertisements(START) failed. result:%d\n", result);
        return WICED_FALSE;
    }
    mesh_app_transmissions[instance - 1] = transmissions;
    return WICED_TRUE;
}

#elif defined(CYW20829) && defined (MESH_HOST_MODE) // if 20829 and ExtAdv

// Number of microseconds before the ADV the notification is to be sent. Will be rounded down to
// the nearest 1.25mS. Has to be an even multiple of 625uS.
#define MESH_ADVANCE_NOTICE_FOR_ADV                     1250

// Flags of the initialized notifications
uint16_t mesh_app_ntf_init_flags = 0;
// Remaining number of retransmissions
uint8_t mesh_app_transmissions[MESH_APP_ADV_MAX_NUM_INSTANCES] = { 0 };


#define tBTM_BLE_ADV_EXT_EVENT        wiced_bt_ble_adv_ext_event_t
#define tBTM_BLE_ADV_EXT_EVENT_DATA   wiced_bt_ble_adv_ext_event_data_t

void mesh_ble_ext_adv_handler (tBTM_BLE_ADV_EXT_EVENT event, tBTM_BLE_ADV_EXT_EVENT_DATA* p_data);

uint32_t mesh_num_transmitted_advs = 0;

#include "cy_smif.h"


extern void mesh_trace_debug(const char* p_fmt, ...);

CY_SECTION_RAMFUNC_BEGIN
void mesh_app_adv_stop(uint8_t instance)
{
    wiced_result_t  result;
#ifdef MESH_APP_ADV_DEBUG_TRACE
    WICED_BT_TRACE("Stop Adv: instance:%d\n", instance);
#endif

    wiced_bt_ble_ext_adv_duration_config_t dcfg = {instance,  0,  0 };

    result = wiced_bt_ble_start_ext_adv(MULTI_ADVERT_STOP, 1, &dcfg);

    if (result)
    {
        WICED_BT_TRACE(" wiced_bt_ble_start_ext_adv(stop). result:%d\n", result);
    }
}
CY_SECTION_RAMFUNC_END

CY_SECTION_RAMFUNC_BEGIN
void mesh_app_adv_notifyAdvPacketTransmissions_cb(uint8_t instance)
{
    // handle only DONE notifications with correct instance
    if ( instance != 0 && instance <= sizeof(mesh_app_transmissions))
    {
#ifdef MESH_APP_ADV_DEBUG_TRACE
        WICED_BT_TRACE("ADV transmitted: instance:%d transmissions:%d\n", instance, mesh_app_transmissions[instance - 1]);
#endif
        // Sum to keep track of total # of ADV transmissions confirmed by the controller
        // This is count the number of transmissions during RELAY - not concerned about beacons etc.
        if(mesh_app_transmissions[instance - 1] < 10)
            mesh_num_transmitted_advs += mesh_app_transmissions[instance - 1];

        // if it is invalid instance then stop adv has been lost or not handled yet in the FW. Stop that advert again.
        if (mesh_app_transmissions[instance - 1] == 0)
        {
            mesh_app_adv_stop(instance);
            return;
        }

        if (mesh_app_transmissions[instance - 1] != 0xff)
            mesh_app_transmissions[instance - 1] = 0;

        // If last advert has been sent then stop it and notify mesh_core_lib
        if (mesh_app_transmissions[instance - 1] == 0)
        {
            wiced_bt_mesh_core_adv_end(instance);
        }
    }
}
CY_SECTION_RAMFUNC_END

CY_SECTION_RAMFUNC_BEGIN
void mesh_ble_ext_adv_handler(tBTM_BLE_ADV_EXT_EVENT event, tBTM_BLE_ADV_EXT_EVENT_DATA *ped)
{
    WICED_BT_TRACE("[%s] Event: %d", __FUNCTION__, event);

    /* Save the data in case script wants to read it. */
    if (ped == NULL)
        return;

    switch (event)
    {
    case WICED_BT_BLE_SCAN_REQUEST_RECEIVED_EVENT:
        WICED_BT_TRACE("[%s] Event: %d", __FUNCTION__, event);
    break;

    case WICED_BT_BLE_ADV_SET_TERMINATED_EVENT:
        mesh_app_adv_notifyAdvPacketTransmissions_cb(ped->adv_set_terminated.adv_handle);
    break;

        default:
        break;
    }
}
CY_SECTION_RAMFUNC_END

static uint8_t supported_adv;

CY_SECTION_RAMFUNC_BEGIN
void mesh_app_adv_init(void)
{
    // If it is first call
    if(mesh_app_ntf_init_flags == 0)
        memset(mesh_app_transmissions, 0, sizeof(mesh_app_transmissions));

    supported_adv = wiced_bt_ble_read_num_ext_adv_sets();
    WICED_BT_TRACE("Init App Adv: ntf_init_flags:%x num_ext_adv_sets:%d MAX_NUM_INSTANCES=%d\n", mesh_app_ntf_init_flags, supported_adv, MESH_APP_ADV_MAX_NUM_INSTANCES);
    if (supported_adv > MESH_APP_ADV_MAX_NUM_INSTANCES)
    {
        supported_adv = MESH_APP_ADV_MAX_NUM_INSTANCES;
    }
    // Update mesh_core_lib with max supported adv instances
    wiced_bt_mesh_core_set_max_num_adv_instances(supported_adv);

    wiced_bt_ble_register_adv_ext_cback(mesh_ble_ext_adv_handler);
}
CY_SECTION_RAMFUNC_END

CY_SECTION_RAMFUNC_BEGIN
void mesh_app_adv_reset(void)
{
    WICED_BT_TRACE("Reset App Adv: ntf_init_flags:%x\n", mesh_app_ntf_init_flags);
    mesh_app_ntf_init_flags = 0;
    memset(mesh_app_transmissions, 0, sizeof(mesh_app_transmissions));


}
CY_SECTION_RAMFUNC_END



static wiced_bt_device_address_t                peer_addr = {0};

/* Adv parameter defines */
#define PARAM_EVENT_PROPERTY    (WICED_BT_BLE_EXT_ADV_EVENT_CONNECTABLE_ADV | \
                                 WICED_BT_BLE_EXT_ADV_EVENT_SCANNABLE_ADV | \
                                 WICED_BT_BLE_EXT_ADV_EVENT_LEGACY_ADV)    // wiced_bt_ble_ext_adv_event_property_t


#define PARAM_FILTER_POLICY     (BTM_BLE_ADV_POLICY_ACCEPT_CONN_AND_SCAN)       // wiced_bt_ble_advert_filter_policy_t
#define PARAM_TX_POWER_MAX      0x7f                                            // tBTM_BLE_ADV_TX_POWER
#define PARAM_SCAN_REQ_NOTIF    WICED_BT_BLE_EXT_ADV_SCAN_REQ_NOTIFY_ENABLE     // wiced_bt_ble_ext_adv_scan_req_notification_setting_t

#define mesh_set_instance_params(instance, interval)   wiced_bt_ble_set_ext_adv_parameters(\
    instance,                           /* wiced_bt_ble_ext_adv_handle_t adv_handle */ \
    PARAM_EVENT_PROPERTY,               /* wiced_bt_ble_ext_adv_event_property_t event_properties */ \
    interval,                           /* uint32_t primary_adv_int_min */ \
    interval,                           /* uint32_t primary_adv_int_max */ \
    BTM_BLE_DEFAULT_ADVERT_CHNL_MAP,    /* wiced_bt_ble_advert_chnl_map_t primary_adv_channel_map */ \
    BLE_ADDR_RANDOM,                    /* wiced_bt_ble_address_type_t own_addr_type */ \
    BLE_ADDR_PUBLIC,                    /* wiced_bt_ble_address_type_t peer_addr_type */ \
    peer_addr,                          /* wiced_bt_device_address_t peer_addr */ \
    PARAM_FILTER_POLICY,                /* wiced_bt_ble_advert_filter_policy_t adv_filter_policy */ \
    PARAM_TX_POWER_MAX,                 /* int8_t adv_tx_power */ \
    WICED_BT_BLE_EXT_ADV_PHY_1M,        /* wiced_bt_ble_ext_adv_phy_t primary_adv_phy */ \
    0,                                  /* uint8_t secondary_adv_max_skip */ \
    0,                                  /* wiced_bt_ble_ext_adv_phy_t secondary_adv_phy */ \
    0,                                  /* wiced_bt_ble_ext_adv_sid_t adv_sid */ \
    PARAM_SCAN_REQ_NOTIF);              /* wiced_bt_ble_ext_adv_scan_req_notification_setting_t scan_request_not */


CY_SECTION_RAMFUNC_BEGIN
wiced_bool_t mesh_app_adv_send_callback(uint8_t instance, uint8_t transmissions, uint32_t interval, uint8_t type, uint8_t* own_bd_addr,
    const uint8_t* data, uint8_t data_len, const uint8_t* scan_rsp_data, uint8_t scan_rsp_data_len)
{
    wiced_result_t  result;
    uint16_t        ntf_flag;

    uint16_t max_adv_len =  wiced_bt_ble_read_max_ext_adv_data_len();

#ifdef MESH_APP_ADV_DEBUG_TRACE
    WICED_BT_TRACE("Send or Stop Adv: instance:%d transmissions:%d interval:%d type:%d data_len:%d scan_rsp_data_len:%d ntf_init_flags:%x\n",
        instance, transmissions, interval, type, data_len, scan_rsp_data_len, mesh_app_ntf_init_flags);
#endif

    // Initialize adverts state if it is first call after startup or after reset
    if (mesh_app_ntf_init_flags == 0)
        mesh_app_adv_init();

    // 0 instance means reset
    if (instance == 0)
    {
        mesh_app_adv_reset();
        return WICED_TRUE;
    }

    // Validate instance
    if (instance > supported_adv)
        return WICED_FALSE;

#ifdef MESH_APP_ADV_DEBUG_TRACE
    WICED_BT_TRACE(" active transmissions:%d\n", mesh_app_transmissions[instance - 1]);
#endif

    // 0 transmissions means stop adv
    if (transmissions == 0)
    {
        if (mesh_app_transmissions[instance - 1] == 0)
            return WICED_FALSE;
        mesh_app_transmissions[instance - 1] = 0;
        mesh_app_adv_stop(instance);
        return WICED_TRUE;
    }

    // Make sure that instance isn't busy
    if (mesh_app_transmissions[instance - 1] != 0)
        return WICED_FALSE;

    // Update interval in accordance with restrictions
    if (interval < MESH_APP_ADV_MIN_INTERVAL)
        interval = MESH_APP_ADV_MIN_INTERVAL;
    if (interval > MESH_APP_ADV_MAX_INTERVAL)
        interval = MESH_APP_ADV_MAX_INTERVAL;

    // initialize notifications once for each instance
    ntf_flag = ((uint16_t)1) << (instance - 1);
    if ((mesh_app_ntf_init_flags & ntf_flag) == 0)
    {
#ifdef MESH_APP_ADV_DEBUG_TRACE
        WICED_BT_TRACE("adv_send_callback: initialize notifications %d\n", instance);
#endif
        mesh_app_ntf_init_flags |= ntf_flag;
    }

    uint8_t new_type = WICED_BT_BLE_EXT_ADV_EVENT_LEGACY_ADV;
    switch (type)
    {
        case MULTI_ADVERT_CONNECTABLE_UNDIRECT_EVENT:
            new_type |= WICED_BT_BLE_EXT_ADV_EVENT_SCANNABLE_ADV | WICED_BT_BLE_EXT_ADV_EVENT_CONNECTABLE_ADV;
            break;

        case MULTI_ADVERT_NONCONNECTABLE_EVENT:
        default:
            break;
    }


    WICED_BT_TRACE("type:%d new_type:%d \n", type, new_type);

    result = wiced_bt_ble_set_ext_adv_parameters(\
        instance,                           /* wiced_bt_ble_ext_adv_handle_t adv_handle */ \
        new_type,                           /* wiced_bt_ble_ext_adv_event_property_t event_properties */ \
        interval,                           /* uint32_t primary_adv_int_min */ \
        interval,                           /* uint32_t primary_adv_int_max */ \
        BTM_BLE_DEFAULT_ADVERT_CHNL_MAP,    /* wiced_bt_ble_advert_chnl_map_t primary_adv_channel_map */ \
        BLE_ADDR_RANDOM,                    /* wiced_bt_ble_address_type_t own_addr_type */ \
        BLE_ADDR_PUBLIC,                    /* wiced_bt_ble_address_type_t peer_addr_type */ \
        peer_addr,                          /* wiced_bt_device_address_t peer_addr */ \
        PARAM_FILTER_POLICY,                /* wiced_bt_ble_advert_filter_policy_t adv_filter_policy */ \
        PARAM_TX_POWER_MAX,                 /* int8_t adv_tx_power */ \
        WICED_BT_BLE_EXT_ADV_PHY_1M,        /* wiced_bt_ble_ext_adv_phy_t primary_adv_phy */ \
        0,                                  /* uint8_t secondary_adv_max_skip */ \
        0,                                  /* wiced_bt_ble_ext_adv_phy_t secondary_adv_phy */ \
        0,                                  /* wiced_bt_ble_ext_adv_sid_t adv_sid */ \
        PARAM_SCAN_REQ_NOTIF);              /* wiced_bt_ble_ext_adv_scan_req_notification_setting_t scan_request_not */



    if (result)
    {
        WICED_BT_TRACE("set_multi_advertisement_params failed. result:%d\n", result);
        return WICED_FALSE;
    }

    wiced_bt_ble_set_ext_adv_random_address (instance, own_bd_addr);


      if( data_len > max_adv_len)
      {
          WICED_BT_TRACE("data length greater than max supported adv length max_supported: %d data_len: %d\n",  max_adv_len, data_len);
          return WICED_FALSE;
      }


    /* Sets adv data for this instance & start to adv */
    result =  wiced_bt_ble_set_ext_adv_data(instance, data_len, (uint8_t*)data);

    if (result)
    {
        WICED_BT_TRACE("wiced_set_multi_advertisement_data failed. result:%d\n", result);
        return WICED_FALSE;
    }

    // if scan response has been requested for that instance
    if (scan_rsp_data_len != 0)
    {
    result =  wiced_bt_ble_set_ext_scan_rsp_data(instance, scan_rsp_data_len, (uint8_t*)scan_rsp_data);

        if (result)
        {
            WICED_BT_TRACE("set_multi_advertisement_scan_response_data failed. result:%d\n", result);
            return WICED_FALSE;
        }
    }

    // Start advertisement
    interval /= 16;

    wiced_bt_ble_ext_adv_duration_config_t dcfg = {instance,  interval * (transmissions-1) + interval/2,  transmissions };

    if( transmissions == 255)
    {
        dcfg.adv_duration = 0;
        dcfg.max_ext_adv_events = 0;
    }

     WICED_BT_TRACE("START wiced_bt_ble_start_ext_adv \n");

    result =     wiced_bt_ble_start_ext_adv(MULTI_ADVERT_START, 1, &dcfg);
    if (result)
    {
        WICED_BT_TRACE("start_multi_advertisements(START) failed. result:%d\n", result);
        return WICED_FALSE;
    }

    mesh_app_transmissions[instance - 1] = transmissions;
    return WICED_TRUE;
}
CY_SECTION_RAMFUNC_END

#endif // MESH_CORE_ONE_SHOT_ADV
#endif // MESH_OVER_GATT_ONLY

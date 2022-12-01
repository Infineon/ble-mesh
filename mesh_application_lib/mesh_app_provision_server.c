/*
 * $ Copyright 2016-YEAR Cypress Semiconductor $
 */

/** @file
 *
 * This file contains functionality required for the device to be provisioned to be
 * a part of the mesh network.
 *
 */

#include "sparcommon.h"
#include "wiced_bt_types.h"
#include "wiced_bt_ble.h"
#include "wiced_bt_gatt.h"
#include "wiced_bt_cfg.h"
#include "wiced_bt_factory_app_config.h"
#include "wiced_bt_uuid.h"
#include "wiced_bt_mesh_provision.h"
#include "wiced_bt_mesh_core.h"
#include "wiced_bt_trace.h"
#include "bt_gatt_db.h"
#include "mesh_application.h"

#if ( defined(CYW20719B0) || defined(CYW20719B1) || defined(CYW20721B1) || defined (CYW20819A1) ||defined(CYW20719B2) || defined(CYW20721B2) )
#include "wiced_memory.h"
#elif ( defined(CYW20735B0) || defined(CYW20735B1) || defined(CYW20835B1) )
#include "wiced_gki.h"
#endif

/******************************************************
 *          Structures
 ******************************************************/
typedef struct
{
    uint32_t conn_id;
    wiced_bt_mesh_provision_server_callback_t *p_parent_callback;
} mesh_app_provision_state_t;

/******************************************************
 *          Function Prototypes
 ******************************************************/
static void            mesh_app_provision_started(uint32_t conn_id);
static void            mesh_app_provision_end(uint32_t conn_id, uint16_t addr, uint16_t net_key_idx, uint8_t result, const uint8_t *p_dev_key);
static void            mesh_app_provision_gatt_send_cb(uint16_t conn_id, const uint8_t *packet, uint32_t packet_len);
static wiced_bool_t    mesh_app_provision_get_oob(uint32_t conn_id, uint8_t type, uint8_t size, uint8_t action);
static wiced_bool_t    mesh_app_provision_get_capabilities(uint32_t conn_id);
static uint16_t        mesh_app_provision_get_records(uint16_t *buffer);
static uint16_t        mesh_app_provision_record_request(uint16_t record_id, uint8_t *buffer, uint16_t fragment_length, uint16_t fragment_offset, uint16_t *record_size);

/******************************************************
 *          Variables Definitions
 ******************************************************/
wiced_bt_mesh_provision_capabilities_data_t provisioning_config =
{
    .pub_key_type      = 0,      // If 1 Public Key OOB information available
    .static_oob_type   = 0,      // Supported static OOB Types (1 if available)
    .output_oob_size   = 0,      // Maximum size of Output OOB supported (0 - device does not support output OOB, 1-8 max size in octets supported by the device)
    .output_oob_action = 0,      // Output OOB Action field values (see @ref BT_MESH_OUT_OOB_ACT "Output OOB Action field values")
    .input_oob_size    = 0,      // Maximum size in octets of Input OOB supported
    .input_oob_action  = 0,      // Supported Input OOB Actions (see @ref BT_MESH_IN_OOB_ACT "Input OOB Action field values")
};

mesh_app_provision_state_t state =
{
    .conn_id = 0,
    .p_parent_callback = NULL,
};

/*
 * Application can call this function to provide its own private key and to register a callback
 * to be executed when OOB information is requested by the provisioner.
 */
void wiced_bt_mesh_app_provision_server_init(uint8_t *p_priv_key, wiced_bt_mesh_provision_server_callback_t *p_callback)
{
    state.p_parent_callback = p_callback;

    // Initialize or reinitialize provisioning layer
    wiced_bt_mesh_core_provision_server_init(p_priv_key, mesh_app_provision_started, mesh_app_provision_end,
        mesh_app_provision_get_capabilities, mesh_app_provision_get_oob, mesh_app_provision_gatt_send_cb,
        mesh_app_provision_get_records, mesh_app_provision_record_request);
}

/*
 * By default application does not expose provisioning public key and does not use any of the
 * provisioning authentication methods. Hopefully a real application will call this function
 * during startup to setup provisioning capabilities.
 */
void wiced_bt_mesh_app_provision_server_configure(wiced_bt_mesh_provision_capabilities_data_t *p_config)
{
    memcpy(&provisioning_config, p_config, sizeof(wiced_bt_mesh_provision_capabilities_data_t));
}


/*
 * This callback is executed by the core library on successful start of provisioning.
 */
void mesh_app_provision_started(uint32_t conn_id)
{
    WICED_BT_TRACE("mesh_app_provision_started: conn_id:%x\n", conn_id);
    if (state.p_parent_callback)
        (*state.p_parent_callback)(WICED_BT_MESH_PROVISION_STARTED, NULL);

}

/*
 * This callback is executed by the core library at the end of the provisioning.
 */
void mesh_app_provision_end(uint32_t conn_id, uint16_t addr, uint16_t net_key_idx, uint8_t result, const uint8_t *p_dev_key)
{
    wiced_bt_mesh_provision_status_data_t data;
    WICED_BT_TRACE("mesh_app_provision_end: conn_id:%x result:%d dev_key:\n", conn_id, result);
//    WICED_BT_TRACE_ARRAY((uint8_t*)p_dev_key, 16, "");

    if (state.p_parent_callback != NULL)
    {
        data.addr        = addr;
        data.net_key_idx = net_key_idx;
        data.result      = result;
        if (p_dev_key != NULL)
        {
            WICED_BT_TRACE_ARRAY((uint8_t*)p_dev_key, 16, "");
            memcpy(data.dev_key, p_dev_key, WICED_BT_MESH_KEY_LEN);
        }
        (*state.p_parent_callback)(WICED_BT_MESH_PROVISION_END, &data);
    }
}

/*
 * This callback is executed by the core library to get OOB data.
 */
wiced_bool_t mesh_app_provision_get_oob(uint32_t conn_id, uint8_t type, uint8_t size, uint8_t action)
{
    wiced_bt_mesh_provision_device_oob_request_data_t data;
    if (state.p_parent_callback != NULL)
    {
        data.type = type;
        data.size = size;
        data.action = action;

        (*state.p_parent_callback)(WICED_BT_MESH_PROVISION_GET_OOB_DATA, &data);
        return WICED_TRUE;
    }

    return WICED_FALSE;
}

/*
* Application can call this function with oob provisioning data.
*/
wiced_bool_t wiced_bt_mesh_provision_set_oob(uint8_t* p_oob, uint8_t len)
{
    return wiced_bt_mesh_core_provision_set_oob(p_oob, len);
}


// Callback function to send provisioning packet.
void mesh_app_provision_gatt_send_cb(uint16_t conn_id, const uint8_t *packet, uint32_t packet_len)
{
    uint8_t * p_data = mesh_app_alloc_buffer(packet_len);
    if(!p_data){
        return;
    }
    memcpy(p_data, packet, packet_len);

    //WICED_BT_TRACE_ARRAY((char*)packet, packet_len, "");
    wiced_bt_gatt_server_send_notification(conn_id, HANDLE_CHAR_MESH_PROVISIONING_DATA_OUT_VALUE, packet_len, p_data, mesh_app_free_buffer);

    WICED_BT_TRACE("mesh_app_provision_gatt_send_cb: conn_id:%x\n", conn_id);
}


// for provisioning app it should set its capabilities.
wiced_bool_t mesh_app_provision_get_capabilities(uint32_t conn_id)
{
    // We use 0 in all members of capabilities (except algorithm) which means no any OOB data
    wiced_bt_mesh_core_provision_capabilities_t capabilities;
    memset(&capabilities, 0, sizeof(capabilities));
    capabilities.elements_num = mesh_config.elements_num;
#ifdef ENHANCED_PROVISIONING_AUTHENTICATION
    UINT16TOBE2(capabilities.algorithms, WICED_BT_MESH_PROVISION_ALG_FIPS_P256_ELLIPTIC_CURVE | WICED_BT_MESH_PROVISION_ALG_ECDH_P256_HMAC_SHA256_AES_CCM);
#else
    UINT16TOBE2(capabilities.algorithms, WICED_BT_MESH_PROVISION_ALG_FIPS_P256_ELLIPTIC_CURVE);
#endif
    capabilities.pub_key_type = provisioning_config.pub_key_type;    // WICED_BT_MESH_PROVISION_CAPS_PUB_KEY_TYPE_AVAILABLE
    capabilities.static_oob_type = provisioning_config.static_oob_type;
    UINT16TOBE2(capabilities.output_oob_action, provisioning_config.output_oob_action);
    capabilities.output_oob_size = provisioning_config.output_oob_size;
    UINT16TOBE2(capabilities.input_oob_action, provisioning_config.input_oob_action);
    capabilities.input_oob_size = provisioning_config.input_oob_size;
    WICED_BT_TRACE("mesh_app_provision_get_capabilities: conn_id:%x\n", conn_id);
    // now we can proceed
    wiced_bt_mesh_core_provision_set_capabilities(conn_id, &capabilities);
    return WICED_TRUE;
}


/**
 * @brief get list of provisioning records
 *
 * @param buffer - pointer to save the list of record ids
 * @return uint16_t - size of the return list of records
 */
uint16_t mesh_app_provision_get_records(uint16_t *buffer)
{
    return wiced_bt_factory_config_provisioning_records_get(buffer);
}


/**
 * @brief read fragment of the provisioning record
 *
 * @param record_id - provisioning record id to read
 * @param buffer - pointer to save fragment of the record
 * @param fragment_length - size of the fragment to read
 * @param fragment_offset - offset of fragment to read from start of the record
 * @param record_size - pointer to the return parameter that returns full record size
 * @return uint16_t - size of the fragment that was actually read into buffer
 */
uint16_t mesh_app_provision_record_request(uint16_t record_id, uint8_t* buffer, uint16_t fragment_length, uint16_t fragment_offset, uint16_t* record_size)
{
    return wiced_bt_factory_config_provisioning_record_req(record_id, buffer, fragment_length, fragment_offset, record_size);
}

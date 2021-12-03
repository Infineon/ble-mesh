/******************************************************************************
* File Name:   mesh_app_provision_server.c
*
* Description:  This file contains functionality required for the device to be
*               provisioned to be a part of the mesh network.
*
*******************************************************************************
* $ Copyright 2021-YEAR Cypress Semiconductor $
*******************************************************************************/

/*******************************************************************************
 * Header file includes
 ******************************************************************************/
#include "bt_types.h"
#include "wiced_bt_ble.h"
#include "wiced_bt_gatt.h"
#include "wiced_bt_cfg.h"
#include "wiced_bt_uuid.h"
#include "wiced_bt_mesh_provision.h"
#include "wiced_bt_mesh_core.h"
#include "cy_retarget_io.h"
#include "bt_app.h"

/*******************************************************************************
 * Macros
 ******************************************************************************/
#define BE2TOUINT16(p) ((uint16_t)(p)[1] + (((uint16_t)(p)[0])<<8))
#define UINT16TOBE2(p, ui) (p)[0]=(uint8_t)((ui)>>8); (p)[1]=(uint8_t)(ui)

/*******************************************************************************
 * Structures
 ******************************************************************************/
typedef struct
{
    uint16_t conn_id;
    wiced_bt_mesh_provision_server_callback_t *p_parent_callback;
} mesh_app_provision_state_t;

/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/
static void            mesh_app_provision_started(uint16_t conn_id);
static void            mesh_app_provision_end(uint16_t conn_id, uint16_t addr, uint16_t net_key_idx, uint8_t result, const uint8_t *p_dev_key);
static wiced_bool_t    mesh_app_provision_get_oob(uint16_t conn_id, uint8_t type, uint8_t size, uint8_t action);
static wiced_bool_t    mesh_app_provision_get_capabilities(uint16_t conn_id);
static void            mesh_app_provision_gatt_send_cb(uint16_t conn_id, const uint8_t *packet, uint32_t packet_len);

/******************************************************
 *          Variables Definitions
 ******************************************************/
wiced_bt_mesh_provision_capabilities_data_t provisioning_config =
{
    .pub_key_type      = 0u,      // If 1 Public Key OOB information available
    .static_oob_type   = 0u,      // Supported static OOB Types (1 if available)
    .output_oob_size   = 0u,      // Maximum size of Output OOB supported (0 - device does not support output OOB, 1-8 max size in octets supported by the device)
    .output_oob_action = 0u,      // Output OOB Action field values (see @ref BT_MESH_OUT_OOB_ACT "Output OOB Action field values")
    .input_oob_size    = 0u,      // Maximum size in octets of Input OOB supported
    .input_oob_action  = 0u,      // Supported Input OOB Actions (see @ref BT_MESH_IN_OOB_ACT "Input OOB Action field values")
};

mesh_app_provision_state_t state =
{
    .conn_id = 0u,
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
        mesh_app_provision_get_capabilities, mesh_app_provision_get_oob, mesh_app_provision_gatt_send_cb);
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
void mesh_app_provision_started(uint16_t conn_id)
{
    printf("mesh_app_provision_started: conn_id:%d\r\n", conn_id);
    if (state.p_parent_callback)
        (*state.p_parent_callback)(WICED_BT_MESH_PROVISION_STARTED, NULL);

}

/*
 * This callback is executed by the core library at the end of the provisioning.
 */
void mesh_app_provision_end(uint16_t conn_id, uint16_t addr, uint16_t net_key_idx, uint8_t result, const uint8_t *p_dev_key)
{
    wiced_bt_mesh_provision_status_data_t data;
    printf("mesh_app_provision_end: conn_id:%d result:%d\r\n", conn_id, result);

    if (state.p_parent_callback != NULL)
    {
        data.addr        = addr;
        data.net_key_idx = net_key_idx;
        data.result      = result;
        if (p_dev_key != NULL)
        {
            memcpy(data.dev_key, p_dev_key, WICED_BT_MESH_KEY_LEN);
        }
        (*state.p_parent_callback)(WICED_BT_MESH_PROVISION_END, &data);
    }
}

/*
 * This callback is executed by the core library to get OOB data.
 */
wiced_bool_t mesh_app_provision_get_oob(uint16_t conn_id, uint8_t type, uint8_t size, uint8_t action)
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
wiced_bool_t mesh_app_provision_set_oob(uint8_t* p_oob, uint8_t len)
{
    return wiced_bt_mesh_core_provision_set_oob(p_oob, len);
}


// Callback function to send provisioning packet.
void mesh_app_provision_gatt_send_cb(uint16_t conn_id, const uint8_t *packet, uint32_t packet_len)
{
    uint8_t * p_data = bt_app_alloc_buffer(packet_len);
    if(!p_data){
        return;
    }
    memcpy(p_data, packet, packet_len);
    printf("mesh_app_provision_gatt_send_cb: conn_id:%d\r\n", conn_id);
    //WICED_BT_TRACE_ARRAY((char*)packet, packet_len, "");
    wiced_bt_gatt_server_send_notification(conn_id, HANDLE_CHAR_MESH_PROVISIONING_DATA_OUT_VALUE, packet_len, p_data, bt_app_free_buffer);

}

// for provisioning app it should set its capabilities.
wiced_bool_t mesh_app_provision_get_capabilities(uint16_t conn_id)
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
    printf("mesh_app_provision_get_capabilities: conn_id:%d\r\n", conn_id);
    // now we can proceed
    wiced_bt_mesh_core_provision_set_capabilities(conn_id, &capabilities);
    return WICED_TRUE;
}

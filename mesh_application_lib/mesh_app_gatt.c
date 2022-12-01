/*
* $ Copyright 2016-YEAR Cypress Semiconductor $
*/

/** @file
 *
 * Mesh GATT related functionality
 *
 */
// Comment out next line to disable proprietary command GATT service
//#define _DEB_COMMAND_SERVICE

#include "wiced_bt_types.h"
#include "wiced_bt_gatt.h"
#include "wiced_bt_ble.h"
#include "wiced_bt_cfg.h"
#include "wiced_memory.h"
#include "bt_gatt_db.h"

#include "wiced_bt_ota_firmware_upgrade.h"
#ifndef MESH_HOST_MODE
#include "wiced_bt_event.h"
#include "wiced_hal_puart.h"
#endif

#include "wiced_bt_stack.h"
#include "wiced_bt_mesh_app.h"
#include "wiced_bt_uuid.h"
#include "wiced_bt_mesh_core.h"
#include "mesh_application.h"
#include "hci_control_api.h"
#include "wiced_bt_trace.h"
#if ( defined(CYW20706A2) || defined(CYW20719B1) || defined(CYW20719B0) || defined(CYW20721B1) || defined(CYW20735B0) || defined(CYW43012C0) )
#include "wiced_bt_app_common.h"
#endif

// If defined then GATT_DB of the unprovisioned device contains OTA FW Upgrade service
#ifndef MESH_HOST_MODE
#define MESH_FW_UPGRADE_UNPROVISIONED
#endif

// Comment out next line if we don't want to support GATT provisioning
#define MESH_SUPPORT_PB_GATT

#define MESH_GATT_MAX_WRITE_SIZE    512

/******************************************************
 *          Function Prototypes
 ******************************************************/
       wiced_bt_gatt_status_t   mesh_gatts_callback(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_data);
static wiced_bt_gatt_status_t   mesh_write_handler(uint16_t conn_id, wiced_bt_gatt_opcode_t opcode, wiced_bt_gatt_write_req_t * p_data);
static wiced_bt_gatt_status_t   mesh_prep_write_handler(uint16_t conn_id, wiced_bt_gatt_opcode_t opcode, wiced_bt_gatt_write_req_t * p_data);
static wiced_bt_gatt_status_t   mesh_write_exec_handler(uint16_t conn_id, wiced_bt_gatt_opcode_t opcode, wiced_bt_gatt_execute_write_req_t *p_data);

extern wiced_bt_cfg_settings_t wiced_bt_cfg_settings;

#if defined REMOTE_PROVISION_SERVER_SUPPORTED && defined REMOTE_PROVISION_OVER_GATT_SUPPORTED
extern wiced_bt_gatt_status_t remote_provision_gatt_client_event(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t* p_data);
#endif

#if defined GATT_PROXY_CLIENT_SUPPORTED
extern wiced_bt_gatt_status_t mesh_gatt_client_event(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t* p_data);
#endif

/******************************************************
 *          Variables Definitions
 ******************************************************/

typedef struct
{
    uint16_t handle;
    uint16_t attr_len;
    void     *p_attr;
} attribute_t;

typedef struct
{
    // GATT Prepare Write command process
    uint8_t                 *p_write_buffer;
    uint16_t                write_length;
    uint16_t                write_handle;
} mesh_gatt_cb_t;

uint8_t mesh_0[20]                  = { 0 };
uint8_t mesh_prov_client_config[2]  = { BIT16_TO_8(GATT_CLIENT_CONFIG_NOTIFICATION) };
uint8_t mesh_proxy_client_config[2] = { 0 };
#ifdef _DEB_COMMAND_SERVICE
uint8_t mesh_cmd_client_config[2]   = { BIT16_TO_8(GATT_CLIENT_CONFIG_NOTIFICATION) };
#endif
uint16_t conn_id = 0;
uint16_t conn_mtu = 0; // MTU to use in wiced_bt_mesh_core_connection_status() at notifications enable
mesh_gatt_cb_t mesh_gatt_cb;

attribute_t gauAttributes[] =
{
    { MESH_HANDLE_GAP_SERVICE_CHARACTERISTIC_DEV_NAME_VAL, 0, NULL },
    { MESH_HANDLE_GAP_SERVICE_CHARACTERISTIC_APPEARANCE_VAL, 0, NULL },
    { HANDLE_CHAR_MESH_PROVISIONING_DATA_IN_VALUE, 20, mesh_0 },
    { HANDLE_DESCR_MESH_PROVISIONING_DATA_CLIENT_CONFIG, sizeof(mesh_prov_client_config), mesh_prov_client_config },
    { HANDLE_CHAR_MESH_PROXY_DATA_IN_VALUE, 20, mesh_0 },
    { HANDLE_DESCR_MESH_PROXY_DATA_CLIENT_CONFIG, sizeof(mesh_proxy_client_config), mesh_proxy_client_config },
    { MESH_HANDLE_DEV_INFO_SERVICE_CHARACTERISTIC_MFR_NAME_VAL, sizeof(mesh_mfr_name), mesh_mfr_name },
    { MESH_HANDLE_DEV_INFO_SERVICE_CHARACTERISTIC_MODEL_NUM_VAL, sizeof(mesh_model_num), mesh_model_num },
    { MESH_HANDLE_DEV_INFO_SERVICE_CHARACTERISTIC_SYSTEM_ID_VAL, sizeof(mesh_system_id), mesh_system_id },
#ifdef _DEB_COMMAND_SERVICE
    { HANDLE_CHAR_MESH_COMMAND_DATA_VALUE, 20, mesh_0 },
    { HANDLE_DESCR_MESH_COMMAND_DATA_CLIENT_CONFIG, sizeof(mesh_cmd_client_config), mesh_cmd_client_config },
#endif
};

/******************************************************
 *               Function Definitions
 ******************************************************/

wiced_bt_gatt_status_t mesh_app_ota_firmware_upgrade_send_data_cb(wiced_bool_t is_notification, uint16_t conn_id,
    uint16_t attr_handle, uint16_t val_len, uint8_t *p_val)
{
    wiced_bt_gatt_status_t status = WICED_BT_GATT_INTERNAL_ERROR;
    uint16_t ret_data_len = 0;

    uint8_t *p_buffer = mesh_app_alloc_buffer(128);
    if (p_buffer == NULL)
    {
        WICED_BT_TRACE("mesh_app_ota_firmware_upgrade_send_data_cb: no_mem len=%d\n", 128);
        return WICED_BT_GATT_NO_RESOURCES;
    }

    // Encrypt outgoing notification/indication data here
    ret_data_len =  wiced_bt_mesh_core_crypt(WICED_TRUE, p_val, val_len, p_buffer, 128);

    WICED_BT_TRACE("mesh_app_ota_firmware_upgrade_send_data_cb: ret_data_len:%d\n", ret_data_len);

    if (ret_data_len)
    {
        if (is_notification)
        {
            status = wiced_bt_gatt_server_send_notification(conn_id, attr_handle, ret_data_len, p_buffer, mesh_app_free_buffer);
        }
        else
        {
            status = wiced_bt_gatt_server_send_indication(conn_id, attr_handle, ret_data_len, p_buffer, mesh_app_free_buffer);
        }
    }

    return status;
}

// This function is executed in the BTM_ENABLED_EVT management callback.
void mesh_app_gatt_init(void)
{
#ifndef MESH_HOMEKIT_COMBO_APP
    /* Register with stack to receive GATT callback */
    wiced_bt_gatt_register(mesh_gatts_callback);
    memset(&mesh_gatt_cb, 0, sizeof(mesh_gatt_cb_t));
#endif // MESH_HOMEKIT_COMBO_APP
}

// setup appropriate GATT DB
void mesh_app_gatt_db_init(wiced_bool_t is_authenticated)
{
    wiced_bt_gatt_status_t gatt_status;
#if !defined (WICEDX_LINUX)

    if (!is_authenticated)
    {
        gatt_status = wiced_bt_gatt_db_init(gatt_db_unprovisioned, gatt_db_unprovisioned_size, NULL);
        WICED_BT_TRACE("mesh_app_gatt_db_init: unprovisioned GATT DB status:%x\n", gatt_status);
    }
    // If device is provisioned we will always show the Proxy service.  If device really
    // does not support Proxy feature it will not send connectable adverts, so it will not
    // cause any problems.  If Proxy feature is supported and turned on, or if device is
    // making itself connectable after provisioning over PB-GATT or when Identification is
    // turned on, the Proxy service is required.  In the latter 2 cases, core will make sure that
    // the device does not relay packets received over GATT.
    else
    {
        gatt_status = wiced_bt_gatt_db_init(gatt_db_provisioned, gatt_db_provisioned_size, NULL);
        WICED_BT_TRACE("mesh_app_gatt_db_init: provisioned GATT DB status:%x\n", gatt_status);
    }
#endif

//    wiced_bt_dev_register_hci_trace(mesh_hci_trace_callback);

    UNUSED_VARIABLE(gatt_status);
}

// Find attribute description by handle
static attribute_t * get_attribute_(uint16_t handle)
{
    int i;
    for (i = 0; i < sizeof(gauAttributes) / sizeof(gauAttributes[0]); i++)
    {
        if (gauAttributes[i].handle == handle)
        {
            return (&gauAttributes[i]);
        }
    }
    WICED_BT_TRACE("get_attribute_: attr not found:%x\n", handle);
    return NULL;
}


attribute_t *mesh_app_get_attribute(uint16_t handle)
{
    attribute_t *puAttribute = get_attribute_(handle);
    if (puAttribute == NULL)
    {
        return NULL;
    }
    if ((handle == MESH_HANDLE_GAP_SERVICE_CHARACTERISTIC_DEV_NAME_VAL) && (puAttribute->attr_len == 0))
    {
        puAttribute->p_attr = wiced_bt_cfg_settings.device_name;
        puAttribute->attr_len = (uint16_t)strlen((char *)wiced_bt_cfg_settings.device_name);
    }
    else if ((handle == MESH_HANDLE_GAP_SERVICE_CHARACTERISTIC_APPEARANCE_VAL) && (puAttribute->attr_len == 0))
    {
        static uint8_t buf[2]; // buffer for appearance

        buf[0] = (uint8_t)wiced_bt_cfg_settings.p_ble_cfg->appearance;
        buf[1] = (uint8_t)(wiced_bt_cfg_settings.p_ble_cfg->appearance >> 8);
        puAttribute->p_attr = buf;
        puAttribute->attr_len = 2;
    }

    return puAttribute;
}

// Process Read request or command from peer device
static wiced_bt_gatt_status_t read_handler_(uint16_t conn_id, wiced_bt_gatt_opcode_t opcode, wiced_bt_gatt_read_t *p_read_req, uint16_t len_requested)
{
    attribute_t *puAttribute;
    int          attr_len_to_copy;

#if !defined (WICEDX_LINUX) && !defined (MESH_HOST_MODE) && OTA_FW_UPGRADE
    // if read request is for the OTA FW upgrade service, pass it to the library to process
    if ((p_read_data->handle > HANDLE_OTA_FW_UPGRADE_SERVICE) && (p_read_data->handle <= HANDLE_OTA_FW_UPGRADE_APP_INFO))
    {
        return wiced_ota_fw_upgrade_read_handler(conn_id, p_read_data);
    }
#endif

    puAttribute = mesh_app_get_attribute(p_read_req->handle);
    if (puAttribute == NULL) {
        wiced_bt_gatt_server_send_error_rsp(conn_id, opcode, p_read_req->handle, WICED_BT_GATT_INVALID_HANDLE);
        return WICED_BT_GATT_INVALID_HANDLE;
    }

    attr_len_to_copy = puAttribute->attr_len;

    //WICED_BT_TRACE("read_handler_: conn_id:%d hdl:%x\n", conn_id, p_read_data->handle);
    //WICED_BT_TRACE(" offset:%d len:%d\n", p_read_data->offset, attr_len_to_copy);

    if (p_read_req->offset > puAttribute->attr_len)
    {
        wiced_bt_gatt_server_send_error_rsp(conn_id, opcode, p_read_req->handle, WICED_BT_GATT_INVALID_OFFSET);
        return WICED_BT_GATT_INVALID_OFFSET;
    }

    attr_len_to_copy = MIN(len_requested, attr_len_to_copy - p_read_req->offset);

    wiced_bt_gatt_server_send_read_handle_rsp(conn_id, opcode, attr_len_to_copy, ((uint8_t *)puAttribute->p_attr) + p_read_req->offset, NULL);

    return WICED_BT_GATT_SUCCESS;
}

/*
 * Process write read-by-type request from peer device
 */
wiced_bt_gatt_status_t read_by_type_handler_(uint16_t conn_id, wiced_bt_gatt_opcode_t opcode, wiced_bt_gatt_read_by_type_t *p_read_req, uint16_t len_requested)
{
    attribute_t *puAttribute;
    uint16_t    attr_handle = p_read_req->s_handle;
    uint8_t *p_rsp = mesh_app_alloc_buffer(len_requested);

    uint8_t    pair_len = 0;
    int used = 0;

    if (p_rsp == NULL)
    {
        WICED_BT_TRACE("[%s]  no memory len_requested: %d!!\n", __FUNCTION__, len_requested);

        wiced_bt_gatt_server_send_error_rsp(conn_id, opcode, attr_handle, WICED_BT_GATT_INSUF_RESOURCE);
        return WICED_BT_GATT_INSUF_RESOURCE;
    }

    /* Read by type returns all attributes of the specified type, between the start and end handles */
    while (WICED_TRUE)
    {
        /// Add your code here
        attr_handle = wiced_bt_gatt_find_handle_by_type(attr_handle, p_read_req->e_handle, &p_read_req->uuid);

        if (attr_handle == 0)
            break;

        puAttribute = mesh_app_get_attribute(attr_handle);
        if (puAttribute == NULL) {
            wiced_bt_gatt_server_send_error_rsp(conn_id, opcode, attr_handle, WICED_BT_GATT_ERR_UNLIKELY);
            return WICED_BT_GATT_ERR_UNLIKELY;
        }

        // --------

        {
            int filled = wiced_bt_gatt_put_read_by_type_rsp_in_stream(p_rsp + used, len_requested - used, &pair_len,
                attr_handle, puAttribute->attr_len, puAttribute->p_attr);
            if (filled == 0) {
                break;
            }
            used += filled;
        }

        /* Increment starting handle for next search to one past current */
        attr_handle++;
    }

    if (used == 0)
    {
        WICED_BT_TRACE("[%s]  attr not found  start_handle: 0x%04x  end_handle: 0x%04x  Type: 0x%04x\n",
            __FUNCTION__, p_read_req->s_handle, p_read_req->e_handle, p_read_req->uuid.uu.uuid16);

        wiced_bt_gatt_server_send_error_rsp(conn_id, opcode, p_read_req->s_handle, WICED_BT_GATT_INVALID_HANDLE);
        mesh_app_free_buffer(p_rsp);
        return WICED_BT_GATT_INVALID_HANDLE;
    }

    /* Send the response */
    wiced_bt_gatt_server_send_read_by_type_rsp(conn_id, opcode, pair_len, used, p_rsp, mesh_app_free_buffer);

    return WICED_BT_GATT_SUCCESS;
}

/*
 * Process write read multi request from peer device
 */
wiced_bt_gatt_status_t read_multi_handler_(uint16_t conn_id, wiced_bt_gatt_opcode_t opcode, wiced_bt_gatt_read_multiple_req_t *p_read_req,
    uint16_t len_requested)
{
    attribute_t *puAttribute;
    uint8_t *p_rsp = mesh_app_alloc_buffer(len_requested);
    int         used = 0;
    int         xx;
    uint16_t handle;

    if (p_rsp == NULL)
    {
        WICED_BT_TRACE("[%s]  no memory len_requested: %d!!\n", __FUNCTION__, len_requested);

        wiced_bt_gatt_server_send_error_rsp(conn_id, opcode, wiced_bt_gatt_get_handle_from_stream(p_read_req->p_handle_stream, 0),
            WICED_BT_GATT_INSUF_RESOURCE);

        return WICED_BT_GATT_INVALID_HANDLE;
    }

    /* Read by type returns all attributes of the specified type, between the start and end handles */
    for (xx = 0; xx < p_read_req->num_handles; xx++)
    {
        handle = wiced_bt_gatt_get_handle_from_stream(p_read_req->p_handle_stream, xx);
        puAttribute = mesh_app_get_attribute(handle);

        if (puAttribute == NULL)
        {
            WICED_BT_TRACE("[%s]  no handle 0x%04x\n", __FUNCTION__, handle);
            wiced_bt_gatt_server_send_error_rsp(conn_id, opcode, handle, WICED_BT_GATT_ERR_UNLIKELY);
            mesh_app_free_buffer(p_rsp);
            return WICED_BT_GATT_INVALID_HANDLE;
        }

        {
            int filled = wiced_bt_gatt_put_read_multi_rsp_in_stream(opcode, p_rsp + used, len_requested - used,
                puAttribute->handle, puAttribute->attr_len, puAttribute->p_attr);
            if (!filled) {
                break;
            }
            used += filled;
        }
    }

    if (used == 0)
    {
        WICED_BT_TRACE("[%s] no attr found\n", __FUNCTION__);

        wiced_bt_gatt_server_send_error_rsp(conn_id, opcode, *p_read_req->p_handle_stream, WICED_BT_GATT_INVALID_HANDLE);
        return WICED_BT_GATT_INVALID_HANDLE;
    }

    /* Send the response */
    wiced_bt_gatt_server_send_read_multiple_rsp(conn_id, opcode, used, p_rsp, mesh_app_free_buffer);

    return WICED_BT_GATT_SUCCESS;
}


/*
* Process indication_confirm from peer device
*/
wiced_bt_gatt_status_t mesh_cfm_handler(uint16_t conn_id, uint16_t handle)
{
#if !defined (WICEDX_LINUX) && !defined (MESH_HOST_MODE) && OTA_FW_UPGRADE
    // if write request is for the OTA FW upgrade service, pass it to the library to process
    if ((handle > HANDLE_OTA_FW_UPGRADE_SERVICE) && (handle <= HANDLE_OTA_FW_UPGRADE_APP_INFO))
    {
        return wiced_ota_fw_upgrade_indication_cfm_handler(conn_id, handle);
    }
#endif
    return WICED_BT_GATT_INVALID_HANDLE;
}

// Process GATT request from the peer
wiced_bt_gatt_status_t mesh_gatts_req_cb(wiced_bt_gatt_attribute_request_t *p_data)
{
    wiced_bt_gatt_status_t result = WICED_BT_GATT_INVALID_PDU;

    // WICED_BT_TRACE("mesh_gatts_req_cb. conn:%d, type:%d\n", p_data->conn_id, p_data->request_type);

    switch (p_data->opcode)
    {
    case GATT_REQ_READ:
    case GATT_REQ_READ_BLOB:
        result = read_handler_(conn_id, p_data->opcode, &p_data->data.read_req, p_data->len_requested);
        break;
    case GATT_REQ_READ_BY_TYPE:
        result = read_by_type_handler_(conn_id, p_data->opcode, &p_data->data.read_by_type, p_data->len_requested);
        break;
    case GATT_REQ_READ_MULTI:
    case GATT_REQ_READ_MULTI_VAR_LENGTH:
        result = read_multi_handler_(conn_id, p_data->opcode, &p_data->data.read_multiple_req, p_data->len_requested);
        break;

    case GATT_REQ_WRITE:
    case GATT_CMD_WRITE:
    case GATT_CMD_SIGNED_WRITE:
        result = mesh_write_handler(p_data->conn_id, p_data->opcode, &p_data->data.write_req);
        break;

    case GATT_REQ_PREPARE_WRITE:
        result = mesh_prep_write_handler(p_data->conn_id, p_data->opcode, &p_data->data.write_req);
        break;

    case GATT_REQ_EXECUTE_WRITE:
        result = mesh_write_exec_handler(p_data->conn_id, p_data->opcode, &p_data->data.exec_write_req);
        break;

    case GATT_REQ_MTU:
        WICED_BT_TRACE("mesh_gatts_req_cb:GATTS_REQ_TYPE_MTU mtu:%x\n", p_data->data.remote_mtu);
        if (p_data->data.remote_mtu < GATT_BLE_DEFAULT_MTU_SIZE) {
            p_data->data.remote_mtu = GATT_BLE_DEFAULT_MTU_SIZE;
        }


        // We will use that MTU in wiced_bt_mesh_core_connection_status() at notifications enable
        conn_mtu = MIN(p_data->data.remote_mtu, wiced_bt_cfg_settings.p_ble_cfg->ble_max_rx_pdu_size);
        wiced_bt_mesh_core_set_gatt_mtu(conn_mtu);
        result = wiced_bt_gatt_server_send_mtu_rsp(conn_id, p_data->data.remote_mtu, conn_mtu);
        break;

    case GATT_HANDLE_VALUE_CONF:
        result = mesh_cfm_handler(p_data->conn_id, p_data->data.confirm.handle);
        break;



    default:
	break;
    }
    return result;
}

/*
* Callback for various GATT events.  As this application performs only as a GATT server, some of the events are ommitted.
*/
wiced_bt_gatt_status_t mesh_gatts_callback(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_data)
{
    wiced_bt_gatt_status_t result = WICED_BT_GATT_INVALID_PDU;

    //WICED_BT_TRACE("mesh_gatts_callback: event:%x p_data:%x\n", event, (uint32_t)p_data);

    switch (event)
    {
    case GATT_CONNECTION_STATUS_EVT:
        WICED_BT_TRACE("mesh gatt connection status: connected:%d node_authenticated:%d conn_id:%x reason:%d role:%d\n", p_data->connection_status.connected, mesh_app_node_is_provisioned(), p_data->connection_status.conn_id, p_data->connection_status.reason, p_data->connection_status.link_role);

        result = WICED_BT_GATT_SUCCESS;

        // If we are in Peripheral role, this is a connection for Proxy or for Provisioning server.
        // Otherwise, we are a provisioner and we established connection to a new device or to a proxy device
        if (p_data->connection_status.link_role == HCI_ROLE_PERIPHERAL)
        {
            if (wiced_bt_mesh_app_func_table.p_mesh_app_gatt_conn_status)
            {
                wiced_bt_mesh_app_func_table.p_mesh_app_gatt_conn_status(&p_data->connection_status);
            }

            if (p_data->connection_status.connected)
            {
                // on connection up return proxy advert interval to 0.5 sec
                // On connection up core stops proxy adverts.On next disconnection it will start adverts with interval 800
                wiced_bt_mesh_core_proxy_adv_interval = 800;

                conn_id = p_data->connection_status.conn_id;
                // When connection is established befor GATTS_REQ_TYPE_MTU, use the default MTU 23 bytes which makes max message length 20
                // Otherwise don't change MTU
                if(conn_mtu == 0)
                    conn_mtu = 20;
                //wiced_bt_l2cap_update_ble_conn_params(p_data->connection_status.bd_addr, 30, 48, 0, 200);
                // We will call mesh_core's wiced_bt_mesh_core_connection_status() on notification enable (write 0x0001 to HANDLE_DESCR_MESH_PROXY_DATA_CLIENT_CONFIG)
            }
            else
            {
                conn_id = 0;
                conn_mtu = 0;
                // On disconnect ref_data is disconnection reason.
                wiced_bt_mesh_core_connection_status(0, WICED_FALSE, p_data->connection_status.reason, 20);
            }
    #if !defined (WICEDX_LINUX) && !defined (MESH_HOST_MODE) && OTA_FW_UPGRADE
            // Pass connection up/down event to the OTA FW upgrade library
            {
                wiced_bt_gatt_connection_status_t connection_status = { 0 };
                connection_status.connected = p_data->connection_status.connected;
                connection_status.conn_id = p_data->connection_status.connected ? p_data->connection_status.conn_id : 0;
                connection_status.bd_addr = p_data->connection_status.bd_addr;
                wiced_ota_fw_upgrade_connection_status_event(&connection_status);
            }
    #endif
        }
        else
        {
#if defined REMOTE_PROVISION_SERVER_SUPPORTED && defined REMOTE_PROVISION_OVER_GATT_SUPPORTED
            remote_provision_gatt_client_event(event, p_data);
#endif
#if defined GATT_PROXY_CLIENT_SUPPORTED
            mesh_gatt_client_event(event, p_data);
#endif
        }
        break;

    case GATT_ATTRIBUTE_REQUEST_EVT:
        result = mesh_gatts_req_cb(&p_data->attribute_request);
        break;

    case GATT_GET_RESPONSE_BUFFER_EVT:
    {
        wiced_bt_gatt_buffer_request_t *p_buf_req = &p_data->buffer_request;

        p_buf_req->buffer.p_app_ctxt = mesh_app_free_buffer;
        p_buf_req->buffer.p_app_rsp_buffer = mesh_app_alloc_buffer(p_buf_req->len_requested);
        result = WICED_BT_GATT_SUCCESS;
        WICED_BT_TRACE("[%s] get buffer of len %d alloced %x ", __FUNCTION__,
            p_buf_req->len_requested, p_buf_req->buffer.p_app_rsp_buffer);
    }break;
    case GATT_APP_BUFFER_TRANSMITTED_EVT:
    {
        mesh_app_free_t pfn_free = (mesh_app_free_t)p_data->buffer_xmitted.p_app_ctxt;

        if (pfn_free)
            pfn_free(p_data->buffer_xmitted.p_app_data);
        result = WICED_BT_GATT_SUCCESS;
    }break;

    default:
#if defined REMOTE_PROVISION_SERVER_SUPPORTED && defined REMOTE_PROVISION_OVER_GATT_SUPPORTED
        remote_provision_gatt_client_event(event, p_data);
#endif
#if defined GATT_PROXY_CLIENT_SUPPORTED
        mesh_gatt_client_event(event, p_data);
#endif
	break;
    }
    return result;
}

wiced_bool_t mesh_app_gatt_is_connected(void)
{
    return conn_id != 0;
}
// Handle a command packet received from the BLE host.
static wiced_bt_gatt_status_t mesh_write_handler(uint16_t conn_id, wiced_bt_gatt_opcode_t opcode, wiced_bt_gatt_write_req_t * p_data)
{
    wiced_bt_gatt_status_t result    = WICED_BT_GATT_SUCCESS;
    uint8_t                *attr     = p_data->p_val;
    uint16_t                len      = p_data->val_len;
    attribute_t            *pAttr;
    //uint8_t                *p_buffer;

    //WICED_BT_TRACE("write_handler: conn_id:%x handle=%x len=%d\n", conn_id, p_data->handle, len);
    //WICED_BT_TRACE_ARRAY((char*)attr, len, "");
#if !defined (WICEDX_LINUX) && !defined (MESH_HOST_MODE) && OTA_FW_UPGRADE

    // if write request is for the OTA FW upgrade service, pass it to the library to process
    if ((p_data->handle > HANDLE_OTA_FW_UPGRADE_SERVICE) && (p_data->handle <= HANDLE_OTA_FW_UPGRADE_APP_INFO))
    {
        if ((p_data->handle == HANDLE_OTA_FW_UPGRADE_CONTROL_POINT) || (p_data->handle ==  HANDLE_OTA_FW_UPGRADE_DATA))
        {
            p_buffer = (uint8_t *)wiced_bt_get_buffer(p_data->val_len);
            if (p_buffer == NULL)
            {
                WICED_BT_TRACE("write_handler: no_mem len=%d\n", p_data->val_len);
                return WICED_BT_GATT_NO_RESOURCES;
            }
            // Decrypt data to use
            p_data->val_len = wiced_bt_mesh_core_crypt(WICED_FALSE, p_data->p_val, p_data->val_len, p_buffer, p_data->val_len);
            p_data->p_val = p_buffer;

            result = wiced_ota_fw_upgrade_write_handler(conn_id, p_data);
            wiced_bt_free_buffer(p_buffer);
        }
        else
        {
            result = wiced_ota_fw_upgrade_write_handler(conn_id, p_data);
        }
    }
    else
#endif
    {
        switch (p_data->handle)
        {
#ifdef _DEB_COMMAND_SERVICE
        case HANDLE_CHAR_MESH_COMMAND_DATA_VALUE:
            if (!mesh_app_node_is_provisioned())
            {
                result = WICED_BT_GATT_WRONG_STATE;
                WICED_BT_TRACE("write_handler: mesh isn't started yet. ignoring...\n");
                break;
            }
            switch (*attr)
            {
            case MESH_COMMAND_RESET_NODE:
                if (len != 1)
                {
                    result = WICED_BT_GATT_INVALID_ATTR_LEN;
                    break;
                }
                WICED_BT_TRACE("write_handler: RESET_NODE\n");
                wiced_bt_mesh_core_init(NULL);
                extern wiced_bool_t node_authenticated;
                node_authenticated = WICED_FALSE;
                break;
            }
            break;
#endif

        case HANDLE_CHAR_MESH_PROVISIONING_DATA_IN_VALUE:
            if (mesh_app_node_is_provisioned())
            {
                result = WICED_BT_GATT_WRONG_STATE;
                WICED_BT_TRACE("write_handler: mesh is started already. ignoring...\n");
                break;
            }
            // Pass received packet to pb_gat transport layer
            wiced_bt_mesh_core_provision_gatt_packet(0, conn_id, attr, (uint8_t)len);
            break;

        case HANDLE_CHAR_MESH_PROXY_DATA_IN_VALUE:
            // handle it the same way as mesh packet received via advert report
            if (!mesh_app_node_is_provisioned())
            {
                result = WICED_BT_GATT_WRONG_STATE;
                WICED_BT_TRACE("write_handler: mesh isn't started. ignoring...\n");
                break;
            }
            wiced_bt_mesh_core_proxy_packet(attr, (uint8_t)len);
            break;

        case HANDLE_DESCR_MESH_PROVISIONING_DATA_CLIENT_CONFIG:
        case HANDLE_DESCR_MESH_PROXY_DATA_CLIENT_CONFIG:
#ifdef _DEB_COMMAND_SERVICE
        case HANDLE_DESCR_MESH_COMMAND_DATA_CLIENT_CONFIG:
#endif
            pAttr = get_attribute_(p_data->handle);
            if (pAttr)
            {
                memset(pAttr->p_attr, 0, pAttr->attr_len);
                memcpy(pAttr->p_attr, attr, len <= pAttr->attr_len ? len : pAttr->attr_len);
            }

#ifdef _DEB_COMMAND_SERVICE
            if (p_data->handle != HANDLE_DESCR_MESH_PROXY_DATA_CLIENT_CONFIG && p_data->handle != HANDLE_DESCR_MESH_PROVISIONING_DATA_CLIENT_CONFIG)
                break;
#endif
            // Use conn_mtu value to call wiced_bt_mesh_core_connection_status() just once - on first write to HANDLE_DESCR_MESH_PROXY_DATA_CLIENT_CONFIG
            if (conn_mtu != 0)
            {
                wiced_bt_mesh_core_connection_status(conn_id, WICED_FALSE, 0, conn_mtu);
                conn_mtu = 0;
            }

            break;

        default:
            WICED_BT_TRACE("write_handler: Skip the packet...\n");
            result = WICED_BT_GATT_INVALID_HANDLE;
            break;
        }
    }

    if (!opcode) {
        /** this has been called on execute write */
        return result;
    }

    if (result == WICED_BT_GATT_SUCCESS) {
        wiced_bt_gatt_server_send_write_rsp(conn_id, opcode, p_data->handle);
    }
    else {
        wiced_bt_gatt_server_send_error_rsp(conn_id, opcode, p_data->handle, result);
    }

    // WICED_BT_TRACE("write_handler: returns %d handle:%x\n", result, p_data->handle);
    return result;
}

// Handle GATT Prepare Write command
static wiced_bt_gatt_status_t mesh_prep_write_handler(uint16_t conn_id, wiced_bt_gatt_opcode_t opcode, wiced_bt_gatt_write_req_t * p_write_req)
{
    wiced_bt_gatt_status_t result = WICED_BT_GATT_SUCCESS;

    if (!mesh_gatt_cb.p_write_buffer)
    {
        mesh_gatt_cb.p_write_buffer = mesh_app_alloc_buffer(MESH_GATT_MAX_WRITE_SIZE);
        if (!mesh_gatt_cb.p_write_buffer) {
            result = WICED_BT_GATT_PREPARE_Q_FULL;
            wiced_bt_gatt_server_send_error_rsp(conn_id, opcode, p_write_req->handle, result);
            return result;
        }
    }

    if (p_write_req->offset + p_write_req->val_len <= MESH_GATT_MAX_WRITE_SIZE)
    {
        mesh_gatt_cb.write_handle = p_write_req->handle;
        memcpy(mesh_gatt_cb.p_write_buffer + p_write_req->offset, p_write_req->p_val, p_write_req->val_len);
        mesh_gatt_cb.write_length = p_write_req->offset + p_write_req->val_len;
    }
    else
    {
        result = WICED_BT_GATT_INSUF_RESOURCE;
    }

    if (result == WICED_BT_GATT_SUCCESS) {
        /** send a null context here, since the p_write_buffer is to be freed on exe write, or disconnect */
        wiced_bt_gatt_server_send_prepare_write_rsp(conn_id, opcode, p_write_req->handle, p_write_req->offset, p_write_req->val_len,
            mesh_gatt_cb.p_write_buffer + p_write_req->offset, NULL);
    }
    else {
        wiced_bt_gatt_server_send_error_rsp(conn_id, opcode, p_write_req->handle, result);
    }

    return result;
}

// Handle GATT Write Execute command
static wiced_bt_gatt_status_t mesh_write_exec_handler(uint16_t conn_id, wiced_bt_gatt_opcode_t opcode, wiced_bt_gatt_execute_write_req_t *p_req)
{
    wiced_bt_gatt_status_t result = WICED_BT_GATT_SUCCESS;
    wiced_bt_gatt_write_req_t write_req;

    if (p_req->exec_write == GATT_PREPARE_WRITE_CANCEL) {
        mesh_app_free_buffer(mesh_gatt_cb.p_write_buffer);
        mesh_gatt_cb.p_write_buffer = NULL;

        wiced_bt_gatt_server_send_execute_write_rsp(conn_id, opcode);

        return result;
    }

    if (mesh_gatt_cb.p_write_buffer)
    {


        memset(&write_req, 0, sizeof(wiced_bt_gatt_write_req_t));
        write_req.handle  = mesh_gatt_cb.write_handle;
        write_req.p_val   = mesh_gatt_cb.p_write_buffer;
        write_req.val_len = mesh_gatt_cb.write_length;

        result = mesh_write_handler(conn_id, 0, &write_req);

        mesh_app_free_buffer(mesh_gatt_cb.p_write_buffer);
        mesh_gatt_cb.p_write_buffer = NULL;
    }
    else
    {
        result = WICED_BT_GATT_INSUF_RESOURCE;
    }

    if (result == WICED_BT_GATT_SUCCESS) {
        wiced_bt_gatt_server_send_execute_write_rsp(conn_id, opcode);
    }
    else {
        wiced_bt_gatt_server_send_error_rsp(conn_id, opcode, write_req.handle, result);
    }

    return result;
}

// Callback function to send a packet over GATT proxy connection.  If gatt_char_handle parameter is not
// zero, this device performs as a GATT client and should use GATT Write Command to send the packet out.
// Otherwise, the device is a GATT server and should use GATT Notification
void mesh_app_proxy_gatt_send_cb(uint32_t conn_id, uint32_t ref_data, const uint8_t *packet, uint32_t packet_len)
{
    //uint16_t  gatt_char_handle = (uint16_t)ref_data;
    uint8_t  *p_data = mesh_app_alloc_buffer(packet_len);

    if(p_data){
        memcpy(p_data, packet, packet_len);
    }
    else{
        WICED_BT_TRACE_CRIT("proxy_gatt_send_cb: no mem to write %d", packet_len);
        return;
    }

    //WICED_BT_TRACE("proxy_gatt_send_cb: conn_id:%x packet: \n", conn_id);
    //WICED_BT_TRACE_ARRAY((char*)packet, packet_len, "");
    if (ref_data)
    {
        wiced_bt_gatt_write_hdr_t value;

        value.handle = (uint16_t)ref_data;
        value.offset = 0;
        value.len = packet_len;
        value.auth_req = 0;

        wiced_bt_gatt_client_send_write(conn_id, GATT_CMD_WRITE, &value, p_data, mesh_app_free_buffer);
    }
    else if (mesh_proxy_client_config[0] & GATT_CLIENT_CONFIG_NOTIFICATION)
    {
        wiced_bt_gatt_server_send_notification(conn_id, HANDLE_CHAR_MESH_PROXY_DATA_OUT_VALUE, packet_len, p_data, mesh_app_free_buffer);
        // WICED_BT_TRACE("proxy_gatt_send_cb: conn_id:%x res:%d\n", conn_id, res);
    }
    else
    {
        if (p_data) {
            mesh_app_free_buffer(p_data);
        }

#ifndef MESH_APPLICATION_MCU_MEMORY
        WICED_BT_TRACE("proxy_gatt_send_cb: drop notification\n");
#else
        WICED_BT_TRACE("Proxy Data to MCU len:%d\n", packet_len);
        mesh_application_send_hci_event(HCI_CONTROL_MESH_EVENT_PROXY_DATA, packet, packet_len);
#endif
    }
}


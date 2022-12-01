/*
* $ Copyright 2016-YEAR Cypress Semiconductor $
*/

/** @file
 *
 * This file contains functions which embeddded application can use to send and
 * receive data from the host.
 */

#undef WIN32

#include "wiced_bt_cfg.h"
#include "wiced_memory.h"
#include "wiced_timer.h"
//#include "wiced_platform.h"
//#include "wiced_transport.h"
#include "wiced_bt_mesh_app.h"

#include "wiced_bt_mesh_app.h"
#include "wiced_bt_mesh_models.h"
#include "wiced_bt_mesh_core.h"
#include "wiced_bt_trace.h"
#include "mesh_application.h"
#include "hci_control_api.h"
#ifndef MESH_HOST_MODE
#include "wiced_hal_puart.h"
#include "wiced_hal_wdog.h"
#endif
#ifdef CYW20706A2
#include "wiced_bt_hci_defs.h"
#else
#include "hcidefs.h"
#endif
#ifdef DIRECTED_FORWARDING_SERVER_SUPPORTED
#if defined HCI_CONTROL
#include "wiced_bt_mesh_mdf.h"
#endif
#endif

#include "cybt_result.h"
#include "cyhal_gpio.h"
#include "cybt_debug_uart.h"


// Enables HCI trace to first(same as app download) com port.
//#define _DEB_ENABLE_HCI_TRACE
#define _DEB_ENABLE_HCI_TRACE_NO_BLE_ADV_EVT

void  mesh_hci_trace_cback(wiced_bt_hci_trace_type_t type, uint16_t length, uint8_t* p_data);
#ifdef DIRECTED_FORWARDING_SERVER_SUPPORTED
#if defined HCI_CONTROL
void mesh_df_stats_hci_event_send(void);
#endif
#endif

/******************************************************
 *          Function Prototypes
 ******************************************************/

/******************************************************
 *          Variables Definitions
 ******************************************************/
uint32_t mesh_application_proc_rx_cmd(uint8_t *p_buffer, uint32_t length);

#ifndef MESH_HOST_MODE
/* transport configuration */
const wiced_transport_cfg_t  transport_cfg =
{
    .type = WICED_TRANSPORT_UART,
#ifdef HCI_UART_BAUDRATE
    .cfg.uart_cfg = {.mode = WICED_TRANSPORT_UART_HCI_MODE, .baud_rate = HCI_UART_BAUDRATE },
#else
    .cfg.uart_cfg = {.mode = WICED_TRANSPORT_UART_HCI_MODE, .baud_rate = HCI_UART_DEFAULT_BAUD },   // default it 3mbps
#endif
    .rx_buff_pool_cfg = {.buffer_size = 1024, .buffer_count = 2 },
    .p_status_handler = NULL,
    .p_data_handler = mesh_application_proc_rx_cmd,
    .p_tx_complete_cback = NULL
};

wiced_transport_buffer_pool_t* host_trans_pool = NULL;

/******************************************************
 *               Function Definitions
 ******************************************************/
void mesh_app_hci_init(void)
{
#ifndef MESH_HOMEKIT_COMBO_APP
    wiced_transport_init(&transport_cfg);

    // create special pool for sending data to the MCU
    host_trans_pool = wiced_transport_create_buffer_pool(1024, 2);
#endif

#ifndef WICEDX_LINUX
#ifdef WICED_BT_TRACE_ENABLE
#ifndef _DEB_ENABLE_HCI_TRACE
    //For the 24 MHz board set the UART type as WICED_ROUTE_DEBUG_TO_PUART
    // For BCM920706V2_EVAL board make sure SW5.2 and SW5.4 are ON and all other SW5 are off
    wiced_set_debug_uart(WICED_ROUTE_DEBUG_TO_PUART);
#if (defined(CYW20706A2) || defined(CYW20735B0) || defined(CYW20719B0) || defined(CYW43012C0))
    wiced_hal_puart_select_uart_pads(WICED_PUART_RXD, WICED_PUART_TXD, 0, 0);
#endif
    wiced_hal_puart_configuration(921600, PARITY_NONE, STOP_BIT_1);
#else // _DEB_ENABLE_HCI_TRACE
    // WICED_ROUTE_DEBUG_TO_WICED_UART to send debug strings over the WICED debug interface
    wiced_set_debug_uart(WICED_ROUTE_DEBUG_TO_WICED_UART);
#endif // _DEB_ENABLE_HCI_TRACE
#endif // WICED_BT_TRACE_ENABLE
#endif // WICEDX_LINUX
//    wiced_bt_dev_register_hci_trace(mesh_hci_trace_cback);
}

#else // MESH_HOST_MODE

#ifdef WIN32
static SOCKET  m_ListenSocket = INVALID_SOCKET;
static SOCKET  m_ClientSocket = INVALID_SOCKET;

static int readHostTCPpkt(unsigned char* pPkt);
static DWORD __stdcall RpcReceiveThread(VOID* p);
#endif

#define MPAF_TRAN_PKT_TYPE  25

extern int wicedx_emulator_instance;

#endif  // MESH_HOST_MODE

wiced_bt_mesh_hci_event_t *wiced_bt_mesh_create_hci_event(wiced_bt_mesh_event_t *p_event)
{
#ifdef NEW_WICED_STACK
    wiced_bt_mesh_hci_event_t *p_hci_event = (wiced_bt_mesh_hci_event_t *)wiced_bt_get_buffer(1000);
#else
    wiced_bt_mesh_hci_event_t* p_hci_event = (wiced_bt_mesh_hci_event_t*)wiced_transport_allocate_buffer(host_trans_pool);
#endif

    if (p_hci_event == NULL)
    {
        WICED_BT_TRACE("no trans buffer\n");
    }
    else if (p_event != NULL)
    {
        p_hci_event->src         = p_event->src;
        p_hci_event->dst         = p_event->dst;
        p_hci_event->app_key_idx = p_event->app_key_idx;
        p_hci_event->element_idx = p_event->element_idx;
        p_hci_event->rssi        = p_event->rssi;
        p_hci_event->ttl         = p_event->ttl;
        p_hci_event->company_id  = p_event->company_id;
        p_hci_event->opcode      = p_event->opcode;
    }
    else
    {
        memset(p_hci_event, 0, sizeof(wiced_bt_mesh_hci_event_t));
    }
    return p_hci_event;
}

wiced_bt_mesh_hci_event_t *wiced_bt_mesh_alloc_hci_event(uint8_t element_idx)
{
#ifdef NEW_WICED_STACK
    wiced_bt_mesh_hci_event_t *p_hci_event = (wiced_bt_mesh_hci_event_t *)wiced_bt_get_buffer(1000);
#else
    wiced_bt_mesh_hci_event_t* p_hci_event = (wiced_bt_mesh_hci_event_t*)wiced_transport_allocate_buffer(host_trans_pool);
#endif

    if (p_hci_event == NULL)
    {
        WICED_BT_TRACE("no trans buffer\n");
    }
    else
    {
        p_hci_event->src         = 0;
        p_hci_event->app_key_idx = 0;
        p_hci_event->element_idx = element_idx;
    }
    return p_hci_event;
}

wiced_result_t mesh_transport_send_data(uint16_t opcode, uint8_t *p_trans_buf, uint16_t data_len)
{
#ifndef MESH_HOST_MODE
    wiced_result_t result = wiced_transport_send_buffer(opcode, p_trans_buf, data_len);
    if (result != WICED_BT_SUCCESS)
    {
        WICED_BT_TRACE("transport send buffer:%d\n", result);
#if !defined(CYW20706A2)
        wiced_transport_free_buffer(p_trans_buf);
#endif
    }
    return result;
#else
#ifdef WIN32
    uint8_t     buf[1500];

    buf[0] = MPAF_TRAN_PKT_TYPE;
    buf[1] = (uint8_t)opcode;
    buf[2] = (uint8_t)(opcode >> 8);
    buf[3] = (uint8_t)data_len;
    buf[4] = (uint8_t)(data_len >> 8);

    // copy received data to transport buffer
    memcpy(&buf[5], p_trans_buf, data_len);

    wiced_bt_free_buffer (p_trans_buf);

    if (m_ClientSocket == INVALID_SOCKET)
    {
        WICED_BT_TRACE_CRIT("!!!!  hci_control_send_script_event() - no TCP socket - dropping data !!!");
        return (WICED_BT_ERROR);
    }

    send(m_ClientSocket, buf, data_len + 5, 0);
    return (WICED_BT_SUCCESS);
#endif
#endif

if (cybt_debug_uart_send_data((uint16_t)opcode, data_len, p_trans_buf) == CYBT_SUCCESS)
        return WICED_SUCCESS;

    return (WICED_BT_SUCCESS);

}

//  Pass protocol traces up through the UART
void mesh_hci_trace_cback(wiced_bt_hci_trace_type_t type, uint16_t length, uint8_t* p_data)
{
#ifndef MESH_HOST_MODE
    // handle HCI_HARDWARE_ERROR_EVT - print trace and reboot.
    if (type == HCI_TRACE_EVENT && p_data[0] == HCI_HARDWARE_ERROR_EVT && p_data[2] != 0)
    {
        // allow retry for HARDWARE_CODE_VS_UART_PARSING_ERROR (p_data[2] == 0)
        WICED_BT_TRACE("Rebooting on HCI_HARDWARE_ERROR_EVT(%x)...\n", p_data[2]);
        wiced_hal_wdog_reset_system();
    }
#ifdef _DEB_ENABLE_HCI_TRACE
#ifdef _DEB_ENABLE_HCI_TRACE_NO_BLE_ADV_EVT
    // filter out HCI_BLE_Advertising_Report_Event
    if (type == 0               // if HCI event
        && p_data[0] == 0x3e    // and HCI_BLE_Event
        && p_data[2] == 2)      // and HCI_BLE_Advertising_Report_Event
        return;
#endif
    wiced_transport_send_hci_trace(NULL, type, length, p_data);
//#else
//    if ((type == HCI_TRACE_COMMAND) || (type == HCI_TRACE_EVENT))
//    {
//        WICED_BT_TRACE("hci_trace_cback: type:%x length:%d\n", type, length);
//        WICED_BT_TRACE_ARRAY((char*)p_data, length, "");
//    }
#endif

#else

//    cybt_debug_uart_send_hci_trace(type, length, p_data);

#endif  // MESH_HOST_MODE
}

extern void wiced_transport_free_buffer(void* p);

uint32_t mesh_application_proc_rx_cmd(uint8_t *p_buffer, uint32_t length)
{
    uint16_t opcode;
    uint16_t payload_len;
    uint8_t *p_data = p_buffer;
#ifndef NEW_DYNAMIC_MEMORY_INCLUDED
    uint8_t  buffer_processed = WICED_TRUE;
#endif

    if (p_buffer == NULL)
    {
        WICED_BT_TRACE("NULL pbuffer\n");
        return 0;
    }

    // Expected minimum 4 byte as the wiced header
    if (length < 4)
    {
        WICED_BT_TRACE("invalid params\n");
    }
    else
    {
        STREAM_TO_UINT16(opcode, p_data);       // Get OpCode
        STREAM_TO_UINT16(payload_len, p_data);  // Gen Payload Length
#if defined MESH_APPLICATION_MCU_MEMORY || defined PTS
        if (!mesh_application_process_hci_cmd(opcode, p_data, payload_len))
#endif
        {
            WICED_BT_TRACE("***************opcode:  %x\n", opcode);
            WICED_BT_TRACE("***************len:     %d\n", payload_len);
            WICED_BT_TRACE_ARRAY(p_data, length - 4, "***************payload: ");
#ifdef PTS
            if (opcode == 0xFFFF)
            {
                WICED_BT_TRACE("***************Reinitialized(Unprovision)\n");
                mesh_application_factory_reset();
            } else if (opcode == 0xFFFE)
            {
                WICED_BT_TRACE("***************Exit command\n");
            } else if (opcode == 0xFFFD)
            {
                WICED_BT_TRACE("***************Reset system\n");
            /* Reboot the system */
                wiced_hal_wdog_reset_system();
            }
            else if (opcode == HCI_CONTROL_COMMAND_RESET)
            {
                wiced_hal_wdog_reset_system();
            }
            else if (!wiced_bt_mesh_core_test_mode_signal(opcode, p_data, payload_len))
            {
#else
            if ((opcode == HCI_CONTROL_MESH_COMMAND_CORE_SET_SEQ) ||
                (opcode == HCI_CONTROL_MESH_COMMAND_CORE_DEL_SEQ))
            {
                wiced_bt_mesh_core_proc_rx_cmd(opcode, p_data, payload_len);
            }
            else if (opcode == HCI_CONTROL_MESH_COMMAND_TRACE_CORE_SET)
            {
                wiced_bt_mesh_core_set_trace_level(p_data[1] + (p_data[2] << 8) + (p_data[3] << 16) + (p_data[4] << 24), p_data[0]);
            }
            else if (opcode == HCI_CONTROL_MESH_COMMAND_TRACE_MODELS_SET)
            {
                wiced_bt_mesh_models_set_trace_level(p_data[0]);
            }
            else
            {
#endif
            if (opcode == HCI_CONTROL_MESH_COMMAND_HARD_RESET)
            {
                mesh_application_hard_reset(p_data, (uint8_t)payload_len);
            }
            else
#ifdef DIRECTED_FORWARDING_SERVER_SUPPORTED
#if defined HCI_CONTROL
            if (opcode == HCI_CONTROL_MESH_COMMAND_DF_STATS_GET)
                mesh_df_stats_hci_event_send();
            else
#endif
#endif
            {
                if (wiced_bt_mesh_app_func_table.p_mesh_app_proc_rx_cmd)
                {
                    wiced_bt_mesh_app_func_table.p_mesh_app_proc_rx_cmd(opcode, p_data, payload_len);
                }
            }
        }
    }
    }
    //wiced_transport_free_buffer(p_buffer);


#ifndef NEW_DYNAMIC_MEMORY_INCLUDED
    if (buffer_processed)
    {
        // Freeing the buffer in which data is received
    wiced_transport_free_buffer(p_buffer);
    }
#endif
    return 0;
}

/*
 * This function can be used to process WICED HCI header common all WICED HCI commands and create corresponding mesh event
 */
wiced_bt_mesh_event_t *wiced_bt_mesh_create_event_from_wiced_hci(uint16_t opcode, uint16_t company_id, uint16_t model_id, uint8_t **p_data, uint32_t *len)
{
    uint16_t dst;
    uint16_t app_key_idx;
    uint8_t  element_idx, ttl;
    uint8_t  *p = *p_data;
    wiced_bt_mesh_event_t *p_event;
    uint8_t flags;

    STREAM_TO_UINT16(dst, p);
    STREAM_TO_UINT16(app_key_idx, p);
    STREAM_TO_UINT8(element_idx, p);

    *len = *len - 5;

    p_event = wiced_bt_mesh_create_event(element_idx, company_id, model_id, dst, app_key_idx);
    if (p_event != NULL)
    {
        p_event->hci_opcode = opcode;

        STREAM_TO_UINT8(p_event->reply, p);
        STREAM_TO_UINT8(flags, p);
        p_event->send_segmented = flags & WICED_BT_MESH_HCI_COMMAND_FLAGS_SEND_SEGMENTED;
        if (flags & WICED_BT_MESH_HCI_COMMAND_FLAGS_TAG_USE_DIRECTED)
            p_event->flags |= TAG_USE_DIRECTED;
        if (flags & WICED_BT_MESH_HCI_COMMAND_FLAGS_TAG_IMMUTABLE_CREDENTIALS)
            p_event->flags |= TAG_IMMUTABLE_CREDENTIALS;

        STREAM_TO_UINT8(ttl, p);
        if(ttl <= 0x7F)
            p_event->ttl = ttl;

        STREAM_TO_UINT8(p_event->retrans_cnt, p);
        STREAM_TO_UINT8(p_event->retrans_time, p);
        STREAM_TO_UINT8(p_event->reply_timeout, p);

        *len = *len - 6;
    }
    *p_data = p;
    return p_event;
}

/*
 * This function can be used to skip mesh WICED HCI header
 */
void wiced_bt_mesh_skip_wiced_hci_hdr(uint8_t **p_data, uint32_t *len)
{
    // skip dst (2), app_key_idx(2), element_idx(1), reply(1), flags(1), ttl(1), retrans_cnt(1), retrans_time(1), reply_timeout(1)
    *len = *len - 11;
    *p_data = (*p_data + 11);
}

/*
 * This function can be used to parse WICED HCI header and extract needed info
 */
uint8_t wiced_bt_mesh_get_element_idx_from_wiced_hci(uint8_t **p_data, uint32_t *len)
{
    uint8_t  element_idx;
    uint8_t  *p = *p_data;

    p += 4;  // skip dst(2) and app_key_idx(2)
    STREAM_TO_UINT8(element_idx, p);
    p += 6;             // reply(1), flags(1), ttl(1), retrans_cnt(1), retrans_time(1), reply_timeout(1)
    *len = *len - 11;
    *p_data = p;
    return element_idx;
}

/*
 * This function can be used to send TX Complete event to the MCU
 */
void wiced_bt_mesh_send_hci_tx_complete(wiced_bt_mesh_hci_event_t *p_hci_event, wiced_bt_mesh_event_t *p_event)
{
    uint8_t *p = p_hci_event->data;

    UINT16_TO_STREAM(p, p_event->hci_opcode);
    UINT8_TO_STREAM(p, p_event->status.tx_flag);
    UINT16_TO_STREAM(p, p_event->dst);
    mesh_transport_send_data(HCI_CONTROL_MESH_EVENT_TX_COMPLETE, (uint8_t *)p_hci_event, (uint16_t)(p - (uint8_t *)p_hci_event));
}

void mesh_app_hci_send_seq_changed(wiced_bt_mesh_core_state_seq_t *p_seq)
{
    uint8_t *p_hci_event;
    uint8_t *p;

#ifdef NEW_WICED_STACK
    p_hci_event = (uint8_t *)wiced_bt_get_buffer (32);
#else
    p_hci_event = wiced_transport_allocate_buffer(host_trans_pool);
#endif

    if (p_hci_event != NULL)
    {
        p = p_hci_event;
        UINT16_TO_STREAM(p, p_seq->addr);
        UINT32_TO_STREAM(p, p_seq->seq);
        UINT8_TO_STREAM(p, p_seq->previous_iv_idx);
        UINT16_TO_STREAM(p, p_seq->rpl_entry_idx);
        mesh_transport_send_data(HCI_CONTROL_MESH_EVENT_CORE_SEQ_CHANGED, p_hci_event, (uint16_t)(p - p_hci_event));
    }
}

#ifndef MESH_HOST_MODE
#ifdef PTS
void mesh_app_hci_send_private_beacon(wiced_bt_mesh_core_state_beacon_t *p_beacon)
{
    uint8_t *p_hci_event;
    uint8_t *p;
    p_hci_event = wiced_transport_allocate_buffer(host_trans_pool);
    if (p_hci_event != NULL)
    {
        p = p_hci_event;
        UINT8_TO_STREAM(p, p_beacon->flags);
        memcpy(p, p_beacon->iv_index, 4);
        p += 4;
        memcpy(p, p_beacon->random, 13);
        p += 13;
        if (p_beacon->bd_addr)
            memcpy(p, p_beacon->bd_addr, sizeof(wiced_bt_device_address_t));
        else
            memset(p, 0, sizeof(wiced_bt_device_address_t));
        p += sizeof(wiced_bt_device_address_t);
        mesh_transport_send_data(HCI_CONTROL_MESH_EVENT_PRIVATE_BEACON, p_hci_event, (uint16_t)(p - p_hci_event));
    }
}

void mesh_app_hci_send_proxy_service_adv(wiced_bt_mesh_core_state_proxy_serivce_t *p_service)
{
    uint8_t *p_hci_event;
    uint8_t *p;
    p_hci_event = wiced_transport_allocate_buffer(host_trans_pool);
    if (p_hci_event != NULL)
    {
        p = p_hci_event;
        UINT8_TO_STREAM(p, p_service->type);
        UINT16_TO_STREAM(p, p_service->net_key_idx);
        mesh_transport_send_data(HCI_CONTROL_MESH_EVENT_PROXY_SERVICE_ADV, p_hci_event, (uint16_t)(p - p_hci_event));
    }
}
#endif
#endif

wiced_result_t mesh_application_send_hci_event(uint16_t opcode, const uint8_t *p_data, uint16_t data_len)
{
#ifdef NEW_WICED_STACK
    uint8_t      *p_hci_event = wiced_bt_get_buffer (data_len + 16);
#else
    uint8_t     *p_hci_event = wiced_transport_allocate_buffer(host_trans_pool);
#endif

    if (p_hci_event != NULL)
    {
        if (data_len != 0)
            memcpy(p_hci_event, p_data, data_len);
        mesh_transport_send_data(opcode, p_hci_event, data_len);
    }
    return WICED_BT_SUCCESS;
}

typedef struct
{
    void        *next;
    uint16_t    id;
    uint16_t    len;
    uint8_t     data[1];
} mesh_nvram_block_t;

mesh_nvram_block_t  *mesh_application_mcu_memory_first_block = NULL;

mesh_nvram_block_t *mesh_application_mcu_memory_find(uint16_t id)
{
    mesh_nvram_block_t  *p;
    for (p = mesh_application_mcu_memory_first_block; p != NULL; p = (mesh_nvram_block_t*)p->next)
    {
        if (p->id == id)
            break;
    }
    return p;
}

uint16_t mesh_application_mcu_memory_read(uint16_t id, uint16_t buf_len, uint8_t *p_buf)
{
    mesh_nvram_block_t  *p;
    if ((p = mesh_application_mcu_memory_find(id)) == NULL)
    {
        WICED_BT_TRACE("mcu_memory_read: no NVRAM id:%d\n", id);
        return 0;
    }
    WICED_BT_TRACE("mcu_memory_read: id:%d buf_len:%d len:%d\n", id, buf_len, p->len);
    if (buf_len > p->len)
        buf_len = p->len;
    memcpy(p_buf, p->data, buf_len);
    return buf_len;
}

uint16_t mesh_application_mcu_memory_write(uint16_t id, uint16_t data_len, const uint8_t *p_data)
{
    uint8_t buf[257];// data_len can't be > 255. 0 data_len means delete NVRAM ID
    WICED_BT_TRACE("mcu_memory_write: id:%d data_len:%d\n", id, data_len);
    buf[0] = (uint8_t)id;
    buf[1] = (uint8_t)(id >> 8);
    if(data_len)
        memcpy(&buf[2], p_data, data_len);
    mesh_application_send_hci_event(HCI_CONTROL_MESH_EVENT_NVRAM_DATA, buf, data_len + 2);
    return data_len;
}

void command_complete_callback(wiced_bt_dev_vendor_specific_command_complete_params_t *p_command_complete_params)
{
    uint8_t buf[257];

    WICED_BT_TRACE("HCI complete callback: opcode:%04x data_len:%d\n", p_command_complete_params->opcode, p_command_complete_params->param_len);

    buf[0] = (uint8_t)p_command_complete_params->opcode;
    buf[1] = (uint8_t)(p_command_complete_params->opcode >> 8);
    buf[2] = (uint8_t)p_command_complete_params->param_len;
    memcpy(&buf[3], p_command_complete_params->p_param_buf, p_command_complete_params->param_len);
    mesh_application_send_hci_event(HCI_CONTROL_TEST_EVENT_ENCAPSULATED_HCI_EVENT, buf, p_command_complete_params->param_len + 3);
}

wiced_result_t mesh_application_process_hci_cmd(uint16_t opcode, const uint8_t *p_data, uint16_t data_len)
{
    wiced_result_t      ret = WICED_TRUE;
    uint16_t            id;
    mesh_nvram_block_t  *p;

    switch (opcode)
    {
    case HCI_CONTROL_TEST_COMMAND_ENCAPSULATED_HCI_COMMAND:
        if ((data_len < 3) || (data_len - 3 != p_data[2]))
        {
            WICED_BT_TRACE("hci_cmd: bad len %d\n", data_len);
            break;
        }
        id = (uint16_t)p_data[0] | (((uint16_t)p_data[1]) << 8);
        ret = wiced_bt_dev_vendor_specific_command(id, data_len - 3, (uint8_t *)&p_data[3], command_complete_callback);
        break;

    case HCI_CONTROL_COMMAND_PUSH_NVRAM_DATA:
        // make sure it contains NVRAM ID and at least one byte of data
        if (data_len <= 2)
        {
            WICED_BT_TRACE("mesh_application_process_hci_cmd: invalid NVRAM len %d\n", data_len);
            break;
        }
        // make sure NVRAM ID is correct
        id = (uint16_t)p_data[0] | (((uint16_t)p_data[1]) << 8);
        p_data += 2;
        data_len -= 2;

        // make sure this MVRAM ID hasn't been pushed already
        if (NULL != mesh_application_mcu_memory_find(id))
        {
            WICED_BT_TRACE("mesh_application_process_hci_cmd: already pushed NVRAM id %d\n", id);
            break;
        }
        // allocate buffer for that NVRAM ID and place it to the queue
        if ((p = (mesh_nvram_block_t*)wiced_bt_get_buffer(sizeof(mesh_nvram_block_t) + data_len)) == NULL)
        {
            WICED_BT_TRACE("mesh_application_process_hci_cmd: get_buffer(%d) failed\n", sizeof(mesh_nvram_block_t) + data_len);
            break;
        }
        WICED_BT_TRACE("mesh_application_process_hci_cmd: id:%d len:%d\n", id, data_len);
        p->next = mesh_application_mcu_memory_first_block;
        p->id = id;
        p->len = data_len;
        memcpy(p->data, p_data, data_len);
        mesh_application_mcu_memory_first_block = p;
        break;

    case HCI_CONTROL_MESH_COMMAND_APP_START:
        // Start application and then delete all allocated NVRAM chunks
        mesh_application_init();
        while (mesh_application_mcu_memory_first_block != NULL)
        {

            p = mesh_application_mcu_memory_first_block;
            mesh_application_mcu_memory_first_block = (mesh_nvram_block_t*)p->next;
            wiced_bt_free_buffer(p);
        }
        break;

    case HCI_CONTROL_MESH_COMMAND_GATEWAY_CONN_STATUS:
        WICED_BT_TRACE("HCI_CONTROL_MESH_COMMAND_GATEWAY_CONN_STATUS\n");
        wiced_bt_mesh_core_connection_status(p_data[0] + (p_data[1] << 8), p_data[2], 0, 75);
        break;

    case HCI_CONTROL_MESH_COMMAND_SEND_PROXY_DATA:
        WICED_BT_TRACE("SEND_PROXY_DATA\n");
        wiced_bt_mesh_core_proxy_packet(p_data, (uint8_t)data_len);
        break;

    default:
        ret = WICED_FALSE;
    }
    return ret;
}

#ifdef DIRECTED_FORWARDING_SERVER_SUPPORTED
#if defined HCI_CONTROL
/*
 * Send DF statistics Status event over transport
 */
void mesh_df_stats_hci_event_send(void)
{
    wiced_result_t                  result;
    wiced_bt_mesh_df_statistics_t   stats;
    uint8_t                         *p;
    wiced_bt_mesh_hci_event_t       *p_hci_event;

    if (NULL == (p_hci_event = wiced_bt_mesh_create_hci_event(NULL)))
    {
        WICED_BT_TRACE("no buffer to send DF statistics status\n");
    }
    else
    {
        wiced_bt_mesh_df_statistics_get(&stats);
        wiced_bt_mesh_df_statistics_reset();
        uint8_t* p = p_hci_event->data;
        UINT16_TO_STREAM(p, stats.sent_df_msg_cnt);
        UINT16_TO_STREAM(p, stats.relayed_df_to_df_msg_cnt);
        UINT16_TO_STREAM(p, stats.relayed_df_to_flood_msg_cnt);
        UINT16_TO_STREAM(p, stats.relayed_flood_to_df_msg_cnt);
        UINT16_TO_STREAM(p, stats.relayed_flood_to_flood_msg_cnt);
        UINT16_TO_STREAM(p, stats.received_df_msg_cnt);
        UINT16_TO_STREAM(p, stats.sent_df_access_msg_cnt);
        UINT16_TO_STREAM(p, stats.received_df_access_msg_cnt);
        result = mesh_transport_send_data(HCI_CONTROL_MESH_EVENT_DF_STATS_STATUS, (uint8_t*)p_hci_event, (uint16_t)(p - (uint8_t*)p_hci_event));
        WICED_BT_TRACE("sent df stats: result:%d access sent/received:%d/%d sent/received%d/%d relayed df_to_df/df_to_flood/flood_to_df/flood_to_flood:%d/%d/%d/%d\n",
            result, stats.sent_df_access_msg_cnt, stats.received_df_access_msg_cnt,
            stats.sent_df_msg_cnt, stats.received_df_msg_cnt, stats.relayed_df_to_df_msg_cnt,
            stats.relayed_df_to_flood_msg_cnt, stats.relayed_flood_to_df_msg_cnt, stats.relayed_flood_to_flood_msg_cnt);
    }
}
#endif
#endif

#if defined(MESH_HOST_MODE) && defined(WIN32)
void mesh_app_hci_init(void)
{
    DWORD   thread_address;

    // Create a thread to read HCI packets from the host via a TCP socket. Give it time to start.
    CreateThread(0, 0, RpcReceiveThread, 0, 0, &thread_address);
}

static int SOCK_PORT_NUM[] = { 12012, 12012, 12013 };

static DWORD __stdcall RpcReceiveThread(VOID* p)
{
    uint8_t             pkt[1500];
    int                 iResult;
    SOCKADDR_IN         service;
    int                 bytes_rcvd;

    // Create a local SOCKET for incoming connection
    if (INVALID_SOCKET == (m_ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)))
    {
        WICED_BT_TRACE_CRIT("listen socket failed with error: %ld, socket thread exiting\n", WSAGetLastError());
        return 0;
    }

    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("127.0.0.1");
    service.sin_port = htons(SOCK_PORT_NUM[wicedx_emulator_instance]);
    memset(service.sin_zero, 0, sizeof(service.sin_zero));

    if (SOCKET_ERROR == (iResult = bind(m_ListenSocket, (SOCKADDR*)&service, sizeof(service))))
    {
        WICED_BT_TRACE_CRIT("bind failed with error: %d, socket thread exiting\n", WSAGetLastError());
        closesocket(m_ListenSocket);
        m_ListenSocket = INVALID_SOCKET;
        return 0;
    }

    for (; ; )
    {
        WICED_BT_TRACE("Listening for client to connect TCP socket....");

        if (m_ClientSocket != INVALID_SOCKET)
        {
            closesocket(m_ClientSocket);
            m_ClientSocket = INVALID_SOCKET;
        }

        if (SOCKET_ERROR == (iResult = listen(m_ListenSocket, 1)))
        {
            WICED_BT_TRACE_CRIT("TCP socket listen failed with error: %d\n", WSAGetLastError());
            break;
        }

        // Accept the client TCP socket
        if (INVALID_SOCKET == (m_ClientSocket = accept(m_ListenSocket, NULL, NULL)))
        {
            WICED_BT_TRACE_CRIT("Client TCP socket accept failed with error: %d", WSAGetLastError());
            break;
        }

        WICED_BT_TRACE("Client TCP socket accepted OK");

        // Receive until the peer shuts down the connection
        for (; ; )
        {
            bytes_rcvd = readHostTCPpkt((char*)&pkt);

            if (bytes_rcvd <= 0)
            {
                WICED_BT_TRACE_CRIT("Client TCP socket recv failed with error: %d    Closing...", WSAGetLastError());
                break;
            }

            BTU_STACK_LOCK();
            mesh_application_proc_rx_cmd(&pkt[1], bytes_rcvd - 1);
            BTU_STACK_UNLOCK();
        }
    }

    // sockets will get closed when program exits
    WICED_BT_TRACE("Socket thread exiting");

    return 0;
}

static int readHostTCPpkt(unsigned char* pPkt)
{
    unsigned int readLen, hdrLen, dataLen;

    if ((readLen = recv(m_ClientSocket, (char*)pPkt, 1, 0)) != 1)
    {
        WICED_BT_TRACE_CRIT("readHostTCPpkt() Expected 1, got: %d", readLen);
        return (-1);
    }

    // ACL and WICED-HCI share the same basic format
    if (pPkt[0] == MPAF_TRAN_PKT_TYPE)
    {
        if ((hdrLen = recv(m_ClientSocket, (char*)&pPkt[1], 4, 0)) != 4)
        {
            WICED_BT_TRACE_CRIT("readHostTCPpkt() Expected 4, got: %d", readLen);
            return (-1);
        }
        dataLen = pPkt[3] | (pPkt[4] << 8);
    }
    else
    {
        WICED_BT_TRACE_CRIT("!!!!Unknown Type: %u", pPkt[0]);
        return (-1);
    }

    if (dataLen != 0)
    {
        if ((readLen = recv(m_ClientSocket, (char*)&pPkt[1 + hdrLen], dataLen, 0)) != dataLen)
        {
            WICED_BT_TRACE_CRIT("readHostTCPpkt() Expected to read datalen of %u, actually got: %d", dataLen, readLen);
            return (-1);
        }
    }

    return (1 + hdrLen + dataLen);
}
#endif // MESH_HOST_MODE

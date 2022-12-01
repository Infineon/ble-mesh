/*
* $ Copyright 2016-YEAR Cypress Semiconductor $
*/

/** @file
*
* mesh definitions
*
*/

#ifndef __MESH_APPLICATION_H__
#define __MESH_APPLICATION_H__


#ifdef __cplusplus
extern "C"
{
#endif


#ifdef CYW20706A2
#define NVRAM_ID_NODE_INFO          WICED_NVRAM_VSID_START
#elif ( defined(CYW20719B1) || defined(CYW20719B0) || defined(CYW20721B1) || defined (CYW20819A1) || defined(CYW20719B2) || defined(CYW20721B2))
#define NVRAM_ID_NODE_INFO          WICED_NVRAM_VSID_START
#else
#define NVRAM_ID_NODE_INFO  (WICED_NVRAM_VSID_START + 0x100)  // ID of the memory block used for NVRAM access (+0x100 - workaround to get rid of lost NVM IDs)
#endif

#define NVRAM_ID_PAIRED_KEYS        (NVRAM_ID_NODE_INFO + 0)
#define NVRAM_ID_LOCAL_KEYS         (NVRAM_ID_NODE_INFO + 1)
#define NVRAM_ID_LOCAL_UUID         (NVRAM_ID_NODE_INFO + 2)
// the first usable by application NVRAM Identifier
#define NVRAM_ID_APP_START          (NVRAM_ID_NODE_INFO + 3)

#ifdef _DEB_COMMAND_SERVICE

// UUIDs for proprietary COMMAND service and characteristic
#define WICED_BT_MESH_CORE_UUID_SERVICE_COMMAND                 0x7FD3
#define WICED_BT_MESH_CORE_UUID_CHARACTERISTIC_COMMAND_DATA     0x7FCB

#define MESH_COMMAND_START_SCAN_UNPROVISIONED     1       // <cmd=1>(1 byte). Start sending UUIDs of detected unprovisioned devices
#define MESH_COMMAND_STOP_SCAN_UNPROVISIONED      2       // <cmd=2>(1 byte). Stops sending UUIDs of detected unprovisioned devices
#define MESH_COMMAND_PROVISION                    3       // <cmd=3>(1 byte), <uuid>(16bytes), <addr>(2bytes). Starts provisioning of device with uuid and assigns addrt to it
#define MESH_COMMAND_RESET_NODE                   4       // <cmd=4>(1 byte). Resets node and makes it unprovisioned

#define MESH_COMMAND_EVENT_UNPROVISIONED_DEV      1       // <event=1>(1 byte), <uuid>(16bytes). Detected unprovisioned device
#define MESH_COMMAND_EVENT_PB_END                 2       // <event=2>(1 byte), <conn_id>(4bytes), <reason>(1byte). End of provisioning
#endif

#define BE2TOUINT16(p) ((uint16_t)(p)[1] + (((uint16_t)(p)[0])<<8))
#define UINT16TOBE2(p, ui) (p)[0]=(uint8_t)((ui)>>8); (p)[1]=(uint8_t)(ui)

void mesh_app_gatt_init(void);
void mesh_app_gatt_db_init(wiced_bool_t is_authenticated);
void mesh_app_proxy_gatt_send_cb(uint32_t conn_id, uint32_t ref_data, const uint8_t *packet, uint32_t packet_len);
wiced_bool_t mesh_app_gatt_is_connected(void);
wiced_bool_t mesh_app_adv_send_callback(uint8_t instance, uint8_t transmissions, uint32_t interval, uint8_t type,
										uint8_t* own_bd_addr, const uint8_t* adv_data, uint8_t adv_data_len,
										const uint8_t* scan_rsp_data, uint8_t scan_rsp_data_len);
wiced_bt_gatt_status_t mesh_app_ota_firmware_upgrade_send_data_cb(wiced_bool_t is_notification,uint16_t conn_id,
										uint16_t attr_handle, uint16_t val_len, uint8_t *p_val);

void mesh_application_init(void);
void mesh_app_timer_init(void);
void mesh_app_dfu_init(void);
void mesh_app_setup_nvram_ids(void);
wiced_bool_t mesh_app_node_is_provisioned(void);
uint8_t* mesh_application_get_device_uuid(void);
void mesh_application_factory_reset(void);
#ifdef WICEDX_LINUX
void mesh_application_deinit(void);
#endif
void mesh_app_hci_init(void);
void mesh_app_hci_send_seq_changed(wiced_bt_mesh_core_state_seq_t *p_seq);
void mesh_app_hci_send_private_beacon(wiced_bt_mesh_core_state_beacon_t* p_beacon);
void mesh_app_hci_send_proxy_service_adv(wiced_bt_mesh_core_state_proxy_serivce_t* p_service);
uint16_t mesh_application_mcu_memory_read(uint16_t id, uint16_t buf_len, uint8_t *p_buf);
uint16_t mesh_application_mcu_memory_write(uint16_t id, uint16_t data_len, const uint8_t *p_data);
wiced_result_t mesh_application_process_hci_cmd(uint16_t opcode, const uint8_t *p_data, uint16_t data_len);
wiced_result_t mesh_application_send_hci_event(uint16_t opcode, const uint8_t *p_data, uint16_t data_len);
void mesh_app_aes_encrypt(uint8_t* in_data, uint8_t* out_data, uint8_t* key);

typedef void (*mesh_app_free_t)(uint8_t *p_buf);
uint8_t *mesh_app_alloc_buffer(int len);
void mesh_app_free_buffer(uint8_t *p_buf);

#endif // __MESH_APPLICATION_H__

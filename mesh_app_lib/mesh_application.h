/******************************************************************************
* File Name:   mesh_application.h
*
* Description: This file is the public interface of mesh_application.c
*
* Related Document: See README.md
*
*******************************************************************************
* $ Copyright 2021-YEAR Cypress Semiconductor $
*******************************************************************************/
#ifndef MESH_APPLICATION_H_
#define MESH_APPLICATION_H_

void mesh_state_changed_cb(wiced_bt_mesh_core_state_type_t type, wiced_bt_mesh_core_state_t *p_state);
void mesh_start_stop_scan_callback(wiced_bool_t start, wiced_bool_t is_scan_active);
wiced_bt_mesh_core_received_msg_handler_t mesh_message_handler_callback(uint16_t company_id,
										uint16_t opcode, uint16_t *p_model_id, uint8_t *p_rpl_delay);
uint32_t mesh_nvram_access(wiced_bool_t write, int inx, uint8_t* node_info, uint16_t len,
																			wiced_result_t *p_result);
wiced_bool_t mesh_application_rpl_clr(void);
wiced_bool_t mesh_application_seq_init(void);
wiced_bool_t mesh_application_seq_save(wiced_bt_mesh_core_state_seq_t *p_seq);
wiced_bool_t mesh_application_is_provisioned(void);
uint8_t mesh_application_get_element_count(uint16_t model_id);
void mesh_application_nvram_id_init(void);
void mesh_application_factory_reset(void);
extern void mesh_core_reset(wiced_bool_t reboot, wiced_bool_t save);

extern wiced_bool_t mesh_config_client;
extern wiced_bool_t mesh_node_authenticated;
extern uint16_t mesh_nvm_idx_seq;
extern uint8_t  wiced_bt_mesh_scene_max_num;
extern uint8_t  wiced_bt_mesh_scheduler_events_max_num;
extern wiced_bt_cfg_settings_t* p_wiced_bt_mesh_cfg_settings;
#endif /* MESH_APPLICATION_H_ */

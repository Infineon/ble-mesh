/*
 * $ Copyright 2016-YEAR Cypress Semiconductor $
 */

/**************************************************************************//**
* file <mesh_private_proxy.h>
*
* Mesh Private Proxy
*
******************************************************************************/
#ifndef __MESH_PRIVATE_PROXY_H__
#define __MESH_PRIVATE_PROXY_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*! \cond SUPPRESS_DOXYGEN */

/*---------------------------------------------------------------------------*/
/*                     wiced_bt_mesh_model_defs.h                            */
/*---------------------------------------------------------------------------*/

/**
 * NOTE: This is preliminary implementation subject to change
 * @anchor BT_MESH_GATT_UUID
 * @name UUIDs for services and characteristics
 * \details The following is the list of service and characteristic UUIDs that a mesh node can support.
 * @{
 */
#define WICED_BT_MESH_CORE_UUID_SERVICE_PROXY_SOLICITATION              0x7FCB
/** @} BT_MESH_GATT_UUID */

/**
 * NOTE: This is preliminary implementation subject to change
 * @anchor WICED_BT_MESH_MODEL_ID
 * @name Model identifiers
 * \details The following is the list of Blueoth SIG defined Model IDs that a mesh node can support.
 * @{
 */
#ifdef WICED_BT_MESH_USE_TEMP_OPCODES
#define WICED_BT_MESH_CORE_MODEL_ID_MESH_PRIVATE_BEACON_SRV             0xBF40
#define WICED_BT_MESH_CORE_MODEL_ID_MESH_PRIVATE_BEACON_CLNT            0xBF41
#define WICED_BT_MESH_CORE_MODEL_ID_ON_DEMAND_PRIVATE_PROXY_SRV         0xBF50
#define WICED_BT_MESH_CORE_MODEL_ID_ON_DEMAND_PRIVATE_PROXY_CLNT        0xBF51
#define WICED_BT_MESH_CORE_MODEL_ID_SOLICITATION_PDU_RPL_SRV            0xBF58
#define WICED_BT_MESH_CORE_MODEL_ID_SOLICITATION_PDU_RPL_CLNT           0xBF59
#else
#define WICED_BT_MESH_CORE_MODEL_ID_MESH_PRIVATE_BEACON_SRV             0x000A
#define WICED_BT_MESH_CORE_MODEL_ID_MESH_PRIVATE_BEACON_CLNT            0x000B
#define WICED_BT_MESH_CORE_MODEL_ID_ON_DEMAND_PRIVATE_PROXY_SRV         0x000C
#define WICED_BT_MESH_CORE_MODEL_ID_ON_DEMAND_PRIVATE_PROXY_CLNT        0x000D
#define WICED_BT_MESH_CORE_MODEL_ID_SOLICITATION_PDU_RPL_SRV            0x0014
#define WICED_BT_MESH_CORE_MODEL_ID_SOLICITATION_PDU_RPL_CLNT           0x0015
#endif

/** @} WICED_BT_MESH_MODEL_ID */

/**
 * NOTE: This is preliminary implementation subject to change
 * @anchor WICED_BT_MESH_MODEL_OPCODE
 * @name Mesh model opcodes
 * \details The following is the list of the Mesh model opcodes.
 * @{
 */

#ifdef WICED_BT_MESH_USE_TEMP_OPCODES
 /* Mesh Private Beacon opcodes */
#define WICED_BT_MESH_CORE_CMD_CONFIG_PRIVATE_BEACON_GET                        0xB711
#define WICED_BT_MESH_CORE_CMD_CONFIG_PRIVATE_BEACON_SET                        0xB712
#define WICED_BT_MESH_CORE_CMD_CONFIG_PRIVATE_BEACON_STATUS                     0xB713
#define WICED_BT_MESH_CORE_CMD_CONFIG_PRIVATE_GATT_PROXY_GET                    0xB714
#define WICED_BT_MESH_CORE_CMD_CONFIG_PRIVATE_GATT_PROXY_SET                    0xB715
#define WICED_BT_MESH_CORE_CMD_CONFIG_PRIVATE_GATT_PROXY_STATUS                 0xB716
#define WICED_BT_MESH_CORE_CMD_CONFIG_PRIVATE_NODE_IDENTITY_GET                 0xB718
#define WICED_BT_MESH_CORE_CMD_CONFIG_PRIVATE_NODE_IDENTITY_SET                 0xB719
#define WICED_BT_MESH_CORE_CMD_CONFIG_PRIVATE_NODE_IDENTITY_STATUS              0xB71A

/* On-Demand Private Proxy opcodes */
#define WICED_BT_MESH_CORE_CMD_CONFIG_ON_DEMAND_PRIVATE_PROXY_GET               0xB800
#define WICED_BT_MESH_CORE_CMD_CONFIG_ON_DEMAND_PRIVATE_PROXY_SET               0xB801
#define WICED_BT_MESH_CORE_CMD_CONFIG_ON_DEMAND_PRIVATE_PROXY_STATUS            0xB802

/* Solicitation PDU RPL Configuration opcodes */
#define WICED_BT_MESH_CORE_CMD_SOLICITATION_PDU_RPL_ITEMS_CLEAR                 0xB815
#define WICED_BT_MESH_CORE_CMD_SOLICITATION_PDU_RPL_ITEMS_CLEAR_UNACKED         0xB816
#define WICED_BT_MESH_CORE_CMD_SOLICITATION_PDU_RPL_ITEMS_STATUS                0xB817
#else
 /* Mesh Private Beacon opcodes */
#define WICED_BT_MESH_CORE_CMD_CONFIG_PRIVATE_BEACON_GET                        0x8060
#define WICED_BT_MESH_CORE_CMD_CONFIG_PRIVATE_BEACON_SET                        0x8061
#define WICED_BT_MESH_CORE_CMD_CONFIG_PRIVATE_BEACON_STATUS                     0x8062
#define WICED_BT_MESH_CORE_CMD_CONFIG_PRIVATE_GATT_PROXY_GET                    0x8063
#define WICED_BT_MESH_CORE_CMD_CONFIG_PRIVATE_GATT_PROXY_SET                    0x8064
#define WICED_BT_MESH_CORE_CMD_CONFIG_PRIVATE_GATT_PROXY_STATUS                 0x8065
#define WICED_BT_MESH_CORE_CMD_CONFIG_PRIVATE_NODE_IDENTITY_GET                 0x8066
#define WICED_BT_MESH_CORE_CMD_CONFIG_PRIVATE_NODE_IDENTITY_SET                 0x8067
#define WICED_BT_MESH_CORE_CMD_CONFIG_PRIVATE_NODE_IDENTITY_STATUS              0x8068

 /* On-Demand Private Proxy opcodes */
#define WICED_BT_MESH_CORE_CMD_CONFIG_ON_DEMAND_PRIVATE_PROXY_GET               0x8069
#define WICED_BT_MESH_CORE_CMD_CONFIG_ON_DEMAND_PRIVATE_PROXY_SET               0x806A
#define WICED_BT_MESH_CORE_CMD_CONFIG_ON_DEMAND_PRIVATE_PROXY_STATUS            0x806B

 /* Solicitation PDU RPL Configuration opcodes */
#define WICED_BT_MESH_CORE_CMD_SOLICITATION_PDU_RPL_ITEMS_CLEAR                 0x8078
#define WICED_BT_MESH_CORE_CMD_SOLICITATION_PDU_RPL_ITEMS_CLEAR_UNACKED         0x8079
#define WICED_BT_MESH_CORE_CMD_SOLICITATION_PDU_RPL_ITEMS_STATUS                0x807A
#endif

/** @} WICED_BT_MESH_MODEL_OPCODE */

/**
 * @anchor MESH_PRIVATE_BEACON_STATES
 * @name Mesh Private Beacon States
 * @{
 */
#define WICED_BT_MESH_PRIVATE_BEACON_DISABLED           0x00 /**< The node does not broadcast Mesh Private beacons. */
#define WICED_BT_MESH_PRIVATE_BEACON_ENABLED            0x01 /**< The node broadcasts Mesh Private beacons. */
/** @} MESH_PRIVATE_BEACON_STATES */

/**
 * @anchor MESH_PRIVATE_GATT_PROXY_STATES
 * @name Mesh Private GATT Proxy States
 * @{
 */
#define WICED_BT_MESH_PRIVATE_PROXY_DISABLED            0x00 /**< The Private Proxy functionality is supported and disabled. */
#define WICED_BT_MESH_PRIVATE_PROXY_ENABLED             0x01 /**< The Private Proxy functionality is supported and enabled. */
#define WICED_BT_MESH_PRIVATE_PROXY_NOT_SUPPORTED       0x02 /**< The Private Proxy functionality is not supported. */
/** @} MESH_PRIVATE_GATT_PROXY_STATES */

/**
 * @anchor MESH_PRIVATE_NODE_IDENTITY_STATES
 * @name Mesh Private Node Identity States
 * @{
 */
#define WICED_BT_MESH_PRIVATE_NODE_IDENTITY_DISABLED    0x00 /**< Advertising with Private Node Identity for a subnet is stopped. */
#define WICED_BT_MESH_PRIVATE_NODE_IDENTITY_ENABLED     0x01 /**< Advertising with Private Node Identity for a subnet is running. */
#define WICED_BT_MESH_PRIVATE_NODE_IDENTITY_NOT_SUPPORTED 0x02 /**< Advertising with Private Node Identity is not supported. */
/** @} MESH_PRIVATE_NODE_IDENTITY_STATES */

#define WICED_BT_MESH_PRIVATE_BEACON_RANDOM_LEN         13
#define WICED_BT_MESH_RANDOM_UPDATE_INTERVAL_STEPS_DEFAULT  60   /**< Default random update interval is 60 x 10 seconds (10 minutes) */

#define WICED_BT_MESH_ON_DEMAND_PROXY_DEFAULT           60   /**< Default on-demand proxy advertising time is 60 seconds */


/*---------------------------------------------------------------------------*/
/*                        wiced_bt_mesh_models.h                             */
/*---------------------------------------------------------------------------*/

/**
 * Mesh Private Proxy Client Models
 */
 #define WICED_BT_MESH_MODEL_PRIVATE_PROXY_CLIENT \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_MESH_PRIVATE_BEACON_CLNT, wiced_bt_mesh_private_beacon_client_message_handler, NULL, NULL }, \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_ON_DEMAND_PRIVATE_PROXY_CLNT, wiced_bt_mesh_on_demand_private_proxy_client_message_handler, NULL, NULL }, \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_SOLICITATION_PDU_RPL_CLNT, wiced_bt_mesh_solicitation_pdu_rpl_config_client_message_handler, NULL, NULL }

/**
 * Mesh Private Proxy Server Models
 */
#define WICED_BT_MESH_MODEL_PRIVATE_PROXY_SERVER \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_MESH_PRIVATE_BEACON_SRV, wiced_bt_mesh_private_beacon_server_message_handler, NULL, NULL }, \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_ON_DEMAND_PRIVATE_PROXY_SRV, wiced_bt_mesh_on_demand_private_proxy_server_message_handler, NULL, NULL }, \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_SOLICITATION_PDU_RPL_SRV, wiced_bt_mesh_solicitation_pdu_rpl_config_server_message_handler, NULL, NULL }

 /**
 * \brief Private Beacon Client Message Handler
 * \details The Mesh Core library calls this function for each message received.  The function returns WICED_TRUE if the message is destined for this specific model and successfully processed, and returns WICED_FALSE otherwise.
 * The function parses the message and if appropriate calls the parent back to perform functionality.
 *
 * @param       p_event Mesh event with information about received message.
 * @param       p_data Pointer to the data portion of the message
 * @param       data_len Length of the data in the message
 *
 * @return      WICED_TRUE if the message is for this company ID/Model/Element Index combination, WICED_FALSE otherwise.
 */
wiced_bool_t wiced_bt_mesh_private_beacon_client_message_handler(wiced_bt_mesh_event_t *p_event, uint8_t *p_data, uint16_t data_len);

/**
* \brief Private Beacon Server Message Handler
* \details The Mesh Core library calls this function for each message received.  The function returns WICED_TRUE if the message is destined for this specific model and successfully processed, and returns WICED_FALSE otherwise.
* The function parses the message and if appropriate calls the parent back to perform functionality.
*
* @param       p_event Mesh event with information about received message.
* @param       p_data Pointer to the data portion of the message
* @param       data_len Length of the data in the message
*
* @return      WICED_TRUE if the message is for this company ID/Model/Element Index combination, WICED_FALSE otherwise.
*/
wiced_bool_t wiced_bt_mesh_private_beacon_server_message_handler(wiced_bt_mesh_event_t *p_event, uint8_t *p_data, uint16_t data_len);

/**
* \brief On-Demand Private Proxy Client Message Handler
* \details The Mesh Core library calls this function for each message received.  The function returns WICED_TRUE if the message is destined for this specific model and successfully processed, and returns WICED_FALSE otherwise.
* The function parses the message and if appropriate calls the parent back to perform functionality.
*
* @param       p_event Mesh event with information about received message.
* @param       p_data Pointer to the data portion of the message
* @param       data_len Length of the data in the message
*
* @return      WICED_TRUE if the message is for this company ID/Model/Element Index combination, WICED_FALSE otherwise.
*/
wiced_bool_t wiced_bt_mesh_on_demand_private_proxy_client_message_handler(wiced_bt_mesh_event_t *p_event, uint8_t *p_data, uint16_t data_len);

/**
* \brief On-Demand Private Proxy Server Message Handler
* \details The Mesh Core library calls this function for each message received.  The function returns WICED_TRUE if the message is destined for this specific model and successfully processed, and returns WICED_FALSE otherwise.
* The function parses the message and if appropriate calls the parent back to perform functionality.
*
* @param       p_event Mesh event with information about received message.
* @param       p_data Pointer to the data portion of the message
* @param       data_len Length of the data in the message
*
* @return      WICED_TRUE if the message is for this company ID/Model/Element Index combination, WICED_FALSE otherwise.
*/
wiced_bool_t wiced_bt_mesh_on_demand_private_proxy_server_message_handler(wiced_bt_mesh_event_t *p_event, uint8_t *p_data, uint16_t data_len);

/**
* \brief Solicitation PDU RPL Configuration Client Message Handler
* \details The Mesh Core library calls this function for each message received.  The function returns WICED_TRUE if the message is destined for this specific model and successfully processed, and returns WICED_FALSE otherwise.
* The function parses the message and if appropriate calls the parent back to perform functionality.
*
* @param       p_event Mesh event with information about received message.
* @param       p_data Pointer to the data portion of the message
* @param       data_len Length of the data in the message
*
* @return      WICED_TRUE if the message is for this company ID/Model/Element Index combination, WICED_FALSE otherwise.
*/
wiced_bool_t wiced_bt_mesh_solicitation_pdu_rpl_config_client_message_handler(wiced_bt_mesh_event_t *p_event, uint8_t *p_data, uint16_t data_len);

/**
* \brief Solicitation PDU RPL Configuration Server Message Handler
* \details The Mesh Core library calls this function for each message received.  The function returns WICED_TRUE if the message is destined for this specific model and successfully processed, and returns WICED_FALSE otherwise.
* The function parses the message and if appropriate calls the parent back to perform functionality.
*
* @param       p_event Mesh event with information about received message.
* @param       p_data Pointer to the data portion of the message
* @param       data_len Length of the data in the message
*
* @return      WICED_TRUE if the message is for this company ID/Model/Element Index combination, WICED_FALSE otherwise.
*/
wiced_bool_t wiced_bt_mesh_solicitation_pdu_rpl_config_server_message_handler(wiced_bt_mesh_event_t *p_event, uint8_t *p_data, uint16_t data_len);

/**
 * \brief Process None Connectable Advertising Packets to check if the packet is a proxy solicitation PDU.
 *
 * @param  p_adv_report Advertising report paremeters
 * @param  p_adv_data Advertising data
 *
 * @return   WICED_TRUE - it is a proxy solicitation PDU and had been processed.
 */
wiced_bool_t wiced_bt_mesh_proxy_solicitation_nonconnectable_adv_packet(wiced_bt_ble_scan_results_t* p_adv_report, uint8_t* p_adv_data);


/*---------------------------------------------------------------------------*/
/*                      wiced_bt_mesh_provision.h                            */
/*---------------------------------------------------------------------------*/

/*  This structure contains information sent from the provisioner application to set Private Beacon State of a node */
typedef PACKED struct
{
    uint8_t  state;                     /**< Target private beacon state (see @ref MESH_PRIVATE_BEACON_STATES) */
    wiced_bool_t set_interval;          /**< WICED_TRUE: contain random update interval steps value */
    uint8_t  random_update_interval;    /**< Private beacon random update interval steps (in unit of 10 seconds) */
} wiced_bt_mesh_config_private_beacon_set_data_t;

/*  This structure contains information sent from the provisioner library to provisioner application with the status of the private beacon */
typedef PACKED struct
{
    uint8_t  state;                     /**< Current private beacon state (see @ref MESH_PRIVATE_BEACON_STATES) */
    uint8_t  random_update_interval;    /**< Private beacon random update interval steps (in unit of 10 seconds) */
} wiced_bt_mesh_config_private_beacon_status_data_t;

/*  This structure contains information sent from the provisioner application to set GATT Proxy state of a node */
typedef PACKED struct
{
    uint8_t  state;                     /**< New GATT Proxy state value (see @ref MESH_GATT_PROXY_STATES) */
} wiced_bt_mesh_config_private_gatt_proxy_set_data_t;

/*  This structure contains information sent to the provisioner application with GATT Proxy state */
typedef PACKED struct
{
    uint8_t  state;                     /**< Current GATT Proxy state value (see @ref MESH_GATT_PROXY_STATES) */
} wiced_bt_mesh_config_private_gatt_proxy_status_data_t;

/* This structure contains information sent from the provisioner application to get Node Identity Status of a node */
typedef PACKED struct
{
    uint16_t net_key_idx;               /**< Index of the NetKey */
} wiced_bt_mesh_config_private_node_identity_get_data_t;

/* This structure contains information sent from the provisioner application to set current Node Identity state for a subnet */
typedef PACKED struct
{
    uint16_t net_key_idx;               /**< Index of the NetKey */
    uint8_t  identity;                  /**< New Node Identity state */
} wiced_bt_mesh_config_private_node_identity_set_data_t;

/* This structure contains information sent from the provisioner library to provisioner application with the status of the node reset */
typedef PACKED struct
{
    uint8_t  status;                    /**< Status Code for the requesting message */
    uint16_t net_key_idx;               /**< Index of the NetKey */
    uint8_t  identity;                  /**< Node Identity state */
} wiced_bt_mesh_config_private_node_identity_status_data_t;

/*  This structure contains information sent from the provisioner application to set On-Demand Private GATT Proxy state of a node */
typedef PACKED struct
{
    uint8_t  state;                     /**< New On-Demand Private GATT Proxy state value */
} wiced_bt_mesh_config_on_demand_private_proxy_set_data_t;

/*  This structure contains information sent to the provisioner application with On-Demand Private GATT Proxy state */
typedef PACKED struct
{
    uint8_t  state;                     /**< Current On-Demand Private GATT Proxy state value */
} wiced_bt_mesh_config_on_demand_private_proxy_status_data_t;

/* This structure contains data for unicast address range */
typedef struct
{
    unsigned int length_present : 1;    /**< Indicates the presence or absence of the RangeLength field */
    unsigned int range_start : 15;      /**< 15 least significant bits of the starting unicast address */
    unsigned int range_length : 8;      /**< Number of addresses in the range (0x02 – 0xFF) */
} wiced_bt_mesh_unicast_address_range_t;


/**
 * \brief Get Private Beacon state.
 * \details The application can call this function to send
 * a Config Private Beacon Get message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_config_private_beacon_get(wiced_bt_mesh_event_t *p_event);

/**
 * \brief Set Private Beacon state.
 * \details The application can call this function to send
 * a Config Private Beacon Set message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  p_set Pointer to the data structure of Config Private Beacon Set message.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_config_private_beacon_set(wiced_bt_mesh_event_t *p_event, wiced_bt_mesh_config_private_beacon_set_data_t *p_set);

/**
 * \brief Get Private GATT Proxy State.
 * \details The application can call this function to send a Config Private GATT Proxy Get message to a peer device.
 *
 * @param[in]   p_event Mesh event with information about received message which contain the address of the peer device.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_config_private_gatt_proxy_get(wiced_bt_mesh_event_t *p_event);

/**
 * \brief Set Private GATT Proxy State.
 * \details The application can call this function to send a Config Private GATT Proxy Set message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  p_set   Pointer to the data structure of Set Private GATT Proxy State..
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_config_private_gatt_proxy_set(wiced_bt_mesh_event_t *p_event, wiced_bt_mesh_config_private_gatt_proxy_set_data_t *p_set);

/**
 * \brief Get On-Demand Private GATT Proxy State.
 * \details The application can call this function to send a Config On-Demand Private GATT Proxy Get message to a peer device.
 *
 * @param[in]   p_event Mesh event with information about received message which contain the address of the peer device.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_config_on_demand_private_proxy_get(wiced_bt_mesh_event_t *p_event);

/**
 * \brief Set On-Demand Private GATT Proxy State.
 * \details The application can call this function to send a Config On-Demand Private GATT Proxy Set message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  p_set   Pointer to the data structure of Set On-Demand Private GATT Proxy State..
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_config_on_demand_private_proxy_set(wiced_bt_mesh_event_t *p_event, wiced_bt_mesh_config_on_demand_private_proxy_set_data_t *p_set);

/**
 * \brief Get Private Node Identity State.
 * \details The application can call this function to send
 * a Config Private Node Identity Get message to a peer device.
 *
 * @param[in]  p_event  Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  p_get    Pointer to the data structure of Get Private Node Identity State.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_config_private_node_identity_get(wiced_bt_mesh_event_t *p_event, wiced_bt_mesh_config_private_node_identity_get_data_t *p_get);

/**
 * \brief Set Private Node Identity State.
 * \details The application can call this function to send
 * a Config Private Node Identity message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  p_set   Pointer to the data structure of Set Private Node Identity State.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_config_private_node_identity_set(wiced_bt_mesh_event_t *p_event, wiced_bt_mesh_config_private_node_identity_set_data_t *p_set);

/**
 * \brief Clear Solicitation PDU RPL list.
 * \details The application can call this function to send
 * a Solicitation PDU RPL Items Clear message to a peer device.
 *
 * @param[in]  p_event  Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  p_range  Pointer to the data structure of Solicitation PDU RPL Items Clear.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_config_solicitation_pdu_rpl_items_clear(wiced_bt_mesh_event_t *p_event, wiced_bt_mesh_unicast_address_range_t *p_range);

/**
* \brief Send solicitation PDU.
* \details The function can be called by the application to Send solicitation PDU to Remote Private Proxy Server.
*
* @param[in]       net_key_idx  NetKey index
* @param[in]       seq          Solicitation sequence number
* @param[in]       src          Source address
* @param[in]       dst          Destination address
*
* @return   WICED_TRUE/WICED_FALSE - success/failed.
*/
wiced_bool_t wiced_bt_mesh_core_send_solicitation_pdu(uint16_t net_key_idx, uint32_t seq, uint16_t src, uint16_t dst);

/*! \endcond */

#ifdef __cplusplus
}
#endif

#endif /* __MESH_PRIVATE_PROXY_H__ */

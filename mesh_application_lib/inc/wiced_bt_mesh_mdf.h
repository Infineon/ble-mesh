/*
 * $ Copyright 2016-YEAR Cypress Semiconductor $
 */



/**************************************************************************//**
* file <wiced_bt_mesh_mdf.h>
*
* Mesh Directed Forwarding Configuration Models
*
* Directed forwarding is designed to help improve performance of a multi-hop network by selecting only a subset of nodes to relay a message from a source to a destination
* The Directed Forwarding Configuration Server model is used to support the configuration of the directed forwarding functionality of a node.
* The Directed Forwarding Configuration Client model is used to support the functionality of a node that can configure the directed forwarding functionality of another node.
*
******************************************************************************/
#ifndef __WICED_BT_MESH_MDF_H__
#define __WICED_BT_MESH_MDF_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*! \cond SUPPRESS_DOXYGEN */

/*---------------------------------------------------------------------------*/
/*                     wiced_bt_mesh_mdf.h                                   */
/*---------------------------------------------------------------------------*/

/**
 * NOTE: This is preliminary implementation subject to change
 * @anchor WICED_BT_MESH_MODEL_ID
 * @name Model identifiers
 * \details The following is the list of Blueoth SIG defined Model IDs that a mesh node can support.
 * @{
 */

#ifdef WICED_BT_MESH_USE_TEMP_OPCODES
#define WICED_BT_MESH_CORE_MODEL_ID_DIRECTED_FORWARDING_SRV             0xBF30
#define WICED_BT_MESH_CORE_MODEL_ID_DIRECTED_FORWARDING_CLNT            0xBF31
#else
#define WICED_BT_MESH_CORE_MODEL_ID_DIRECTED_FORWARDING_SRV             0x0006
#define WICED_BT_MESH_CORE_MODEL_ID_DIRECTED_FORWARDING_CLNT            0x0007
#endif

#define WICED_BT_MESH_CORE_MODEL_ID_NETWORK_FILTER_SRV                  0xFF80
#define WICED_BT_MESH_CORE_MODEL_ID_NETWORK_FILTER_CLNT                 0xFF81

/** @} WICED_BT_MESH_MODEL_ID */

/**
 * NOTE: This is preliminary implementation subject to change
 * @anchor WICED_BT_MESH_MODEL_OPCODE
 * @name Mesh model opcodes
 * \details The following is the list of the Mesh model opcodes.
 * @{
 */

/* Direct Forwarding opcodes */
#ifdef WICED_BT_MESH_USE_TEMP_OPCODES
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_CONTROL_GET                      0xBF30
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_CONTROL_SET                      0xBF31
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_CONTROL_STATUS                   0xBF32
#define WICED_BT_MESH_CORE_CMD_DF_PATH_METRIC_GET                           0xBF33
#define WICED_BT_MESH_CORE_CMD_DF_PATH_METRIC_SET                           0xBF34
#define WICED_BT_MESH_CORE_CMD_DF_PATH_METRIC_STATUS                        0xBF35
#define WICED_BT_MESH_CORE_CMD_DF_DISCOVERY_TABLE_CAPABILITIES_GET          0xBF36
#define WICED_BT_MESH_CORE_CMD_DF_DISCOVERY_TABLE_CAPABILITIES_SET          0xBF37
#define WICED_BT_MESH_CORE_CMD_DF_DISCOVERY_TABLE_CAPABILITIES_STATUS       0xBF38
#define WICED_BT_MESH_CORE_CMD_DF_FORWARDING_TABLE_ADD                      0xBF39
#define WICED_BT_MESH_CORE_CMD_DF_FORWARDING_TABLE_DELETE                   0xBF3A
#define WICED_BT_MESH_CORE_CMD_DF_FORWARDING_TABLE_STATUS                   0xBF3B
#define WICED_BT_MESH_CORE_CMD_DF_FORWARDING_TABLE_DEPENDENTS_ADD           0xBF3C
#define WICED_BT_MESH_CORE_CMD_DF_FORWARDING_TABLE_DEPENDENTS_DELETE        0xBF3D
#define WICED_BT_MESH_CORE_CMD_DF_FORWARDING_TABLE_DEPENDENTS_STATUS        0xBF3E
#define WICED_BT_MESH_CORE_CMD_DF_FORWARDING_TABLE_DEPENDENTS_GET           0xBF3F
#define WICED_BT_MESH_CORE_CMD_DF_FORWARDING_TABLE_DEPENDENTS_GET_STATUS    0xBF40
#define WICED_BT_MESH_CORE_CMD_DF_FORWARDING_TABLE_ENTRIES_COUNT_GET        0xBF41
#define WICED_BT_MESH_CORE_CMD_DF_FORWARDING_TABLE_ENTRIES_COUNT_STATUS     0xBF42
#define WICED_BT_MESH_CORE_CMD_DF_FORWARDING_TABLE_ENTRIES_GET              0xBF43
#define WICED_BT_MESH_CORE_CMD_DF_FORWARDING_TABLE_ENTRIES_STATUS           0xBF44
#define WICED_BT_MESH_CORE_CMD_DF_WANTED_LANES_GET                          0xBF45
#define WICED_BT_MESH_CORE_CMD_DF_WANTED_LANES_SET                          0xBF46
#define WICED_BT_MESH_CORE_CMD_DF_WANTED_LANES_STATUS                       0xBF47
#define WICED_BT_MESH_CORE_CMD_DF_TWO_WAY_PATH_GET                          0xBF48
#define WICED_BT_MESH_CORE_CMD_DF_TWO_WAY_PATH_SET                          0xBF49
#define WICED_BT_MESH_CORE_CMD_DF_TWO_WAY_PATH_STATUS                       0xBF4A
#define WICED_BT_MESH_CORE_CMD_DF_PATH_ECHO_INTERVAL_GET                    0xBF4B
#define WICED_BT_MESH_CORE_CMD_DF_PATH_ECHO_INTERVAL_SET                    0xBF4C
#define WICED_BT_MESH_CORE_CMD_DF_PATH_ECHO_INTERVAL_STATUS                 0xBF4D
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_NETWORK_TRANSMIT_GET             0xBF4E
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_NETWORK_TRANSMIT_SET             0xBF4F
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_NETWORK_TRANSMIT_STATUS          0xBF50
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_RELAY_RETRANSMIT_GET             0xBF51
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_RELAY_RETRANSMIT_SET             0xBF52
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_RELAY_RETRANSMIT_STATUS          0xBF53
#define WICED_BT_MESH_CORE_CMD_DF_RSSI_THRESHOLD_GET                        0xBF54
#define WICED_BT_MESH_CORE_CMD_DF_RSSI_THRESHOLD_SET                        0xBF55
#define WICED_BT_MESH_CORE_CMD_DF_RSSI_THRESHOLD_STATUS                     0xBF56
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_PATHS_GET                        0xBF57
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_PATHS_STATUS                     0xBF58
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_PUBLISH_POLICY_GET               0xBF59
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_PUBLISH_POLICY_SET               0xBF5A
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_PUBLISH_POLICY_STATUS            0xBF5B
#define WICED_BT_MESH_CORE_CMD_DF_PATH_DISCOVERY_TIMING_CONTROL_GET         0xBF5C
#define WICED_BT_MESH_CORE_CMD_DF_PATH_DISCOVERY_TIMING_CONTROL_SET         0xBF5D
#define WICED_BT_MESH_CORE_CMD_DF_PATH_DISCOVERY_TIMING_CONTROL_STATUS      0xBF5E
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_CONTROL_NETWORK_TRANSMIT_GET     0xBF5F
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_CONTROL_NETWORK_TRANSMIT_SET     0xBF60
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_CONTROL_NETWORK_TRANSMIT_STATUS  0xBF61
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_CONTROL_RELAY_RETRANSMIT_GET     0xBF62
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_CONTROL_RELAY_RETRANSMIT_SET     0xBF63
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_CONTROL_RELAY_RETRANSMIT_STATUS  0xBF64
#else
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_CONTROL_GET                      0x807B
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_CONTROL_SET                      0x807C
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_CONTROL_STATUS                   0x807D
#define WICED_BT_MESH_CORE_CMD_DF_PATH_METRIC_GET                           0x807E
#define WICED_BT_MESH_CORE_CMD_DF_PATH_METRIC_SET                           0x807F
#define WICED_BT_MESH_CORE_CMD_DF_PATH_METRIC_STATUS                        0x8080
#define WICED_BT_MESH_CORE_CMD_DF_DISCOVERY_TABLE_CAPABILITIES_GET          0x8081
#define WICED_BT_MESH_CORE_CMD_DF_DISCOVERY_TABLE_CAPABILITIES_SET          0x8082
#define WICED_BT_MESH_CORE_CMD_DF_DISCOVERY_TABLE_CAPABILITIES_STATUS       0x8083
#define WICED_BT_MESH_CORE_CMD_DF_FORWARDING_TABLE_ADD                      0x8084
#define WICED_BT_MESH_CORE_CMD_DF_FORWARDING_TABLE_DELETE                   0x8085
#define WICED_BT_MESH_CORE_CMD_DF_FORWARDING_TABLE_STATUS                   0x8086
#define WICED_BT_MESH_CORE_CMD_DF_FORWARDING_TABLE_DEPENDENTS_ADD           0x8087
#define WICED_BT_MESH_CORE_CMD_DF_FORWARDING_TABLE_DEPENDENTS_DELETE        0x8088
#define WICED_BT_MESH_CORE_CMD_DF_FORWARDING_TABLE_DEPENDENTS_STATUS        0x8089
#define WICED_BT_MESH_CORE_CMD_DF_FORWARDING_TABLE_DEPENDENTS_GET           0x808A
#define WICED_BT_MESH_CORE_CMD_DF_FORWARDING_TABLE_DEPENDENTS_GET_STATUS    0x808B
#define WICED_BT_MESH_CORE_CMD_DF_FORWARDING_TABLE_ENTRIES_COUNT_GET        0x808C
#define WICED_BT_MESH_CORE_CMD_DF_FORWARDING_TABLE_ENTRIES_COUNT_STATUS     0x808D
#define WICED_BT_MESH_CORE_CMD_DF_FORWARDING_TABLE_ENTRIES_GET              0x808E
#define WICED_BT_MESH_CORE_CMD_DF_FORWARDING_TABLE_ENTRIES_STATUS           0x808F
#define WICED_BT_MESH_CORE_CMD_DF_WANTED_LANES_GET                          0x8090
#define WICED_BT_MESH_CORE_CMD_DF_WANTED_LANES_SET                          0x8091
#define WICED_BT_MESH_CORE_CMD_DF_WANTED_LANES_STATUS                       0x8092
#define WICED_BT_MESH_CORE_CMD_DF_TWO_WAY_PATH_GET                          0x8093
#define WICED_BT_MESH_CORE_CMD_DF_TWO_WAY_PATH_SET                          0x8094
#define WICED_BT_MESH_CORE_CMD_DF_TWO_WAY_PATH_STATUS                       0x8095
#define WICED_BT_MESH_CORE_CMD_DF_PATH_ECHO_INTERVAL_GET                    0x8096
#define WICED_BT_MESH_CORE_CMD_DF_PATH_ECHO_INTERVAL_SET                    0x8097
#define WICED_BT_MESH_CORE_CMD_DF_PATH_ECHO_INTERVAL_STATUS                 0x8098
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_NETWORK_TRANSMIT_GET             0x8099
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_NETWORK_TRANSMIT_SET             0x809A
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_NETWORK_TRANSMIT_STATUS          0x809B
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_RELAY_RETRANSMIT_GET             0x809C
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_RELAY_RETRANSMIT_SET             0x809D
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_RELAY_RETRANSMIT_STATUS          0x809E
#define WICED_BT_MESH_CORE_CMD_DF_RSSI_THRESHOLD_GET                        0x809F
#define WICED_BT_MESH_CORE_CMD_DF_RSSI_THRESHOLD_SET                        0x80A0
#define WICED_BT_MESH_CORE_CMD_DF_RSSI_THRESHOLD_STATUS                     0x80A1
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_PATHS_GET                        0x80A2
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_PATHS_STATUS                     0x80A3
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_PUBLISH_POLICY_GET               0x80A4
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_PUBLISH_POLICY_SET               0x80A5
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_PUBLISH_POLICY_STATUS            0x80A6
#define WICED_BT_MESH_CORE_CMD_DF_PATH_DISCOVERY_TIMING_CONTROL_GET         0x80A7
#define WICED_BT_MESH_CORE_CMD_DF_PATH_DISCOVERY_TIMING_CONTROL_SET         0x80A8
#define WICED_BT_MESH_CORE_CMD_DF_PATH_DISCOVERY_TIMING_CONTROL_STATUS      0x80A9
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_CONTROL_NETWORK_TRANSMIT_GET     0x80AB
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_CONTROL_NETWORK_TRANSMIT_SET     0x80AC
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_CONTROL_NETWORK_TRANSMIT_STATUS  0x80AD
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_CONTROL_RELAY_RETRANSMIT_GET     0x80AE
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_CONTROL_RELAY_RETRANSMIT_SET     0x80AF
#define WICED_BT_MESH_CORE_CMD_DF_DIRECTED_CONTROL_RELAY_RETRANSMIT_STATUS  0x80B0
#endif

/* Network Filter opcodes */
#define WICED_BT_MESH_CORE_CMD_NETWORK_FILTER_GET                           0xA700
#define WICED_BT_MESH_CORE_CMD_NETWORK_FILTER_SET                           0xA701
#define WICED_BT_MESH_CORE_CMD_NETWORK_FILTER_STATUS                        0xA702

/** @} WICED_BT_MESH_MODEL_OPCODE */

 /**
  * Mesh Directed Forwarding Server Model.
  * It has to be included in the first element space permitted.
  */
#define WICED_BT_MESH_DIRECTED_FORWARDING_SERVER \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_DIRECTED_FORWARDING_SRV, wiced_bt_mesh_directed_forwarding_server_message_handler, NULL, NULL }

  /**
   * Mesh Directed Forwarding Client Model.
   */
#define WICED_BT_MESH_DIRECTED_FORWARDING_CLIENT \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_DIRECTED_FORWARDING_CLNT, wiced_bt_mesh_directed_forwarding_client_message_handler, NULL, NULL }

   /**
    * Mesh Network Filter Server Model.
    */
#define WICED_BT_MESH_NETWORK_FILTER_SERVER \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_NETWORK_FILTER_SRV, wiced_bt_mesh_network_filter_server_message_handler, NULL, NULL }

    /**
     * Mesh Network Filter Client Model.
     */
#define WICED_BT_MESH_NETWORK_FILTER_CLIENT \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_NETWORK_FILTER_CLNT, wiced_bt_mesh_network_filter_client_message_handler, NULL, NULL }
#define WICED_BT_MESH_NETWORK_FILTER_CLIENT_NO_HANDLER \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_NETWORK_FILTER_CLNT, NULL, NULL, NULL }

/**
 * \brief Model Message Handler
 * \details Application Library typically calls this function when function to process a received message.
 * The function parses the message and if appropriate calls the application back to perform functionality.
 *
 * @param       p_event Mesh event with information about received message.
 * @param       p_data Pointer to the data portion of the message
 * @param       data_len Length of the data in the message
 *
 * @return      WICED_TRUE if the message is for this company ID/Model combination, WICED_FALSE otherwise.
 */
wiced_bool_t wiced_bt_mesh_directed_forwarding_server_message_handler(wiced_bt_mesh_event_t* p_event, uint8_t* p_data, uint16_t data_len);
wiced_bool_t wiced_bt_mesh_directed_forwarding_client_message_handler(wiced_bt_mesh_event_t* p_event, uint8_t* p_data, uint16_t data_len);

wiced_bool_t wiced_bt_mesh_network_filter_server_message_handler(wiced_bt_mesh_event_t* p_event, uint8_t* params, uint16_t params_len);
wiced_bool_t wiced_bt_mesh_network_filter_client_message_handler(wiced_bt_mesh_event_t* p_event, uint8_t* params, uint16_t params_len);

/**
 * \brief Initializes Directed Forwarding.
 * \details If Directed Forwarding is supported then Application shall call this function on startup. Otherwise Application shall not call it.
 *
 * @param[in]   directed_proxy_supported    WICED_TRUE if directed proxy is supported.
 * @param[in]   directed_friend_supported   WICED_TRUE if directed friend is supported.
 * @param[in]   default_rssi_threshold      The value of the default_rssi_threshold is implementation specific and should be 10 dB above the receiver sensitivity.
 * @param[in]   max_dt_entries_cnt          The maximum number of Discovery Table entries supported by the node in a given subnet. It shall be >= 2.
 * @param[in]   node_paths                  The minimum number of paths that the node supports when acting as a Path Origin or as a Path Target. It shall be >= 20.
 * @param[in]   relay_paths                 The minimum number of paths that the node supports when acting as an intermediate Directed Relay node. It shall be >= 20.
 * @param[in]   proxy_paths                 The minimum number of paths that the node supports when acting as a Directed Proxy node.
 *                                          If directed proxy is supported, it shall be >= 20; otherwise it shall be 0.
 * @param[in]   friend_paths                The minimum number of paths that the node supports when acting as a Directed Friend node.
 *                                          If directed friend is supported, it shall be >= 20; otherwise it shall be 0.
 */
wiced_bool_t wiced_bt_mesh_directed_forwarding_init(wiced_bool_t    proxy_supported,
                                                    wiced_bool_t    friend_supported,
                                                    int8_t          default_rssi_threshold,
                                                    uint8_t         max_dt_entries_cnt,
                                                    uint16_t        node_paths,
                                                    uint16_t        relay_paths,
                                                    uint16_t        proxy_paths,
                                                    uint16_t        friend_paths);

#pragma pack(1)
#ifndef PACKED
#define PACKED
#endif

#define WICED_BT_MESH_DF_STATE_CONTROL_DISABLED             0x00
#define WICED_BT_MESH_DF_STATE_CONTROL_ENABLED              0x01
#define WICED_BT_MESH_DF_STATE_CONTROL_NOT_SUPPORTED        0x02
#define WICED_BT_MESH_DF_STATE_CONTROL_IGNORE               0xff

#define WICED_BT_MESH_DF_STATE_CONTROL_FORWARDING_DEFAULT   WICED_BT_MESH_DF_STATE_CONTROL_DISABLED
#define WICED_BT_MESH_DF_STATE_CONTROL_RELAY_DEFAULT        WICED_BT_MESH_DF_STATE_CONTROL_DISABLED
#define WICED_BT_MESH_DF_STATE_CONTROL_PROXY_DEFAULT        WICED_BT_MESH_DF_STATE_CONTROL_DISABLED
#define WICED_BT_MESH_DF_STATE_CONTROL_PROXY_USE_DIRECTED_DEFAULT   WICED_BT_MESH_DF_STATE_CONTROL_NOT_SUPPORTED

#define WICED_BT_MESH_DF_BEARER_IDX_UNASSIGNED              0x0000
#define WICED_BT_MESH_DF_BEARER_IDX_ADV                     0x0001
#define WICED_BT_MESH_DF_BEARER_IDX_GATT                    0x0002
#define WICED_BT_MESH_DF_BEARER_IDX_RFU                     0xfffc

// Directed Control state for a subnet
typedef PACKED struct
{
    uint16_t    netkey_idx;                     // NetKey Index of the NetKey used in the subnet.
    uint8_t     forwarding;                     // Directed Forwarding state. 0x00-Disable; 0x01-Enable; 0x02-0xff-Prohibited
    uint8_t     relay;                          // Directed Relay state. 0x00-Disable; 0x01-Enable; 0x02-0xff-Prohibited
    uint8_t     proxy;                          // Directed Proxy state or 0xFF. 0x00-Disable; 0x01-Enable; 0x02-not-supported; 0x03-0xff-Prohibited; in set and status 0xff-Ignore.
    uint8_t     proxy_use_directed_default;     // Directed Proxy Use Directed Default state or value to ignore. 0x00-Disable; 0x01-Enable; 0x02-not supported; 0x03-0xff-Prohibited; in set and status 0xff-Ignore.
    uint8_t     friend;                         // Directed Friend state or 0xFF. 0x00-Disable; 0x01-Enable; 0x02-not supported; 0x02-0xff-Prohibited; in set and status 0xff-Ignore
} wiced_bt_mesh_df_state_control_t;

/* This structure contains information sent to the provisioner application from the Directed Forwarding Server with current Directed Forwarding Control status information */
typedef PACKED struct
{
    uint8_t  status;                                /**< Status Code for the requesting message */
    wiced_bt_mesh_df_state_control_t    control;    /**< Directed Forwarding Control State */
} wiced_bt_mesh_df_directed_control_status_data_t;

typedef PACKED struct
{
    uint8_t  status;                                /**< Status Code for the requesting message */
    uint16_t netkey_idx;                            /**< NetKey Index of the NetKey used in the subnet */
    uint8_t  type;                                  /**< Path Metric Type: can be only 0. */
    uint8_t  lifetime;                              /**< 0 - 12 min; 1 - 2 hours; 2 - 24 hours; 3 - 10 days*/
} wiced_bt_mesh_df_path_metric_status_data_t;
typedef PACKED struct
{
    uint8_t  status;                                /**< Status Code for the requesting message */
    uint16_t netkey_idx;                            /**< NetKey Index of the NetKey used in the subnet */
    uint8_t  max_concurrent_init;                   /**< the maximum number of concurrent Initialization procedures in a node. 0x00 is a prohibited value. */
    uint8_t  max_discovery_table_entries_count;     /**< the maximum number of Discovery Table entries in a subnet. Shell be >= 2 */
} wiced_bt_mesh_df_discovery_table_capabilities_status_data_t;
typedef PACKED struct
{
    uint8_t  status;                                /**< Status Code for the requesting message */
    uint16_t netkey_idx;                            /**< NetKey Index of the NetKey used in the subnet */
    uint16_t po;                                    /**< Primary element address of the Path Origin */
    uint16_t dst;                                   /**< Destination address */
} wiced_bt_mesh_df_forwarding_table_status_data_t;
typedef PACKED struct
{
    uint8_t  status;                                /**< Status Code for the requesting message */
    uint16_t netkey_idx;                            /**< NetKey Index of the NetKey used in the subnet */
    uint16_t po;                                    /**< Primary element address of the Path Origin */
    uint16_t dst;                                   /**< Destination address */
} wiced_bt_mesh_df_forwarding_table_dependents_status_data_t;
typedef PACKED struct
{
    uint16_t addr;                                  /**< unicast address of the primary element */
    uint8_t  sec_elem_cnt;                          /**< Number of the secondary elements */
} wiced_bt_mesh_df_addr_range_t;
typedef PACKED struct
{
    uint8_t  status;                                /**< Status Code for the requesting message */
    uint16_t netkey_idx;                            /**< NetKey Index of the NetKey used in the subnet */
    wiced_bool_t  get_po;                           /**< Return list of unicast address ranges of dependent nodes of the Path Origin */
    wiced_bool_t  get_pt;                           /**< Return list of unicast address ranges of dependent nodes of the Path Target */
    wiced_bool_t  fixed;                            /**< Return the unicast address ranges of dependent nodes in a fixed path entry */
    uint16_t start_idx;                             /**< Start offset in units of unicast address ranges */
    uint16_t po;                                    /**< Primary element address of the Path Origin */
    uint16_t dst;                                   /**< Destination address */
    uint16_t update_id;                             /**< Current Forwarding Table Update Identifier state. 0xffff means no value */
    uint8_t  po_cnt;                                /**< Number of unicast address ranges in the Dependent_Origin_Unicast_Addr_Range_List field in the message */
    uint8_t  pt_cnt;                                /**< Number of unicast address ranges in the Dependent_Target_Unicast_Addr_Range_List field in the message */
    wiced_bt_mesh_df_addr_range_t dependents[1];    /**< List of unicast address ranges of dependent nodes of the Path Origin and then Path Target - size = po_cnt + pt_cnt */
} wiced_bt_mesh_df_forwarding_table_dependents_get_status_data_t;
typedef PACKED struct
{
    uint8_t  status;                                /**< Status Code for the requesting message */
    uint16_t netkey_idx;                            /**< NetKey Index of the NetKey used in the subnet */
    uint16_t update_id;                             /**< Current Forwarding Table Update Identifier state. 0xffff means no value */
    uint16_t fixed;                                 /**< Number of fixed path entries in the Forwarding Table */
    uint16_t non_fixed;                             /**< Number of non-fixed path entries in the Forwarding Table */
} wiced_bt_mesh_df_forwarding_table_entries_count_status_data_t;
typedef PACKED struct
{
    uint8_t lane_cnt;                               /**< Number of lanes in the path */
    uint16_t path_remaining_time;                   /**< Path lifetime remaining */
    uint8_t  po_fn;                                 /**< Forwarding number of the Path Origin */
}wiced_bt_mesh_df_forwarding_table_entry_non_fixed_t;

typedef PACKED struct
{
    wiced_bool_t  fixed;                            /**< whether the table entry is a fixed path entry */
    wiced_bool_t  back_validated;                   /**< whether or not the backward path has been validated */
    wiced_bt_mesh_df_addr_range_t po;               /**< Path Origin unicast address range */
    uint16_t po_dependents_cnt;                     /**< Current number of entries in the list of dependent nodes of the Path Origin */
    uint16_t po_bearer;                             /**< Index of the bearer toward the Path Origin. Can be any of WICED_BT_MESH_DF_BEARER_IDX_XXX. 0 means no value */
    wiced_bt_mesh_df_addr_range_t pt;               /**< Multicase destination or Path Target unicast address range */
    uint16_t pt_dependents_cnt;                     /**< Current number of entries in the list of dependent nodes of the Path Target */
    uint16_t pt_bearer;                             /**< Index of the bearer toward the Path Origin. Can be any of WICED_BT_MESH_DF_BEARER_IDX_XXX. 0 means no value */
    wiced_bt_mesh_df_forwarding_table_entry_non_fixed_t non_fixed;  /**< non-fixed specific data */
}wiced_bt_mesh_df_forwarding_table_entry_t;
typedef PACKED struct
{
    uint8_t  status;                                /**< Status Code for the requesting message */
    uint16_t netkey_idx;                            /**< NetKey Index of the NetKey used in the subnet */
    wiced_bool_t  fixed;                            /**< Return fixed path entries */
    wiced_bool_t  non_fixed;                        /**< Return non-fixed path entries */
    uint16_t start_idx;                             /**< Start offset in units of unicast address ranges */
    uint16_t po;                                    /**< Primary element address of the Path Origin. 0 means don't match PO */
    uint16_t dst;                                   /**< Destination address . 0 means don't match dst */
    uint16_t update_id;                             /**< Current Forwarding Table Update Identifier state. 0xffff means no value */
    uint8_t  entries_cnt;                           /**< number of elements in the entries array */
    wiced_bt_mesh_df_forwarding_table_entry_t entries[10];       /**< List of Forwarding Table entries (Optional). If number of entries > 10 then allocate memory and use pointer to that structure*/
} wiced_bt_mesh_df_forwarding_table_entries_status_data_t;
typedef PACKED struct
{
    uint8_t  status;                                /**< Status Code for the requesting message */
    uint16_t netkey_idx;                            /**< NetKey Index of the NetKey used in the subnet */
    uint8_t  wanted_lines;                          /**< Current Wanted Lanes state */
} wiced_bt_mesh_df_wanted_lanes_status_data_t;

/* This structure contains information sent to the provisioner application from the Directed Forwarding Server with current Directed Forwarding two_way_path status information */
typedef PACKED struct
{
    uint8_t  status;                                /**< Status Code for the requesting message */
    uint16_t netkey_idx;                            /**< Network key for the network subnet used for that message */
    wiced_bool_t  two_way_path;                     /**< Whether or not the two way path is enabled for that subnet */
} wiced_bt_mesh_df_two_way_path_status_data_t;

typedef PACKED struct
{
    uint8_t  status;                                /**< Status Code for the requesting message */
    uint16_t netkey_idx;                            /**< NetKey Index of the NetKey used in the subnet */
    uint8_t  unicast;                               /**< Current Unicast Echo Interval state */
    uint8_t  multicast;                             /**< Current Multicast Echo Interval state */
} wiced_bt_mesh_df_path_echo_interval_status_data_t;
typedef PACKED struct
{
    uint8_t  count;                                /**< Current Directed Network or Relay or Ctrl Network or Ctrl Relay Transmit Count state */
    uint8_t  interval;                             /**< Current Directed Network or Relay or Ctrl Network or Ctrl Relay Transmit Interval Steps state */
} wiced_bt_mesh_df_directed_transmit_status_data_t;
typedef PACKED struct
{
    uint8_t  default_threshold;                    /**< Default RSSI Threshold state */
    uint8_t  margin;                               /**< Current RSSI Margin state */
} wiced_bt_mesh_df_rssi_threshold_status_data_t;
typedef PACKED struct
{
    uint16_t node_paths;                           /**< Directed Node Paths state */
    uint16_t relay_paths;                          /**< Directed Relay Paths state */
    uint16_t proxy_paths;                          /**< Directed Proxy Paths state */
    uint16_t friend_paths;                         /**< Directed Friend Paths state */
} wiced_bt_mesh_df_directed_paths_status_data_t;
typedef PACKED struct
{
    uint8_t  status;                                /**< Status Code for the requesting message */
    wiced_bool_t directed;                          /**< On true The policy is to use directed forwarding when publishing. Otherwise Managed Flooding. */
    uint16_t elem_addr;                             /**< Address of the element */
    uint16_t company_id;                            /**< Company ID */
    uint16_t model_id;                              /**< Model ID */
} wiced_bt_mesh_df_directed_publish_policy_status_data_t;

/* This structure contains information sent to the provisioner application from the Directed Forwarding Server with current Directed Forwarding path_discovery_timing_control status information */
typedef PACKED struct
{
    uint16_t    path_monitoring_interval;           /**< Path Monitoring Interval in seconds */
    uint16_t    path_discovery_retry_interval;      /**< Path Discovery Retry Interval in seconds */
    wiced_bool_t    path_discovery_interval_high;       /**< false - 5 seconds; true - 30 seconds*/
    wiced_bool_t    lane_discovery_guard_interval_high; /**< Lane Discovery Guard Interval state: false 2 sec; true - 10 seconds */
} wiced_bt_mesh_df_path_discovery_timing_control_status_data_t;

/* This structure contains information sent to the provisioner application from the node with current Network Filter status information */
typedef PACKED struct
{
    uint8_t     status;                 /**< Status Code for the requesting message */
    uint16_t    netkey_idx;             /**< NetKey Index of the NetKey used in the message. */
    uint8_t     filter_mode;            /**< Status Code for the requesting message */
    uint8_t     addr_cnt;               /**< Size of the array addr */
    uint16_t    addr[1];                /**< Array of the addresses with size addr_cnt. */
} wiced_bt_mesh_network_filter_status_data_t;

/**
* Reperesents Directed forwarding statistics of messages receiving, sending and handling by network and bearers layers.
*/
typedef struct
{
    uint16_t    sent_df_msg_cnt;                    /**< Number of messages sent with Directed Forwarding credentials */
    uint16_t    relayed_df_to_df_msg_cnt;           /**< Number of relayed messages received and sent with Directed Forwarding credentials */
    uint16_t    relayed_df_to_flood_msg_cnt;        /**< Number of relayed messages received with Directed Forwarding credentials and sent with flooding credentials*/
    uint16_t    relayed_flood_to_df_msg_cnt;        /**< Number of relayed messages received with flooding credentials and sent with Directed Forwarding credentials*/
    uint16_t    relayed_flood_to_flood_msg_cnt;     /**< Number of relayed messages received and sent with flooding credentials */
    uint16_t    received_df_msg_cnt;                /**< Number of messages received with directed forwarding credentials*/
    uint16_t    sent_df_access_msg_cnt;             /**< Number of of access layer messages successfully sent with Directed Forwarding credentials */
    uint16_t    received_df_access_msg_cnt;         /**< Number of access layer messages successfully received with directed forwarding credentials*/
} wiced_bt_mesh_df_statistics_t;

#pragma pack()

/**
 * \brief Send Set Directed Forwarding Control state.
 * \details The application can call this function to send
 * a Directed Forwarding Control Set message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  p_set Pointer to the data structure for Directed Forwarding Control Set message.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_directed_control_set(wiced_bt_mesh_event_t* p_event, wiced_bt_mesh_df_state_control_t* p_set);

/**
 * \brief Send Send Get Directed Forwarding Control state.
 * \details The application can call this function to send
 * a Directed Forwarding Control Get message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  netkey_idx network key Index for Directed Forwarding Control Get message.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_directed_control_get(wiced_bt_mesh_event_t* p_event, uint16_t netkey_idx);

/**
 * \brief Send Send Set Directed Forwarding path_metric set.
 * \details The application can call this function to send
 * a Directed Forwarding path_metric Set message to a peer device.
 *
 * @param[in]  p_event              Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  netkey_idx           network key Index for Directed Forwarding path_metric Set message.
 * @param[in]  path_metric_type     Path Metric Type
 * @param[in]  path_lifetime        Path Lifetime
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_path_metric_set(wiced_bt_mesh_event_t* p_event, uint16_t netkey_idx, uint8_t path_metric_type, uint8_t path_lifetime);

/**
 * \brief Send Get Directed Forwarding path_metric state.
 * \details The application can call this function to send
 * a Directed Forwarding path_metric Get message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  netkey_idx network key Index for Directed Forwarding path_metric Get message.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_path_metric_get(wiced_bt_mesh_event_t* p_event, uint16_t netkey_idx);

/**
 * \brief Send Get Directed Forwarding discovery_table_capabilities_get.
 * \details The application can call this function to send
 * a Directed Forwarding discovery_table_capabilities Get message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  netkey_idx network key Index for Directed Forwarding discovery_table_capabilities Get message.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_discovery_table_capabilities_get(wiced_bt_mesh_event_t* p_event, uint16_t netkey_idx);

/**
 * \brief Send Get Directed Forwarding discovery_table_capabilities_set.
 * \details The application can call this function to send
 * a Directed Forwarding discovery_table_capabilities Set message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  netkey_idx network key Index for Directed Forwarding discovery_table_capabilities Set message.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_discovery_table_capabilities_set(
    wiced_bt_mesh_event_t* p_event,
    uint16_t    netkey_idx,
    uint8_t     max_concurrent_init);

/**
 * \brief Send Send Set Directed Forwarding wanted_lanes set.
 * \details The application can call this function to send
 * a Directed Forwarding wanted_lanes Set message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  netkey_idx network key Index for Directed Forwarding wanted_lanes Set message.
 * @param[in]  wanted_lanes New Wanted Lanes state
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_wanted_lanes_set(wiced_bt_mesh_event_t* p_event, uint16_t netkey_idx, uint8_t wanted_lanes);

/**
 * \brief Send Send Set Directed Forwarding wanted_lanes get.
 * \details The application can call this function to send
 * a Directed Forwarding wanted_lanes Get message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  netkey_idx network key Index for Directed Forwarding wanted_lanes Get message.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_wanted_lanes_get(wiced_bt_mesh_event_t* p_event, uint16_t netkey_idx);

/**
 * \brief Send Get Directed Forwarding forwarding_table_entries_count_get.
 * \details The application can call this function to send
 * a Directed Forwarding forwarding_table_entries_count Get message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  netkey_idx network key Index for Directed Forwarding forwarding_table_entries_count Get message.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_forwarding_table_entries_count_get(wiced_bt_mesh_event_t* p_event, uint16_t netkey_idx);

/**
 * \brief Send Get Directed Forwarding forwarding_table_entries_get.
 * \details The application can call this function to send
 * a Directed Forwarding forwarding_table_entries Get message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  netkey_idx network key Index for Directed Forwarding forwarding_table_entries Get message.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_forwarding_table_entries_get(
    wiced_bt_mesh_event_t* p_event,
    uint16_t        netkey_idx,
    wiced_bool_t    fixed_path,
    wiced_bool_t    non_fixed_path,
    uint16_t        start_index,
    uint16_t        po,
    uint16_t        dst,
    uint16_t        update_id);

/**
 * \brief Send Set Directed Forwarding two_way_path state.
 * \details The application can call this function to send
 * a Directed Forwarding two_way_path Set message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  netkey_idx - NetKey Index of the NetKey used in the subnet.
 * @param[in]  two_way_path Value to set.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_two_way_path_set(wiced_bt_mesh_event_t* p_event, uint16_t netkey_idx, wiced_bool_t two_way_path);

/**
 * \brief Send Get Directed Forwarding two_way_path state.
 * \details The application can call this function to send
 * a Directed Forwarding two_way_path Get message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  netkey_idx network key Index for Directed Forwarding two_way_path Get message.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_two_way_path_get(wiced_bt_mesh_event_t* p_event, uint16_t netkey_idx);

/**
 * \brief Send Send Set Directed Forwarding path_echo_interval state.
 * \details The application can call this function to send
 * a Directed Forwarding path_echo_interval Set message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  netkey_idx network key Index for Directed Forwarding path_echo_interval Get message.
 * @param[in]  unicast New Unicast Echo Interval state(0x00-0x63) or indication of no state change(0xff)
 * @param[in]  multicast New Multicast Echo Interval state(0x00-0x63) or indication of no state change(0xff)
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_path_echo_interval_set(wiced_bt_mesh_event_t* p_event, uint16_t netkey_idx, uint8_t unicast, uint8_t multicast);

/**
 * \brief Send Get Directed Forwarding path_echo_interval state.
 * \details The application can call this function to send
 * a Directed Forwarding path_echo_interval Get message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  netkey_idx network key Index for Directed Forwarding path_echo_interval Get message.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_path_echo_interval_get(wiced_bt_mesh_event_t* p_event, uint16_t netkey_idx);

/**
 * \brief Send Send Set Directed Forwarding network_transmit set.
 * \details The application can call this function to send
 * a Directed Forwarding network_transmit Set message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  count The number of transmissions of a Network PDU is Count + 1. can be 0x00-0x07.
 * @param[in]  interval The number of 10-millisecond steps. transmission interval = (Interval + 1) * 10. can be 0x00-0x1f.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_network_transmit_set(wiced_bt_mesh_event_t* p_event, uint8_t count, uint8_t interval);

/**
 * \brief Send Get Directed Forwarding directed_network_transmit state.
 * \details The application can call this function to send
 * a Directed Forwarding directed_network_transmit Get message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_directed_network_transmit_get(wiced_bt_mesh_event_t* p_event);

/**
 * \brief Send Send Set Directed Forwarding relay_retransmit set.
 * \details The application can call this function to send
 * a Directed Forwarding relay_retransmit Set message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  count The number of transmissions of a Network PDU is Count + 1. can be 0x00-0x07.
 * @param[in]  interval The number of 10-millisecond steps. transmission interval = (Interval + 1) * 10. can be 0x00-0x1f.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_relay_retransmit_set(wiced_bt_mesh_event_t* p_event, uint8_t count, uint8_t interval);

/**
 * \brief Send Get Directed Forwarding directed_relay_retransmit get.
 * \details The application can call this function to send
 * a Directed Forwarding directed_relay_retransmit Get message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_directed_relay_retransmit_get(wiced_bt_mesh_event_t* p_event);

/**
 * \brief Send Send Set Directed Forwarding rssi_threshold set.
 * \details The application can call this function to send
 * a Directed Forwarding rssi_threshold Set message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  margin value for Directed Forwarding rssi_threshold Set message.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_rssi_threshold_set(wiced_bt_mesh_event_t* p_event, uint8_t margin);

/**
 * \brief Send Get Directed Forwarding rssi_threshold get.
 * \details The application can call this function to send
 * a Directed Forwarding rssi_threshold Get message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_rssi_threshold_get(wiced_bt_mesh_event_t* p_event);

/**
 * \brief Send Get Directed Forwarding directed_paths get.
 * \details The application can call this function to send
 * a Directed Forwarding directed_paths Get message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_directed_paths_get(wiced_bt_mesh_event_t* p_event);

/**
 * \brief Send Get Directed Forwarding directed_publish_policy get.
 * \details The application can call this function to send
 * a Directed Forwarding directed_publish_policy Get message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  elem_addr elem_addr value for Directed Forwarding directed_publish_policy Get message.
 * @param[in]  company_id company_id value for Directed Forwarding directed_publish_policy Get message.
 * @param[in]  model_id model_idvalue for Directed Forwarding directed_publish_policy Get message.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_directed_publish_policy_get(wiced_bt_mesh_event_t* p_event, uint16_t elem_addr, uint16_t company_id, uint16_t model_id);

/**
 * \brief Send Get Directed Forwarding directed_publish_policy get.
 * \details The application can call this function to send
 * a Directed Forwarding directed_publish_policy Get message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  directed On true the policy is to use directed forwarding when publishing - otherwise flooding.
 * @param[in]  elem_addr elem_addr value for Directed Forwarding directed_publish_policy Get message.
 * @param[in]  company_id company_id value for Directed Forwarding directed_publish_policy Get message.
 * @param[in]  model_id model_idvalue for Directed Forwarding directed_publish_policy Get message.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_directed_publish_policy_set(wiced_bt_mesh_event_t* p_event, wiced_bool_t directed, uint16_t elem_addr, uint16_t company_id, uint16_t model_id);

/**
 * \brief Send Send Set Directed Forwarding path_discovery_timing_control Set.
 * \details The application can call this function to send
 * a Directed Forwarding path_discovery_timing_control Set message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_path_discovery_timing_control_set(wiced_bt_mesh_event_t* p_event, wiced_bt_mesh_df_path_discovery_timing_control_status_data_t* p_data);

/**
 * \brief Send Get Directed Forwarding path_discovery_timing_control state.
 * \details The application can call this function to send
 * a Directed Forwarding path_discovery_timing_control Get message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_path_discovery_timing_control_get(wiced_bt_mesh_event_t* p_event);

/**
 * \brief Send Send Set Directed Forwarding forwarding_tbl_add.
 * \details The application can call this function to send
 * a Directed Forwarding forwarding_tbl Set message to a peer device.
 *
 * @param[in]  p_event                  Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  netkey_idx               network key Index for Directed Forwarding forwarding_tbl_add message.
 * @param[in]  backward_path_validated  Indicates whether or not the backward path has been validated
 * @param[in]  po                       Unicast address of the Path Origin
 * @param[in]  po_elem_cnt              Number of the secondary elements on the Path Origin
 * @param[in]  pt                       Unicast Path Target or Multicast Destination address
 * @param[in]  pt_elem_cnt              Number of the secondary elements on the Path Target
 * @param[in]  po_bearer                Index of the bearer toward the Path Origin. Can be any of WICED_BT_MESH_DF_BEARER_IDX_XXX. 0 means no value
 * @param[in]  pt_bearer                Index of the bearer toward the Path Origin. Can be any of WICED_BT_MESH_DF_BEARER_IDX_XXX. 0 means no value
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_forwarding_tbl_add( wiced_bt_mesh_event_t* p_event, uint16_t netkey_idx,
    wiced_bool_t backward_path_validated, uint16_t po, uint8_t po_elem_cnt, uint16_t pt, uint8_t pt_elem_cnt,
    uint16_t po_bearer, uint16_t pt_bearer);

/**
 * \brief Send Send Set Directed Forwarding forwarding_tbl_del.
 * \details The application can call this function to send
 * a Directed Forwarding forwarding_tbl_del message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  netkey_idx network key Index for Directed Forwarding forwarding_tbl_del message.
 * @param[in]  po                       Primary element address of the Path Origin
 * @param[in]  pt                       Destination address
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_forwarding_tbl_del(wiced_bt_mesh_event_t* p_event, uint16_t netkey_idx, uint16_t po, uint16_t pt);

/**
 * \brief Send Send Set Directed Forwarding forwarding_tbl_dependents_add.
 * \details The application can call this function to send
 * a Directed Forwarding forwarding_tbl_dependents_add message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  netkey_idx network key Index for Directed Forwarding forwarding_tbl_dependents_add message.
 * @param[in]  po                       Primary element address of the Path Origin
 * @param[in]  pt                       Destination address
 * @param[in]  po_cnt                   Number of entries in the Dependent_Origin_Unicast_Addr_Range_List field of the message
 * @param[in]  pt_cnt                   Number of entries in the Dependent_Target_Unicast_Addr_Range_List field of the message
 * @param[in]  dependents               List of the unicast address ranges of the dependent nodes of the Path Origin and then PathTarget
 * @param[in]  elem_cnts                List of the secondary elements counts for each dependent in the above array (dependents)
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_forwarding_tbl_dependents_add(wiced_bt_mesh_event_t* p_event, uint16_t netkey_idx, uint16_t po, uint16_t pt,
    uint8_t po_cnt, uint8_t pt_cnt, uint16_t* dependents, uint8_t *elem_cnts);

/**
 * \brief Send Send Set Directed Forwarding forwarding_tbl_dependents_del.
 * \details The application can call this function to send
 * a Directed Forwarding forwarding_tbl_dependents_del message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  netkey_idx network key Index for Directed Forwarding forwarding_tbl_dependents_del message.
 * @param[in]  po                       Primary element address of the Path Origin
 * @param[in]  pt                       Destination address
 * @param[in]  po_cnt                   Number of entries in the Dependent_Origin_Unicast_Addr_Range_List field of the message
 * @param[in]  pt_cnt                   Number of entries in the Dependent_Target_Unicast_Addr_Range_List field of the message
 * @param[in]  dependents               List of the unicast address ranges of the dependent nodes of the Path Origin and then PathTarget
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_forwarding_tbl_dependents_del(wiced_bt_mesh_event_t* p_event, uint16_t netkey_idx, uint16_t po, uint16_t pt,
    uint8_t po_cnt, uint8_t pt_cnt, uint16_t* dependents);

/**
 * \brief Send Send Set Directed Forwarding forwarding_tbl_entries_cnt_get.
 * \details The application can call this function to send
 * a Directed Forwarding forwarding_tbl_entries_cnt_get message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  netkey_idx network key Index for Directed Forwarding forwarding_tbl_entries_cnt_get message.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_forwarding_tbl_entries_cnt_get(
    wiced_bt_mesh_event_t* p_event,
    uint16_t    netkey_idx);

/**
 * \brief Send Send Set Directed Forwarding forwarding_tbl_entries_get.
 * \details The application can call this function to send
 * a Directed Forwarding forwarding_tbl_entries_get message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  netkey_idx network key Index for Directed Forwarding forwarding_tbl_entries_get message.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_forwarding_tbl_entries_get(
    wiced_bt_mesh_event_t* p_event,
    uint16_t        netkey_idx,
    wiced_bool_t    fixed_path,
    wiced_bool_t    none_fixed_path,
    uint16_t        start_index,
    uint16_t        po,
    uint16_t        dst,
    uint16_t* p_update_id,
    wiced_bool_t    check_update_id);

/**
 * \brief Send Send Set Directed Forwarding forwarding_tbl_dependents_get.
 * \details The application can call this function to send
 * a Directed Forwarding forwarding_tbl_dependents_get message to a peer device.
 *
 * @param[in]  p_event      Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  netkey_idx   network key Index for Directed Forwarding forwarding_tbl_dependents_get message.
 * @param[in]  get_po       Return list of unicast address ranges of dependent nodes of the Path Origin
 * @param[in]  get_pt       Return list of unicast address ranges of dependent nodes of the Path Target
 * @param[in]  fixed        Return the unicast address ranges of dependent nodes in a fixed path entry
 * @param[in]  start_idx    Start offset in units of unicast address ranges
 * @param[in]  po           Primary element address of the Path Origin
 * @param[in]  pt          Destination address
 * @param[in]  update_id    Last saved Forwarding Table Update Identifier state
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_forwarding_tbl_dependents_get(wiced_bt_mesh_event_t* p_event, uint16_t netkey_idx, wiced_bool_t get_po, wiced_bool_t get_pt, wiced_bool_t fixed,
    uint16_t start_idx, uint16_t po, uint16_t pt, uint16_t update_id);

/**
 * \brief Send Send Set Directed Forwarding control_network_transmit set.
 * \details The application can call this function to send
 * a Directed Forwarding control_network_transmit Set message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  count The number of transmissions of a Network PDU is Count + 1. can be 0x00-0x07.
 * @param[in]  interval The number of 10-millisecond steps. transmission interval = (Interval + 1) * 10. can be 0x00-0x1f.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_control_network_transmit_set(wiced_bt_mesh_event_t* p_event, uint8_t count, uint8_t interval);

/**
 * \brief Send Get Directed Forwarding directed_control_network_transmit get.
 * \details The application can call this function to send
 * a Directed Forwarding directed_control_network_transmit Get message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_directed_control_network_transmit_get(wiced_bt_mesh_event_t* p_event);

/**
 * \brief Send Send Set Directed Forwarding control_relay_retransmit set.
 * \details The application can call this function to send
 * a Directed Forwarding control_relay_retransmit Set message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  count The number of transmissions of a Network PDU is Count + 1. can be 0x00-0x07.
 * @param[in]  interval The number of 10-millisecond steps. transmission interval = (Interval + 1) * 10. can be 0x00-0x1f.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_control_relay_retransmit_set(wiced_bt_mesh_event_t* p_event, uint8_t count, uint8_t interval);

/**
 * \brief Send Get Directed Forwarding directed_control_relay_retransmit state.
 * \details The application can call this function to send
 * a Directed Forwarding directed_control_relay_retransmit Get message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_df_send_directed_control_relay_retransmit_get(wiced_bt_mesh_event_t* p_event);

/**
 * \brief Send Get Network Filter state.
 * \details The application can call this function to send
 * a Network Filter Get message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  netkey_idx network key Index for Network Filter Get message.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_network_filter_get(wiced_bt_mesh_event_t* p_event, uint16_t netkey_idx);

/**
 * \brief Send Set Network Filter state.
 * \details The application can call this function to send a Network Filter Set message to a peer device.
 *
 * @param[in]  p_event Mesh event with information about received message which contain the address of the peer device.
 * @param[in]  netkey_idx network key Index for Network Filter Set message.
 * @param[in]  filter_mode Filter Mode for Network Filter Set message.
 * @param[in]  cnt number of addresses in the array addr for Network Filter Set message.
 * @param[in]  addr array of addresses for Network Filter Set message.
 *
 * @return   WICED_TRUE/WICED_FALSE - success/failed.
 */
wiced_bool_t wiced_bt_mesh_network_filter_set(wiced_bt_mesh_event_t* p_event, uint16_t netkey_idx, uint8_t filter_mode, uint8_t cnt, uint16_t* addr);

// Initializes network filter. App should call it on startup or on provisioning
void wiced_bt_mesh_network_filter_init(void);


/**
* \brief Requests collected Directed Forwarding statistics.
*
* @param[out]   p_data             pointer where to copy Directed Forwarding statistics
*
* @return       None
*/
void wiced_bt_mesh_df_statistics_get(wiced_bt_mesh_df_statistics_t *p_data);

/**
* \brief Resets Directed Forwarding statistics.
*
* @param       None
*
* @return      None
*/
void wiced_bt_mesh_df_statistics_reset(void);

/*! \endcond */

#ifdef __cplusplus
}
#endif

#endif /* __WICED_BT_MESH_MDF_H__ */

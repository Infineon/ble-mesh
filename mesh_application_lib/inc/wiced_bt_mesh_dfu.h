/*
 * $ Copyright 2016-YEAR Cypress Semiconductor $
 */



/**************************************************************************//**
* file <mesh_model_dfu.h>
*
* Mesh DFU Model
*
******************************************************************************/
#ifndef __MESH_MODEL_DFU_H__
#define __MESH_MODEL_DFU_H__

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
 * @anchor WICED_BT_MESH_MODEL_ID
 * @name Model identifiers
 * \details The following is the list of Blueoth SIG defined Model IDs that a mesh node can support.
 * @{
 */

#ifdef WICED_BT_MESH_USE_TEMP_OPCODES
#define WICED_BT_MESH_CORE_MODEL_ID_BLOB_TRANSFER_SRV                   0xBF42
#define WICED_BT_MESH_CORE_MODEL_ID_BLOB_TRANSFER_CLNT                  0xBF43

#define WICED_BT_MESH_CORE_MODEL_ID_FW_UPDATE_SRV                       0xBF44
#define WICED_BT_MESH_CORE_MODEL_ID_FW_UPDATE_CLNT                      0xBF45
#define WICED_BT_MESH_CORE_MODEL_ID_FW_DISTRIBUTION_SRV                 0xBF46
#define WICED_BT_MESH_CORE_MODEL_ID_FW_DISTRIBUTION_CLNT                0xBF47
#else
#define WICED_BT_MESH_CORE_MODEL_ID_BLOB_TRANSFER_SRV                   0x1400
#define WICED_BT_MESH_CORE_MODEL_ID_BLOB_TRANSFER_CLNT                  0x1401

#define WICED_BT_MESH_CORE_MODEL_ID_FW_UPDATE_SRV                       0x1402
#define WICED_BT_MESH_CORE_MODEL_ID_FW_UPDATE_CLNT                      0x1403
#define WICED_BT_MESH_CORE_MODEL_ID_FW_DISTRIBUTION_SRV                 0x1404
#define WICED_BT_MESH_CORE_MODEL_ID_FW_DISTRIBUTION_CLNT                0x1405
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
 /* BLOB Transfer */
#define WICED_BT_MESH_OPCODE_BLOB_TRANSFER_GET                                  0xB701
#define WICED_BT_MESH_OPCODE_BLOB_TRANSFER_START                                0xB702
#define WICED_BT_MESH_OPCODE_BLOB_TRANSFER_CANCEL                               0xB703
#define WICED_BT_MESH_OPCODE_BLOB_TRANSFER_STATUS                               0xB704
#define WICED_BT_MESH_OPCODE_BLOB_BLOCK_GET                                     0xB707
#define WICED_BT_MESH_OPCODE_BLOB_BLOCK_START                                   0xB705
#define WICED_BT_MESH_OPCODE_BLOB_PARTIAL_BLOCK_REPORT                          0x4F
#define WICED_BT_MESH_OPCODE_BLOB_BLOCK_STATUS                                  0x7E
#define WICED_BT_MESH_OPCODE_BLOB_CHUNK_TRANSFER                                0x7D
#define WICED_BT_MESH_OPCODE_BLOB_INFO_GET                                      0xB70A
#define WICED_BT_MESH_OPCODE_BLOB_INFO_STATUS                                   0xB70B

/* Firmware Update */
#define WICED_BT_MESH_OPCODE_FW_UPDATE_INFO_GET                                 0xB71B
#define WICED_BT_MESH_OPCODE_FW_UPDATE_INFO_STATUS                              0xB737
#define WICED_BT_MESH_OPCODE_FW_UPDATE_FW_METADATA_CHECK                        0xB738
#define WICED_BT_MESH_OPCODE_FW_UPDATE_FW_METADATA_STATUS                       0xB739
#define WICED_BT_MESH_OPCODE_FW_UPDATE_GET                                      0xB71C
#define WICED_BT_MESH_OPCODE_FW_UPDATE_START                                    0xB71D
#define WICED_BT_MESH_OPCODE_FW_UPDATE_CANCEL                                   0xB71E
#define WICED_BT_MESH_OPCODE_FW_UPDATE_APPLY                                    0xB71F
#define WICED_BT_MESH_OPCODE_FW_UPDATE_STATUS                                   0xB72B

/* Firmware Distribution */
#define WICED_BT_MESH_OPCODE_FW_DISTR_CAPABILITIES_GET                          0xB723
#define WICED_BT_MESH_OPCODE_FW_DISTR_CAPABILITIES_STATUS                       0xB72E
#define WICED_BT_MESH_OPCODE_FW_DISTR_GET                                       0xB724
#define WICED_BT_MESH_OPCODE_FW_DISTR_START                                     0xB72F
#define WICED_BT_MESH_OPCODE_FW_DISTR_SUSPEND                                   0xB73A
#define WICED_BT_MESH_OPCODE_FW_DISTR_CANCEL                                    0xB725
#define WICED_BT_MESH_OPCODE_FW_DISTR_APPLY                                     0xB726
#define WICED_BT_MESH_OPCODE_FW_DISTR_STATUS                                    0xB730
#define WICED_BT_MESH_OPCODE_FW_DISTR_NODES_GET                                 0xB722
#define WICED_BT_MESH_OPCODE_FW_DISTR_NODES_LIST                                0xB72D
#define WICED_BT_MESH_OPCODE_FW_DISTR_NODES_ADD                                 0xB72C
#define WICED_BT_MESH_OPCODE_FW_DISTR_NODES_DELETE_ALL                          0xB720
#define WICED_BT_MESH_OPCODE_FW_DISTR_NODES_STATUS                              0xB721
#define WICED_BT_MESH_OPCODE_FW_DISTR_UPLOAD_GET                                0xB727
#define WICED_BT_MESH_OPCODE_FW_DISTR_UPLOAD_START                              0xB731
#define WICED_BT_MESH_OPCODE_FW_DISTR_UPLOAD_OOB_START                          0xB732
#define WICED_BT_MESH_OPCODE_FW_DISTR_UPLOAD_CANCEL                             0xB728
#define WICED_BT_MESH_OPCODE_FW_DISTR_UPLOAD_STATUS                             0xB733
#define WICED_BT_MESH_OPCODE_FW_DISTR_FW_GET                                    0xB734
#define WICED_BT_MESH_OPCODE_FW_DISTR_FW_GET_BY_INDEX                           0xB729
#define WICED_BT_MESH_OPCODE_FW_DISTR_FW_DELETE                                 0xB735
#define WICED_BT_MESH_OPCODE_FW_DISTR_FW_DELETE_ALL                             0xB72A
#define WICED_BT_MESH_OPCODE_FW_DISTR_FW_STATUS                                 0xB736
#else
 /* BLOB Transfer */
#define WICED_BT_MESH_OPCODE_BLOB_TRANSFER_GET                                  0x8300
#define WICED_BT_MESH_OPCODE_BLOB_TRANSFER_START                                0x8301
#define WICED_BT_MESH_OPCODE_BLOB_TRANSFER_CANCEL                               0x8302
#define WICED_BT_MESH_OPCODE_BLOB_TRANSFER_STATUS                               0x8303
#define WICED_BT_MESH_OPCODE_BLOB_BLOCK_GET                                     0x8305
#define WICED_BT_MESH_OPCODE_BLOB_BLOCK_START                                   0x8304
#define WICED_BT_MESH_OPCODE_BLOB_PARTIAL_BLOCK_REPORT                          0x65
#define WICED_BT_MESH_OPCODE_BLOB_BLOCK_STATUS                                  0x67
#define WICED_BT_MESH_OPCODE_BLOB_CHUNK_TRANSFER                                0x66
#define WICED_BT_MESH_OPCODE_BLOB_INFO_GET                                      0x8306
#define WICED_BT_MESH_OPCODE_BLOB_INFO_STATUS                                   0x8307

 /* Firmware Update */
#define WICED_BT_MESH_OPCODE_FW_UPDATE_INFO_GET                                 0x8308
#define WICED_BT_MESH_OPCODE_FW_UPDATE_INFO_STATUS                              0x8324
#define WICED_BT_MESH_OPCODE_FW_UPDATE_FW_METADATA_CHECK                        0x8325
#define WICED_BT_MESH_OPCODE_FW_UPDATE_FW_METADATA_STATUS                       0x8326
#define WICED_BT_MESH_OPCODE_FW_UPDATE_GET                                      0x8309
#define WICED_BT_MESH_OPCODE_FW_UPDATE_START                                    0x830A
#define WICED_BT_MESH_OPCODE_FW_UPDATE_CANCEL                                   0x830B
#define WICED_BT_MESH_OPCODE_FW_UPDATE_APPLY                                    0x830C
#define WICED_BT_MESH_OPCODE_FW_UPDATE_STATUS                                   0x8318

 /* Firmware Distribution */
#define WICED_BT_MESH_OPCODE_FW_DISTR_CAPABILITIES_GET                          0x8310
#define WICED_BT_MESH_OPCODE_FW_DISTR_CAPABILITIES_STATUS                       0x831B
#define WICED_BT_MESH_OPCODE_FW_DISTR_GET                                       0x8311
#define WICED_BT_MESH_OPCODE_FW_DISTR_START                                     0x831C
#define WICED_BT_MESH_OPCODE_FW_DISTR_SUSPEND                                   0x8327
#define WICED_BT_MESH_OPCODE_FW_DISTR_CANCEL                                    0x8312
#define WICED_BT_MESH_OPCODE_FW_DISTR_APPLY                                     0x8313
#define WICED_BT_MESH_OPCODE_FW_DISTR_STATUS                                    0x831D
#define WICED_BT_MESH_OPCODE_FW_DISTR_NODES_GET                                 0x830F
#define WICED_BT_MESH_OPCODE_FW_DISTR_NODES_LIST                                0x831A
#define WICED_BT_MESH_OPCODE_FW_DISTR_NODES_ADD                                 0x8319
#define WICED_BT_MESH_OPCODE_FW_DISTR_NODES_DELETE_ALL                          0x830D
#define WICED_BT_MESH_OPCODE_FW_DISTR_NODES_STATUS                              0x830E
#define WICED_BT_MESH_OPCODE_FW_DISTR_UPLOAD_GET                                0x8314
#define WICED_BT_MESH_OPCODE_FW_DISTR_UPLOAD_START                              0x831E
#define WICED_BT_MESH_OPCODE_FW_DISTR_UPLOAD_OOB_START                          0x831F
#define WICED_BT_MESH_OPCODE_FW_DISTR_UPLOAD_CANCEL                             0x8315
#define WICED_BT_MESH_OPCODE_FW_DISTR_UPLOAD_STATUS                             0x8320
#define WICED_BT_MESH_OPCODE_FW_DISTR_FW_GET                                    0x8321
#define WICED_BT_MESH_OPCODE_FW_DISTR_FW_GET_BY_INDEX                           0x8316
#define WICED_BT_MESH_OPCODE_FW_DISTR_FW_DELETE                                 0x8322
#define WICED_BT_MESH_OPCODE_FW_DISTR_FW_DELETE_ALL                             0x8317
#define WICED_BT_MESH_OPCODE_FW_DISTR_FW_STATUS                                 0x8323
#endif

/** @} WICED_BT_MESH_MODEL_OPCODE */

/**
 * NOTE: This is preliminary implementation subject to change
 * @anchor MESH_FW_UPDATE_PHASES
 * @name Mesh Firmware Update Phases
 * @{
 */
#define WICED_BT_MESH_FW_UPDATE_PHASE_IDLE                  0x00 /**< No firmware transfer is in progress */
#define WICED_BT_MESH_FW_UPDATE_PHASE_TRANSFER_ERROR        0x01 /**< Firmware transfer was not completed */
#define WICED_BT_MESH_FW_UPDATE_PHASE_TRANSFER_ACTIVE       0x02 /**< Firmware transfer is in progress */
#define WICED_BT_MESH_FW_UPDATE_PHASE_VERIFICATION_ACTIVE   0x03 /**< Verification of the firmware image is in progress */
#define WICED_BT_MESH_FW_UPDATE_PHASE_VERIFICATION_SUCCESS  0x04 /**< Firmware image verification succeeded */
#define WICED_BT_MESH_FW_UPDATE_PHASE_VERIFICATION_FAILED   0x05 /**< Firmware image verification failed */
#define WICED_BT_MESH_FW_UPDATE_PHASE_APPLY_ACTIVE          0x06 /**< Firmware applying is in progress */
#define WICED_BT_MESH_FW_UPDATE_PHASE_TRANSFER_CANCELLED    0x07 /**< Firmware transfer has been cancelled */
#define WICED_BT_MESH_FW_UPDATE_PHASE_APPLY_SUCCESS         0x08 /**< Firmware image apply succeeded */
#define WICED_BT_MESH_FW_UPDATE_PHASE_APPLY_FAILED          0x09 /**< Firmware image apply failed */
#define WICED_BT_MESH_FW_UPDATE_PHASE_UNKNOWN               0x0A /**< Phase of a node was not retrieved yet */
/** @} MESH_FW_UPDATE_PHASES */

/**
 * NOTE: This is preliminary implementation subject to change
 * @anchor MESH_FW_UPDATE_ADDITIONAL_INFO
 * @name Mesh Firmware Update Additional Information
 * @{
 */
#define WICED_BT_MESH_FW_UPDATE_PROVISIONING_NEEDED         0x01 /**< The device shall be in an unprovisioned state after new firmware is applied (possibly because of new models added). */
#define WICED_BT_MESH_FW_UPDATE_COMPOSITION_CHANGE_EXPECTED 0x02 /**< The device is expected to expose page 0x80 of the Composition Data state, and execution of either the Node Composition Refresh procedure or Node Removal procedure is needed. */
/** @} MESH_FW_UPDATE_ADDITIONAL_INFO */

/**
 * NOTE: This is preliminary implementation subject to change
 * @anchor MESH_FW_UPDATE_OOB_SUPPORT
 * @name Mesh Firmware Update Out-of-Band Retrieval Supported
 * @{
 */
#define WICED_BT_MESH_FW_UPDATE_OOB_SUPPORTED               0x01 /**< The Distributor supports firmware retrieval using Out-of-Band mechanism. */
/** @} MESH_FW_UPDATE_OOB_SUPPORT */

/**
 * NOTE: This is preliminary implementation subject to change
 * @anchor MESH_FW_UPDATE_URI_SCHEME
 * @name Mesh Firmware Update URI Schemes
 * @{
 */
#define WICED_BT_MESH_FW_UPDATE_URI_SCHEME_HTTP             0x16
#define WICED_BT_MESH_FW_UPDATE_URI_SCHEME_HTTPS            0x17
/** @} MESH_FW_UPDATE_SUPPORTED_URI_SCHEME */

/**
 * NOTE: This is preliminary implementation subject to change
 * @anchor MESH_FW_DISTRIBUTION_PHASES
 * @name Mesh Firmware Distribution Phases
 * @{
 */
#define WICED_BT_MESH_FW_DISTR_PHASE_IDLE                   0x00 /**< Distribution is not active. */
#define WICED_BT_MESH_FW_DISTR_PHASE_TRANSFER_ACTIVE        0x01 /**< Firmware transfer is in progress. */
#define WICED_BT_MESH_FW_DISTR_PHASE_TRANSFER_SUCCESS       0x02 /**< The Transfer BLOB procedure has completed successfully. */
#define WICED_BT_MESH_FW_DISTR_PHASE_APPLY_ACTIVE           0x03 /**< Firmware applying is in progress. */
#define WICED_BT_MESH_FW_DISTR_PHASE_COMPLETED              0x04 /**< At least one Updating Node was updated successfully. */
#define WICED_BT_MESH_FW_DISTR_PHASE_FAILED                 0x05 /**< No Updating Nodes were updated successfully. */
#define WICED_BT_MESH_FW_DISTR_PHASE_CANCEL_ACTIVE          0x06 /**< Cancelling the firmware transfer. */
#define WICED_BT_MESH_FW_DISTR_PHASE_SUSPENDED              0x07 /**< The Transfer BLOB procedure is suspended. */
/** @} MESH_FW_DISTRIBUTION_PHASES */

/**
 * NOTE: This is preliminary implementation subject to change
 * @anchor MESH_FW_UPDATE_POLICY
 * @name Mesh Firmware Update Policy
 * @{
 */
#define WICED_BT_MESH_FW_UPDATE_POLICY_VERIFY_ONLY          0x00 /**< Do not send a Firmware Update Apply when the firmware transfer is completed. Wait for the Initiator to send the Firmware Distribution Apply message before sending the Firmware Update Apply message. */
#define WICED_BT_MESH_FW_UPDATE_POLICY_VERIFY_AND_APPLY     0x01 /**< Send a Firmware Update Apply message when the firmware transfer is completed. */
/** @} MESH_FW_UPDATE_POLICY */

/**
 * NOTE: This is preliminary implementation subject to change
 * @anchor MESH_FW_TRANSFER_MODE
 * @name Mesh Firmware Transfer Mode of the BLOB
 * @{
 */
#define WICED_BT_MESH_FW_TRANSFER_MODE_PUSH                 0x01 /**< Push BLOB transfer mode */
#define WICED_BT_MESH_FW_TRANSFER_MODE_PULL                 0x02 /**< Pull BLOB transfer mode */
 /** @} MESH_FW_TRANSFER_MODE */

/**
 * NOTE: This is preliminary implementation subject to change
 * @anchor MESH_FW_DISTRIBUTION_ADDRESS_TYPE
 * @name Distribution address type
 * @{
 */
#define WICED_BT_MESH_FW_DISTRIBUTION_ADDRESS_GROUP         0x00 /**< The Distribution Multicast Address field contains a group address, or the unassigned address, and is 16 bits in size */
#define WICED_BT_MESH_FW_DISTRIBUTION_ADDRESS_VIRTUAL       0x01 /**< The Distribution Multicast Address field contains the Label UUID for a virtual address, and is 128 bits in size */
 /** @} MESH_FW_DISTRIBUTION_ADDRESS_TYPE */

/**
 * NOTE: This is preliminary implementation subject to change
 * @anchor MESH_FW_UPLOAD_PHASES
 * @name Mesh Firmware Upload Phases
 * @{
 */
#define WICED_BT_MESH_FW_UPLOAD_PHASE_IDLE                  0x00 /**< No firmware upload is in progress. */
#define WICED_BT_MESH_FW_UPLOAD_PHASE_TRANSFER_ACTIVE       0x01 /**< Firmware upload using the BLOB transfer is in progress. */
#define WICED_BT_MESH_FW_UPLOAD_PHASE_TRANSFER_ERROR        0x02 /**< Firmware upload failed because of a BLOB Transfer protocol error, such as a timeout. */
#define WICED_BT_MESH_FW_UPLOAD_PHASE_TRANSFER_SUCCESS      0x03 /**< Firmware uploaded successfully. */
/** @} MESH_FW_UPLOAD_PHASES */

/**
* NOTE: This is preliminary implementation subject to change
* @anchor MESH_FW_UPLOAD_TYPE
* @name Mesh Firmware Upload type
* @{
*/
#define WICED_BT_MESH_FW_UPLOAD_TYPE_IN_BAND                0x0  /**< The firmware upload is executed in-band. */
#define WICED_BT_MESH_FW_UPLOAD_TYPE_OUT_OF_BAND            0x1  /**< The firmware upload is executed out-of-band. */
/** @} MESH_FW_TRANSFER_MODE */

/**
 * NOTE: This is preliminary implementation subject to change
 * @anchor MESH_FW_UPDATE_STATUS_CODE
 * @name Mesh Firmware Update Status Code Definitions
 * @{
 */
#define WICED_BT_MESH_FW_UPDATE_STATUS_SUCCESS              0x00
#define WICED_BT_MESH_FW_UPDATE_STATUS_OUT_OF_RESOURCES     0x01
#define WICED_BT_MESH_FW_UPDATE_STATUS_WRONG_PHASE          0x02
#define WICED_BT_MESH_FW_UPDATE_STATUS_INTERNAL_ERROR       0x03
#define WICED_BT_MESH_FW_UPDATE_STATUS_WRONG_FW_INDEX       0x04
#define WICED_BT_MESH_FW_UPDATE_STATUS_VALIDATION_FAILED    0x05
#define WICED_BT_MESH_FW_UPDATE_STATUS_TEMP_UNAVAIL         0x06
#define WICED_BT_MESH_FW_UPDATE_STATUS_BLOB_TRANS_BUSY      0x07
/** @} MESH_FW_UPDATE_STATUS_CODE */

/**
 * NOTE: This is preliminary implementation subject to change
 * @anchor MESH_FW_DISTRIBUTION_STATUS_CODE
 * @name Mesh Firmware Distribution Status Code Definitions
 * @{
 */
#define WICED_BT_MESH_FW_DISTR_STATUS_SUCCESS               0x00
#define WICED_BT_MESH_FW_DISTR_STATUS_OUT_OF_RESOURCES      0x01
#define WICED_BT_MESH_FW_DISTR_STATUS_WRONG_PHASE           0x02
#define WICED_BT_MESH_FW_DISTR_STATUS_INTERNAL_ERROR        0x03
#define WICED_BT_MESH_FW_DISTR_STATUS_FW_NOT_FOUND          0x04
#define WICED_BT_MESH_FW_DISTR_STATUS_INVALID_APPKEY_INDEX  0x05
#define WICED_BT_MESH_FW_DISTR_STATUS_NODES_LIST_EMPTY      0x06
#define WICED_BT_MESH_FW_DISTR_STATUS_BUSY_DISTR            0x07
#define WICED_BT_MESH_FW_DISTR_STATUS_BUSY_UPLOAD           0x08
#define WICED_BT_MESH_FW_DISTR_STATUS_URI_NOT_SUPPORTED     0x09
#define WICED_BT_MESH_FW_DISTR_STATUS_URI_MALFORMED         0x0A
#define WICED_BT_MESH_FW_DISTR_STATUS_URI_UNREACHABLE       0x0B
#define WICED_BT_MESH_FW_DISTR_STATUS_NO_NEW_FW             0x0C
#define WICED_BT_MESH_FW_DISTR_STATUS_SUSPEND_FAILED        0x0D
#define WICED_BT_MESH_FW_DISTR_STATUS_NOT_SUPPORTED         0xFE
 /** @} MESH_FW_DISTRIBUTION_STATUS_CODE */

#define WICED_BT_MESH_FW_DISTR_FW_INDEX_NOT_PRESENT         0xFFFF

/**
 * NOTE: This is preliminary implementation subject to change
 * @anchor MESH_BLOB_TRANSFER_PHASE_STATES
 * @name Mesh BLOB Transfer Phase States
 * @{
 */
#define WICED_BT_MESH_BLOB_TRANSFER_PHASE_INACTIVE          0x00 /**< Receive BLOB Procedure is not ready and has not been given an Expected BLOB ID. */
#define WICED_BT_MESH_BLOB_TRANSFER_PHASE_IDLE              0x01 /**< BLOB Transfer Server is ready to receive the BLOB identified by the Expected BLOB ID. */
#define WICED_BT_MESH_BLOB_TRANSFER_PHASE_WAIT_NEXT_BLOCK   0x02 /**< BLOB Transfer Server is waiting for the next block of data. */
#define WICED_BT_MESH_BLOB_TRANSFER_PHASE_WAIT_NEXT_CHUNK   0x03 /**< BLOB Transfer Server is waiting for the next chunk of data. */
#define WICED_BT_MESH_BLOB_TRANSFER_PHASE_COMPLETE          0x04 /**< Receive BLOB Procedure is complete. The server is waiting for a higher-layer application or model to set the server into the Idle or Inactive state. */
#define WICED_BT_MESH_BLOB_TRANSFER_PHASE_SUSPENDED         0x05 /**< Receive BLOB Procedure has been timed out. */
#define WICED_BT_MESH_BLOB_TRANSFER_PHASE_UNKNOWN           0x06 /**< The transfer phase on the server was not retrieved yet */
/** @} MESH_BLOB_TRANSFER_PHASE_STATES */

/**
 * NOTE: This is preliminary implementation subject to change
 * @anchor MESH_BLOB_TRANSFER_BLOCK_SIZE_LOG
 * @name Mesh BLOB Transfer Block Size Log range
 * @{
 */
#define WICED_BT_MESH_BLOB_TRANSFER_BLOCK_SIZE_LOG_MIN      0x06
#define WICED_BT_MESH_BLOB_TRANSFER_BLOCK_SIZE_LOG_MAX      0x20
/** @} MESH_BLOB_TRANSFER_BLOCK_SIZE_LOG */

/**
 * NOTE: This is preliminary implementation subject to change
 * @anchor MESH_BLOB_TRANSFER_STATUS_CODE
 * @name Mesh BLOB Transfer Status Code Definitions
 * @{
 */
#define WICED_BT_MESH_BLOB_TRANSFER_STATUS_SUCCESS                  0x00 /**< The message was processed successfully. */
#define WICED_BT_MESH_BLOB_TRANSFER_STATUS_INVALID_BLOCK_NUMBER     0x01 /**< The Block Number field value is not within range. */
#define WICED_BT_MESH_BLOB_TRANSFER_STATUS_INVALID_BLOCK_SIZE       0x02 /**< The block size is lower than the size represented by Min Block Size Log, or the block size is higher than the size represented by Max Block Size Log. */
#define WICED_BT_MESH_BLOB_TRANSFER_STATUS_INVALID_CHUNK_SIZE       0x03 /**< Chunk size exceeds the size represented by Max Chunk Size, or the number of chunks exceeds the number specified by Max Chunks Number. */
#define WICED_BT_MESH_BLOB_TRANSFER_STATUS_WRONG_PHASE              0x04 /**< The model is in a state where it cannot process the message. */
#define WICED_BT_MESH_BLOB_TRANSFER_STATUS_INVALID_PARAMETER        0x05 /**< A parameter value in the message cannot be accepted. */
#define WICED_BT_MESH_BLOB_TRANSFER_STATUS_WRONG_BLOB_ID            0x06 /**< The requested BLOB ID is not expected. */
#define WICED_BT_MESH_BLOB_TRANSFER_STATUS_BLOB_TOO_LARGE           0x07 /**< Not enough memory space to receive the BLOB. */
#define WICED_BT_MESH_BLOB_TRANSFER_STATUS_UNSUPPORTED_MODE         0x08 /**< The transfer mode is not supported. */
#define WICED_BT_MESH_BLOB_TRANSFER_STATUS_INTERNAL_ERROR           0x09 /**< An internal error occurred on the node. */
#define WICED_BT_MESH_BLOB_TRANSFER_STATUS_INFO_UNAVAIL             0x0A /**< The requested information cannot be provided while the server is in the current phase. */
#define WICED_BT_MESH_BLOB_TRANSFER_STATUS_INVALID_RESPONSE         0xFF /**< Invalid response. */
/** @} MESH_BLOB_TRANSFER_STATUS_CODE */

/**
 * NOTE: This is preliminary implementation subject to change
 * @anchor MESH_BLOB_TRANSFER_BLOCK_CHUNKS_MISSING
 * @name Mesh BLOB Transfer Block Missing Chunks
 * @{
 */
#define WICED_BT_MESH_BLOB_TRANSFER_BLOCK_ALL_CHUNKS_MISSING        0x00
#define WICED_BT_MESH_BLOB_TRANSFER_BLOCK_NO_CHUNK_MISSING          0x01
#define WICED_BT_MESH_BLOB_TRANSFER_BLOCK_SOME_CHUNKS_MISSING       0x02
#define WICED_BT_MESH_BLOB_TRANSFER_BLOCK_ENCODED_MISSING_CHUNKS    0x03
/** @} MESH_BLOB_TRANSFER_BLOCK_CHUNKS_MISSING */

#define WICED_BT_MESH_MAX_BLOB_POLL_INTERVAL                        30      /**< T_MBPI = 30 seconds */

#define WICED_BT_MESH_BLOB_TRANSFER_BLOB_ID_LEN                     8

/*---------------------------------------------------------------------------*/
/*                        wiced_bt_mesh_models.h                             */
/*---------------------------------------------------------------------------*/

/**
 * NOTE: This is preliminary implementation subject to change
 * @anchor FW_DISTRIBUTION_EVENT
 * @name Definition for messages exchanged between an app and FW Distribution Model
 * @{ */
#define WICED_BT_MESH_FW_DISTRIBUTION_STATUS                153  /**< Status change event and Get Status response */
#define WICED_BT_MESH_FW_UPDATE_INFO_STATUS                 154  /**< Get FW Update Info response */
#define WICED_BT_MESH_FW_UPDATE_METADATA_STATUS             155  /**< Check FW Update metadata response */
/** @} FW_DISTRIBUTION_EVENT */


#define WICED_BT_MESH_MODEL_FW_UPDATE_SERVER \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_BLOB_TRANSFER_SRV, wiced_bt_mesh_model_blob_transfer_server_message_handler, NULL, NULL }, \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_FW_UPDATE_SRV, wiced_bt_mesh_model_fw_update_server_message_handler, NULL, NULL }

#define WICED_BT_MESH_MODEL_FW_DISTRIBUTOR \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_FW_DISTRIBUTION_SRV, wiced_bt_mesh_model_fw_distribution_server_message_handler, NULL, NULL }, \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_FW_UPDATE_CLNT, wiced_bt_mesh_model_fw_update_client_message_handler, NULL, NULL }, \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_BLOB_TRANSFER_SRV, wiced_bt_mesh_model_blob_transfer_server_message_handler, NULL, NULL }, \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_BLOB_TRANSFER_CLNT, wiced_bt_mesh_model_blob_transfer_client_message_handler, NULL, NULL }

#define WICED_BT_MESH_MODEL_FW_DISTRIBUTOR_UPDATE_SERVER \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_FW_DISTRIBUTION_SRV, wiced_bt_mesh_model_fw_distribution_server_message_handler, NULL, NULL }, \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_FW_UPDATE_CLNT, wiced_bt_mesh_model_fw_update_client_message_handler, NULL, NULL }, \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_FW_UPDATE_SRV, wiced_bt_mesh_model_fw_update_server_message_handler, NULL, NULL }, \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_BLOB_TRANSFER_CLNT, wiced_bt_mesh_model_blob_transfer_client_message_handler, NULL, NULL }, \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_BLOB_TRANSFER_SRV, wiced_bt_mesh_model_blob_transfer_server_message_handler, NULL, NULL }

#define WICED_BT_MESH_MODEL_FW_STANDALONE_UPDATER \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_FW_DISTRIBUTION_CLNT, wiced_bt_mesh_model_fw_provider_message_handler, NULL, NULL }, \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_FW_DISTRIBUTION_SRV, wiced_bt_mesh_model_fw_distribution_server_message_handler, NULL, NULL }, \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_FW_UPDATE_CLNT, wiced_bt_mesh_model_fw_update_client_message_handler, NULL, NULL }, \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_BLOB_TRANSFER_CLNT, wiced_bt_mesh_model_blob_transfer_client_message_handler, NULL, NULL }

#define WICED_BT_MESH_MODEL_FW_DISTRIBUTION_CLIENT \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_FW_DISTRIBUTION_CLNT, wiced_bt_mesh_model_fw_provider_message_handler, NULL, NULL }

#define WICED_BT_MESH_MODEL_FW_UPDATE_CLIENT \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_FW_UPDATE_CLNT, wiced_bt_mesh_model_fw_update_client_message_handler, NULL, NULL }

#define WICED_BT_MESH_MODEL_BLOB_TRANSFER_CLIENT \
    { MESH_COMPANY_ID_BT_SIG, WICED_BT_MESH_CORE_MODEL_ID_BLOB_TRANSFER_CLNT, wiced_bt_mesh_model_blob_transfer_client_message_handler, NULL, NULL }


/* DFU opcodes */
typedef struct
{
    uint16_t    blob_transfer_get;
    uint16_t    blob_transfer_start;
    uint16_t    blob_transfer_cancel;
    uint16_t    blob_transfer_status;
    uint16_t    blob_block_get;
    uint16_t    blob_block_start;
    uint16_t    blob_partial_block_report;
    uint16_t    blob_block_status;
    uint16_t    blob_chunk_transfer;
    uint16_t    blob_info_get;
    uint16_t    blob_info_status;

    uint16_t    fw_update_info_get;
    uint16_t    fw_update_info_status;
    uint16_t    fw_update_metadata_check;
    uint16_t    fw_update_metadata_status;
    uint16_t    fw_update_get;
    uint16_t    fw_update_start;
    uint16_t    fw_update_cancel;
    uint16_t    fw_update_apply;
    uint16_t    fw_update_status;

    uint16_t    fw_distr_get;
    uint16_t    fw_distr_start;
    uint16_t    fw_distr_suspend;
    uint16_t    fw_distr_cancel;
    uint16_t    fw_distr_apply;
    uint16_t    fw_distr_status;
    uint16_t    fw_distr_nodes_get;
    uint16_t    fw_distr_nodes_list;
    uint16_t    fw_distr_nodes_add;
    uint16_t    fw_distr_nodes_delete_all;
    uint16_t    fw_distr_nodes_status;
    uint16_t    fw_distr_capabilities_get;
    uint16_t    fw_distr_capabilities_status;
    uint16_t    fw_distr_upload_get;
    uint16_t    fw_distr_upload_start;
    uint16_t    fw_distr_upload_oob_start;
    uint16_t    fw_distr_upload_cancel;
    uint16_t    fw_distr_upload_status;
    uint16_t    fw_distr_fw_get;
    uint16_t    fw_distr_fw_status;
    uint16_t    fw_distr_fw_get_by_index;
    uint16_t    fw_distr_fw_delete;
    uint16_t    fw_distr_fw_delete_all;
} mesh_dfu_opcodes_t;

/* Firmware distribution firmware ID structure */
typedef PACKED struct
{
#define MESH_CYPRESS_FW_ID_LEN              11
    uint8_t  fw_id_len;                                 /**< length of firmware id stored in fw_id */
#define WICED_BT_MESH_MIN_FIRMWARE_ID_LEN   2           /**< At minimum FW ID shall contain 2 bytes of company ID */
#define WICED_BT_MESH_MAX_FIRMWARE_ID_LEN   108         /**< MAX len of firmware id */
    uint8_t  fw_id[WICED_BT_MESH_MAX_FIRMWARE_ID_LEN];  /**< firmware id. maximum length is defined by WICED_BT_MESH_MAX_FIRMWARE_ID_LEN */
} mesh_dfu_fw_id_t;

/* Firmware distribution metadata structure */
typedef PACKED struct
{
    uint8_t  len;                                       /**< length of firmware id stored in fw_id */
#define WICED_BT_MESH_MAX_METADATA_LEN   255            /**< MAX len of metadata is 255 bytes */
    uint8_t  data[WICED_BT_MESH_MAX_METADATA_LEN];      /**< metadata */
} mesh_dfu_metadata_t;

/* Firmware update URI structure */
typedef PACKED struct
{
    uint8_t  len;                                       /**< length of URI */
#define WICED_BT_MESH_MAX_UPDATE_URI_LEN    255         /**< MAX len of Update URI */
    uint8_t  uri[WICED_BT_MESH_MAX_UPDATE_URI_LEN];     /**< URI */
} mesh_dfu_uri_t;

/* BLOB client configuration */
typedef PACKED struct
{
    uint8_t  max_block_size_log;
    uint16_t chunk_size_unicast;
    uint16_t chunk_size_multicast;
    uint16_t max_chunks_number;
    uint16_t max_mtu_size;
} mesh_dfu_blob_client_config_t;

/* BLOB server configuration */
typedef PACKED struct
{
    uint8_t  min_block_size_log;
    uint8_t  max_block_size_log;
    uint16_t max_chunks_number;
    uint16_t max_chunk_size;
    uint16_t chunk_size_pull_mode;
    uint16_t max_mtu_size;
    uint32_t max_blob_size;
    uint16_t max_buffer_size;
} mesh_dfu_blob_server_config_t;

#define MESH_MAX_URI_SCHEME_NAMES           6

/* Distributor configuration */
typedef PACKED struct
{
    uint16_t max_buffer_size;
    uint32_t max_fw_storage_space;
    uint8_t  oob_supported;
    uint8_t  uri_scheme_num;
    uint8_t  uri_scheme_names[MESH_MAX_URI_SCHEME_NAMES];
} mesh_dfu_distributor_config_t;

typedef PACKED struct
{
    mesh_dfu_fw_id_t fw_id;
    char *uri;
} mesh_dfu_fw_info_t;

/* Firmware update server configuration */
typedef PACKED struct
{
    uint8_t  fw_info_list_size;
    mesh_dfu_fw_info_t *fw_info_list;
} mesh_dfu_update_server_config_t;

/* DFU configuration */
typedef struct
{
    mesh_dfu_blob_client_config_t   *p_blob_client_cfg;
    mesh_dfu_blob_server_config_t   *p_blob_server_cfg;
    mesh_dfu_distributor_config_t   *p_distributor_cfg;
    mesh_dfu_update_server_config_t *p_update_server_cfg;
} mesh_dfu_config_t;

/**
 * \brief This callback is called by the Mesh Models library to retrieve current app's firmware ID
 *
 * @param       p_fw_id Space to return the current firmware ID
 *
 * @return      wiced_bool_t
 */
typedef wiced_bool_t(*mesh_dfu_get_active_fw_id_t)(mesh_dfu_fw_id_t *p_fw_id);
/**
 * \brief This callback is called by the Mesh Models library to check if received metadata matches
 *
 * @param       index       Index of the firmware image the metadata is to be checked with
 * @param       p_metadata  The metadata
 *
 * @return      wiced_bool_t
 */
typedef wiced_bool_t(*mesh_dfu_metadata_check_t)(uint8_t index, mesh_dfu_metadata_t *p_metadata);
/**
 * \brief This callback is called by the Mesh Models library to verify the received firmware image
 *
 * @param       index       Index of the firmware image received
 * @param       fw_size     Size of the firmware image received
 * @param       p_metadata  The metadata
 *
 * @return      wiced_bool_t
 */
typedef wiced_bool_t(*mesh_dfu_fw_verify_t)(uint8_t index, uint32_t fw_size, mesh_dfu_metadata_t *p_metadata);
/**
 * \brief This callback is called by the Mesh Models library to apply the received firmware image
 *
 * @param       index       Index of the firmware image to be applied
 *
 * @return      wiced_bool_t
 */
typedef wiced_bool_t(*mesh_dfu_fw_apply_t)(uint8_t index);
/**
 * \brief This callback is called by the Mesh Models library to start OOB firmware upload
 * \details Out-Of-Band firmware upload uses HTTP to retrieve firmware image from a server. It construct the
 * Request URI with Update URI and FW ID input, send an HTTP GET request with the Request URI. The HTTP server
 * returns a firmware archive which contains FW image, FW ID, and metadata.
 *
 * @param       p_uri   Update URI
 * @param       p_fw_id Current firmware ID of the device that needs to be updated
 *
 * @return      MESH_FW_DISTRIBUTION_STATUS_CODE
 */
typedef uint8_t (*mesh_dfu_fw_upload_oob_t)(mesh_dfu_uri_t *p_uri, mesh_dfu_fw_id_t *p_fw_id);

typedef struct
{
    mesh_dfu_get_active_fw_id_t p_active_fw_id_cb;
    mesh_dfu_metadata_check_t   p_metadata_check_cb;
    mesh_dfu_fw_verify_t        p_fw_verify_cb;
    mesh_dfu_fw_apply_t         p_fw_apply_cb;
    mesh_dfu_fw_upload_oob_t    p_fw_upload_oob_cb;
} mesh_dfu_callbacks_t;

/**
* \brief Firmware update set opcodes
* \details Application call this function to pass DFU opcodes to the library
*
* @param       p_opcodes    DFU opcodes defined by SIG.
*
* @return      None.
*/
void wiced_bt_mesh_model_fw_update_set_opcodes(const mesh_dfu_opcodes_t *p_opcodes);

/**
* \brief Firmware update init
* \details Application call this function to initialize DFU models in the library
*
* @param       p_dfu_config     DFU configuration.
* @param       p_dfu_callbacks  DFU callbacks.
*
* @return      None.
*/
void wiced_bt_mesh_model_fw_update_init(const mesh_dfu_config_t *p_dfu_config, const mesh_dfu_callbacks_t *p_dfu_callbacks);

/**
 * NOTE: This is preliminary implementation subject to change
 * @addtogroup  wiced_bt_mesh_fw_distribution_server   Mesh Firmware Distribution Server
 * @ingroup     wiced_bt_mesh_models
 *
 * The AIROC Mesh Firmware Distribution Server module provides a way for an application to implement a functionality for the devices that support Firmware Distribution.
 *
 * TBD
 *
 * @{
 */

/**
* \brief Firmware Distribution Server Message Handler
* \details The Mesh Core library calls this function for each message received.  The function returns WICED_TRUE if the message is destined for this specific model and successfully processed, and returns WICED_FALSE otherwise. FW Distribution Server.
* The function parses the message and if appropriate calls the parent back to perform functionality.
*
* @param       p_event Mesh event with information about received message.
* @param       p_data Pointer to the data portion of the message
* @param       data_len Length of the data in the message
*
* @return      WICED_TRUE if the message is for this company ID/Model/Element Index combination, WICED_FALSE otherwise.
*/
wiced_bool_t wiced_bt_mesh_model_fw_distribution_server_message_handler(wiced_bt_mesh_event_t *p_event, uint8_t *p_data, uint16_t data_len);

/**
 * \brief FW Distribution Server initialization
 *
 * @param       None
 *
 * @return      None
 */
void wiced_bt_mesh_model_fw_distribution_server_init(void);

/**
 * \brief This callback is called by the Mesh Models library to start OOB firmware upload
 * \details Out-Of-Band firmware upload uses HTTP to retrieve firmware image from a server. It construct the
 * Request URI with Update URI and FW ID input, send an HTTP GET request with the Request URI. The HTTP server
 * returns a firmware archive which contains FW image, FW ID, and metadata.
 *
 * @param       p_uri   Update URI
 * @param       p_fw_id Current firmware ID of the device that needs to be updated
 *
 * @return      MESH_FW_DISTRIBUTION_STATUS_CODE
 */
typedef uint8_t (wiced_bt_mesh_fw_upload_oob_callback_t)(mesh_dfu_uri_t *p_uri, mesh_dfu_fw_id_t *p_fw_id);

/* @} wiced_bt_mesh_fw_distribution_server */

/**
* FW Distribution Distributor Capabilities
*/
typedef struct
{
    uint16_t max_node_list_size;                            /**< Maximum possible number of entries on the list of nodes */
    uint16_t max_firmware_list_size;                        /**< Maximum possible number of entries on the list of firmwares */
    uint32_t max_firmware_size;                             /**< Maximum size of one firmware in octets */
    uint32_t max_upload_space;                              /**< Size of the Distributor’s storage in octets */
    uint32_t remaining_upload_space;                        /**< Available size in the storage on the Distributor in octets */
    uint8_t  oob_supported;                                 /**< Value of the Out-of-Band Retrieval Supported state */
    uint8_t  uri_scheme_num;                                /**< Number of Supported URI Scheme Names */
    uint8_t  uri_scheme_names[MESH_MAX_URI_SCHEME_NAMES];   /**< Value of the Supported URI Scheme Names state */
} wiced_bt_mesh_fw_distr_caps_t;

#define WICED_BT_MESH_NODES_ADD_ENTRY_SIZE                  3

/**
* FW Distribution Updating Node Item
*/
typedef struct
{
    unsigned int address : 15;                              /**< 15 least significant bits of a unicast address of the Updating Node */
    unsigned int retrieved_update_phase : 4;                /**< Retrieved Update Phase state of the Updating Node */
    unsigned int update_status : 3;                         /**< Status of the last operation with the Firmware Update Server model */
    unsigned int transfer_status : 4;                       /**< Status of the last operation with the BLOB Transfer server model */
    unsigned int transfer_progress : 6;                     /**< Percentage calculated as (2 * Transfer Progress) */
    unsigned int update_fw_index : 8;                       /**< Index of the firmware image on the Firmware Information List state that is being updated */
} wiced_bt_mesh_fw_updating_node_item_t;

#define WICED_BT_MESH_UPDATING_NODE_ITEM_SIZE               5

/**
* FW Distribution Multicast Address
*/
typedef struct
{
    uint8_t  type;
    union {
        uint16_t group_address;
        uint8_t  virtual_address[16];
    } addr;
} wiced_bt_mesh_fw_distr_multi_addr_t;


/**
 * NOTE: This is preliminary implementation subject to change
 * @addtogroup  wiced_bt_mesh_fw_update_client   Mesh FW Update Client
 * @ingroup     wiced_bt_mesh_models
 *
 * The AIROC Mesh FW Update Client module provides a way for an application to control Firmware Distribution Servers  in the mesh network
 *
 * TBD
 *
 * @{
 */

/**
* \brief Generic Level Client Message Handler
* \details The Mesh Core library calls this function for each message received.  The function returns WICED_TRUE if the message is destined for this specific model and successfully processed, and returns WICED_FALSE otherwise. Generic Level Server device.
* The function parses the message and if appropriate calls the parent back to perform functionality.
*
* @param       p_event Mesh event with information about received message.
* @param       p_data Pointer to the data portion of the message
* @param       data_len Length of the data in the message
*
* @return      WICED_TRUE if the message is for this company ID/Model/Element Index combination, WICED_FALSE otherwise.
*/
wiced_bool_t wiced_bt_mesh_model_fw_update_client_message_handler(wiced_bt_mesh_event_t *p_event, uint8_t *p_data, uint16_t data_len);
/* @} wiced_bt_mesh_fw_update_client */


/**
* BLOB Transfer Status Data structure exchanged between the application and Generic Level Model
*/

/* BLOB Transfer States */

/* BLOB Transfer Capabilities States */
typedef struct
{
    uint8_t     min_block_size_log;                     /**< Minimum block size: 2^Max Block size Log */
    uint8_t     max_block_size_log;                     /**< Maximum block size: 2^Max Block size Log */
    uint16_t    max_chunks_number;                      /**< Supported maximum number of chunks in block */
    uint16_t    max_chunk_size;                         /**< Max chunk size */
    uint32_t    max_blob_size;                          /**< Maximum BLOB size which can be stored */
    uint16_t    mtu;                                    /**< Maximum payload size which fits into one message */
    uint8_t     supported_transfer_mode;                /**< BLOB transfer mode supported by the server */
} mesh_blob_transfer_capabilities_state_t;


/* BLOB Transfer Messages */

/**
 * NOTE: This is preliminary implementation subject to change
 * @anchor BLOB_TRANSFER_EVENT
 * @name Definition for messages exchanged between an app and BLOB Transfer Model
 * @{ */
#define WICED_BT_MESH_BLOB_TRANSFER_START               1   /**< BLOB transfer started */
#define WICED_BT_MESH_BLOB_TRANSFER_DATA                2   /**< Data chunk from the distributor */
#define WICED_BT_MESH_BLOB_TRANSFER_STATUS              3   /**< BLOB transfer status changed */
#define WICED_BT_MESH_BLOB_TRANSFER_FINISH              4   /**< BLOB transfer completed */
/** @} BLOB_TRANSFER_EVENT */

#define WICED_BT_MESH_BLOB_TRANSFER_SUSPENDED           1
#define WICED_BT_MESH_BLOB_TRANSFER_RESUMED             2

#define WICED_BT_MESH_BLOB_TRANSFER_SUCCESS             0
#define WICED_BT_MESH_BLOB_TRANSFER_FAILED              1
#define WICED_BT_MESH_BLOB_TRANSFER_ABORTED             2

/**
 * BLOB Transfer Start structure exchanged between the application and BLOB Transfer model
 */
typedef struct
{
    uint32_t blob_size;                             /**< BLOB size */
    uint32_t block_size;                            /**< block size */
} wiced_bt_mesh_blob_transfer_start_t;

/**
 * BLOB Transfer Block Data structure exchanged between the application and BLOB Transfer model
 */
typedef struct
{
    uint32_t offset;                                /**< Data offset */
    uint32_t data_len;                              /**< Block data length */
    uint8_t  *p_data;                               /**< Pointer to data buffer */
} wiced_bt_mesh_blob_transfer_block_data_t;

/**
 * BLOB Transfer Status structure exchanged between the application and BLOB Transfer model
 */
typedef struct
{
    uint8_t state;                                  /**< BLOB Transfer state */
} wiced_bt_mesh_blob_transfer_status_t;

/**
 * BLOB Transfer Finish structure exchanged between the application and BLOB Transfer model
 */
typedef struct
{
    uint8_t blob_transfer_result;                   /**< BLOB Transfer final result */
} wiced_bt_mesh_blob_transfer_finish_t;


typedef struct
{
    uint8_t     status;                             /**< BLOB block status */
} wiced_bt_mesh_blob_transfer_complete_t;

/* Firmware Upload Messages */

/**
 * NOTE: This is preliminary implementation subject to change
 * @anchor FIRMWARE_UPLOAD_EVENT
 * @name Definition for messages exchanged between an app and Upload OOB module
 * @{ */
#define WICED_BT_MESH_FW_UPLOAD_START                   1   /**< Firmware Upload started */
#define WICED_BT_MESH_FW_UPLOAD_DATA                    2   /**< Firmware Upload firmware image data received */
#define WICED_BT_MESH_FW_UPLOAD_METADATA                3   /**< Firmware Upload metadata received */
#define WICED_BT_MESH_FW_UPLOAD_FW_ID                   4   /**< Firmware Upload firmware ID received */
#define WICED_BT_MESH_FW_UPLOAD_PROGRESS                5   /**< Firmware Upload progress */
#define WICED_BT_MESH_FW_UPLOAD_FINISH                  6   /**< Firmware Upload finished */
/** @} FIRMWARE_UPLOAD_EVENT */

/**
 * Firmware Upload Start structure exchanged between the application and Firmware Upload module
 */
typedef struct
{
    uint32_t fw_size;                               /**< firmware image size */
} wiced_bt_mesh_fw_upload_start_t;

/**
 * Firmware Upload Data structure exchanged between the application and Firmware Upload module
 */
typedef struct
{
    uint32_t offset;                                /**< FW image data offset. This needs to be DWORD alligned */
    uint32_t data_len;                              /**< FW image data block length */
    uint8_t  *p_data;                               /**< Pointer to data buffer */
} wiced_bt_mesh_fw_upload_data_t;

/**
 * Firmware Upload Progress structure exchanged between the application and Firmware Upload module
 */
typedef struct
{
    uint8_t progress;                               /**< Upload progress (0 ~ 100) */
} wiced_bt_mesh_fw_upload_progress_t;

#define WICED_BT_MESH_UPLOAD_SUCCESS                    0
#define WICED_BT_MESH_UPLOAD_FAILED                     1

/**
 * Firmware Upload Finish structure exchanged between the application and Firmware Upload module
 */
typedef struct
{
    uint8_t result;                                 /**< WICED_BT_MESH_UPLOAD_SUCCESS or WICED_BT_MESH_UPLOAD_FAILED */
} wiced_bt_mesh_fw_upload_finish_t;

/**
* \brief  Firmware upload event handler
* \details The firmware upload lower layer module (BLOB transfer or OOB) calls this function
* to send back firmware upload events.
*
* @param       event    One of FIRMWARE_UPLOAD_EVENT
* @param       p_data   Pointer to a data structure type depends on event type
*
* @return      None
*/
void wiced_bt_mesh_model_fw_upload_event_handler(uint16_t event, void *p_data);


/**
 * NOTE: This is preliminary implementation subject to change
 * @addtogroup  wiced_bt_mesh_blob_transfer_client   Mesh BLOB Transfer Client
 * @ingroup     wiced_bt_mesh_models
 *
 * The AIROC Mesh BLOB Transfer Client module provides a way for an application to transfer a BLOB to BLOB Transfer Servers in the mesh network
 *
 * TBD
 *
 * @{
 */
/**
 * \brief BLOB Transfer callback is called by the Mesh Models library on receiving a message from the peer
 *
 * @param       event The event that the application should process (see @ref LEVEL_EVENT "Mesh Level Events")
 * @param       p_data Pointer to the level data
 *
 * @return      None
 */
typedef void(wiced_bt_mesh_blob_transfer_client_callback_t)(uint16_t event, void *p_data);

/**
 * \brief  BLOB Transfer Client Data callback is called by the Mesh Models library request data for chunk transfer
 *
 * @param       p_data Pointer to the chunk data
 * @param       offset offset into the BLOB data
 * @param       data_len len of chunk data requested
 * @return      None
 */
typedef void(wiced_bt_mesh_blob_transfer_client_data_callback_t)(uint32_t offset, uint8_t *p_data, uint16_t data_len);


/**
* \brief  BLOB Transfer Client Message Handler
* \details The Mesh Core library calls this function for each message received.  The function returns WICED_TRUE if the message is destined for this specific model and successfully processed, and returns WICED_FALSE otherwise. Generic Level Server device.
* The function parses the message and if appropriate calls the parent back to perform functionality.
*
* @param       p_event Mesh event with information about received message.
* @param       p_data Pointer to the data portion of the message
* @param       data_len Length of the data in the message
*
* @return      WICED_TRUE if the message is for this company ID/Model/Element Index combination, WICED_FALSE otherwise.
*/
wiced_bool_t wiced_bt_mesh_model_blob_transfer_client_message_handler(wiced_bt_mesh_event_t *p_event, uint8_t *p_data, uint16_t data_len);

/**
 * \brief BLOB Transfer Client Module initialization
 * \details A layer that needs to utilize BLOB Transfer Client services shall call this function to register a callback which will be
 * called on the execution of further function calls.
 *
 * @param       p_callback The application callback function that will be executed by the mesh models library when a response for the client's request has been received.
 * @param       is_provisioned If TRUE, the application is being restarted after being provisioned or after a power loss. If FALSE the model cleans up NVRAM on startup.
 *
 * @return      None
 */
void wiced_bt_mesh_model_blob_transfer_client_init(wiced_bt_mesh_blob_transfer_client_callback_t *p_callback, wiced_bt_mesh_blob_transfer_client_data_callback_t *p_data_callback);

/**
 * \brief BLOB Transfer Client Module de-initialization
 * \details A layer finished using BLOB Transfer Client shall call this function to release allocated resources.
 *
 * @return      None
 */
void wiced_bt_mesh_model_blob_transfer_client_deinit();

/**
 * \brief BLOB Transfer Start
 * \details A client model above BLOB Transfer Client or an application can call this function to start actual BLOB transfer.
 * The tranfer is done using the multicast to a group address.  The BLOB is split into blocks which is then split into chunks.  The
 * group list is used by the BLOB Transfer Client to identify which chunks of a block has not been received and retransmit them.
 *
 * @param       group_addr          Group address to multicast the chunks
 * @param       p_group_list        List of the nodes currently included in the group
 * @param       group_size          Number of the elements in the group list
 * @param       p_blob_id           BLOB ID for the current transmission
 * @param       blob_size           Size of the BLOB to distribute
 * @param       app_key_idx         App Key Index of the key to be used for securing the data
 * @param       transfer_mode       Pull/Push
 * @param       ttl                 TTL used by BLOB transfer for all messages
 *
 * @return      WICED_TRUE if BLOB transfer has been started successfully
 */
wiced_bool_t wiced_bt_mesh_model_blob_transfer_client_start(uint16_t group_addr, uint16_t *p_group_list, uint16_t group_size, uint8_t *p_blob_id, uint32_t blob_size, uint16_t app_key_idx, uint8_t transfer_mode, uint8_t ttl);

/**
 * \brief BLOB Transfer Abort
 * \details A client model above BLOB Transfer Client or an application can call this function to abort BLOB Transfer.
 *
 * @param       p_blob_id           BLOB ID for the current transmission
 *
 * @return      WICED_TRUE if BLOB transfer has been aborted successfully
 */
wiced_bool_t wiced_bt_mesh_model_blob_transfer_client_abort(uint8_t *p_blob_id);

/**
 * \brief BLOB Transfer Suspend
 * \details A client model above BLOB Transfer Client or an application can call this function to suspend BLOB Transfer.
 *
 * @param       p_blob_id           BLOB ID for the current transmission
 *
 * @return      WICED_TRUE if BLOB transfer has been suspended successfully
 */
wiced_bool_t wiced_bt_mesh_model_blob_transfer_client_suspend(uint8_t *p_blob_id);

/**
 * \brief BLOB Transfer Resume
 * \details A client model above BLOB Transfer Client or an application can call this function to resume BLOB Transfer.
 *
 * @param       p_blob_id           BLOB ID for the current transmission
 *
 * @return      WICED_TRUE if BLOB transfer has been resumed successfully
 */
wiced_bool_t wiced_bt_mesh_model_blob_transfer_client_resume(uint8_t *p_blob_id);

/**
 * \brief BLOB Transfer Client Get Node Status
 * \details A client model above BLOB Transfer Client or an application can call this function to retrieve status of a node.
 * The function executes and returns results syncronously.
 *
 * @param       unicast_addr    Address of the node (BLOB Transfer Server)
 * @param       p_state         Pointer to return the node state
 * @param       p_status        Pointer to return the node last received status
 * @param       p_progress      Pointer to return the transfer progress (if in active state, otherwise return 0)
 *
 * @return      WICED_TRUE if node exist
 */
wiced_bool_t wiced_bt_mesh_blob_transfer_client_get_node_status(uint16_t unicast_addr, uint8_t *p_state, uint8_t* p_status, uint8_t *p_progress);
/* @} wiced_bt_mesh_blob_transfer_client */

/**
 * NOTE: This is preliminary implementation subject to change
 * @addtogroup  wiced_bt_mesh_fw_update_server   Mesh Firmware Update Server
 * @ingroup     wiced_bt_mesh_models
 *
 * The AIROC Mesh Firmware Update Server module provides a way for an application to Update Firmware  in the mesh network
 *
 * TBD
 *
 * @{
 */
/*
 * \brief Firmware update server callback is called by the Mesh Models library on receiving a message from the peer
 *
 * @param       event The event that the application should process (see @ref FW_UPDATE_EVENT "Firmware Update Events")
 * @param       p_data Pointer to the firmware update data
 * @param       data_len Length of the firmware update data
 *
 * @return      None
 */
typedef wiced_bool_t(wiced_bt_mesh_fw_update_server_callback_t)(uint16_t event, uint8_t *p_data, uint16_t data_len);

/**
 * \brief Firmware Update Server Message Handler
 * \details The Mesh Core library calls this function for each message received.  The function returns WICED_TRUE if the message is destined for this specific model and successfully processed, and returns WICED_FALSE otherwise.
 * The function parses the message and if appropriate calls the parent back to perform functionality.
 *
 * @param       p_event   Mesh event with information about received message.
 * @param       p_data    Pointer to the data portion of the message
 * @param       data_len  Length of the data in the message
 *
 * @return      WICED_TRUE if the message is for this company ID/Model/Element Index combination, WICED_FALSE otherwise.
*/
wiced_bool_t wiced_bt_mesh_model_fw_update_server_message_handler(wiced_bt_mesh_event_t *p_event, uint8_t *p_data, uint16_t data_len);
/* @} wiced_bt_mesh_fw_update_server */

/**
 * @addtogroup  wiced_bt_mesh_blob_transfer_server   Mesh BLOB Transfer Server
 * @ingroup     wiced_bt_mesh_models
 *
 * The AIROC Mesh BLOB Transfer Server module provides a way for an application to recevie BLOBs in the mesh network
 *
 * TBD
 *
 * @{
 */

/*
 * \brief BLOB Transfer server callback is called by the Mesh Models library on receiving a message from the peer
 *
 * @param       event   event The event that the application should process (see @ref FW_UPDATE_EVENT "Firmware Update Events")
 * @param       p_data  pointer to the BLOB transfer data
 *
 * @return      None
 */
typedef void(wiced_bt_mesh_blob_transfer_server_callback_t)(uint16_t event, void *p_data);

/**
 * \brief BLOB Transfer Server Message Handler
 * \details The Mesh Core library calls this function for each message received.  The function returns WICED_TRUE if the message is destined for this specific model and successfully processed, and returns WICED_FALSE otherwise.
 * The function parses the message and if appropriate calls the parent back to perform functionality.
 *
 * @param       p_event   Mesh event with information about received message.
 * @param       p_data    Pointer to the data portion of the message
 * @param       data_len  Length of the data in the message
 *
 * @return      WICED_TRUE if the message is for this company ID/Model/Element Index combination, WICED_FALSE otherwise.
*/
wiced_bool_t wiced_bt_mesh_model_blob_transfer_server_message_handler(wiced_bt_mesh_event_t *p_event, uint8_t *p_data, uint16_t data_len);

/**
 * \brief BLOB Transfer Server prepare for transfer
 *
 * @param       blob_id     Pointer to BLOB ID
 * @param       ttl         TTL used by BLOB transfer for all messages
 * @param       time_out    BLOB transfer idle time out
 * @param       p_callback  The application callback function that will be executed by the mesh models library when application action is required, or when a reply for the application request has been received.
 *
 * @return      WICED_TRUE if BLOB server is prepared successfully. WICED_FALSE if BLOB server is busy.
 */
wiced_bool_t  wiced_bt_mesh_model_blob_transfer_prepare(uint8_t *blob_id, uint8_t ttl, uint16_t time_out, wiced_bt_mesh_blob_transfer_server_callback_t *p_callback);

/**
 * \brief BLOB Transfer Server transfer close
 *
 * @param       Nond
 *
 * @return      None
 */
void wiced_bt_mesh_model_blob_transfer_close();

/**
 * \brief Get BLOB Transfer Server phase
 *
 * @param       None
 *
 * @return      None
 */
uint8_t wiced_bt_mesh_model_blob_transfer_get_phase();

/*---------------------------------------------------------------------------*/
/*                      wiced_bt_mesh_provision.h                            */
/*---------------------------------------------------------------------------*/

typedef PACKED struct
{
    uint8_t index;                                              /**< Index of the firmware image in the Firmware Information List state to check */
    mesh_dfu_metadata_t metadata;                               /**< Vendor-specific metadata */
} wiced_bt_mesh_dfu_metadata_check_data_t;

typedef PACKED struct
{
    uint8_t status;                                             /**< Status Code from the firmware metadata check */
    uint8_t add_info;                                           /**< Additional Information - whether node needs to be reprovisioned after firmware update */
    uint8_t index;                                              /**< Index of the firmware image in the Firmware Information List state that was checked */
} wiced_bt_mesh_dfu_metadata_status_data_t;

/**
 * NOTE: This is preliminary implementation subject to change
 * @anchor FW_DISTRIBUTION_STATE
 * @name Definitions of firmware distribution states
 * @{ */
#define WICED_BT_MESH_DFU_STATE_INIT                        0    /**< Initial state */
#define WICED_BT_MESH_DFU_STATE_VALIDATE_NODES              1    /**< Checking if nodes can accept the firmware */
#define WICED_BT_MESH_DFU_STATE_GET_DISTRIBUTOR             2    /**< Finding a proper node as Distributor */
#define WICED_BT_MESH_DFU_STATE_UPLOAD                      3    /**< Uploading firmware from Initiator to Distributor */
#define WICED_BT_MESH_DFU_STATE_DISTRIBUTE                  4    /**< Distributing firmware from Distributor to Updating Nodes */
#define WICED_BT_MESH_DFU_STATE_APPLY                       5    /**< Applying firmware on Updating Nodes */
#define WICED_BT_MESH_DFU_STATE_COMPLETE                    6    /**< Firmware distribution completed successfully */
#define WICED_BT_MESH_DFU_STATE_FAILED                      7    /**< Firmware distribution failed */
#define WICED_BT_MESH_DFU_STATE_SUSPENDED                   8    /**< Firmware distribution suspended */
 /** @} FW_DISTRIBUTION_STATE */

typedef PACKED struct
{
    uint16_t addr;
    wiced_bool_t low_power;
} wiced_bt_mesh_fw_distribution_node_t;

/* This structure contains information sent from the provisioner application to the firmware distributor to start firware distribution for the specified list of nodes. */
typedef PACKED struct
{
    mesh_dfu_fw_id_t firmware_id;                               /**< Firmware ID of the firmware that will be downloaded */
    mesh_dfu_metadata_t metadata;                               /**< Metadata of the firmware that will be downloaded */
    uint32_t firmware_size;                                     /**< Firmware size */
    uint16_t proxy_addr;                                        /**< Address of the proxy device */
    uint16_t group_addr;                                        /**< Group address to be used for the nodes being updated */
    uint16_t group_size;                                        /**< Number of elements in the update_node_list */
#define WICED_BT_MESH_MAX_UPDATES_NODES 256                     /**< MAX number of updating nodes  */
    wiced_bt_mesh_fw_distribution_node_t update_nodes[WICED_BT_MESH_MAX_UPDATES_NODES];/**< List of the nodes */
} wiced_bt_mesh_fw_distribution_start_data_t;

/* This structure contains node information sent from the Distributor node when application requested the status information  */
typedef PACKED struct
{
    uint16_t unicast_address;
    uint8_t  phase;
    uint8_t  progress;
} wiced_bt_mesh_fw_distribution_details_t;

/* This structure contains information sent from the Distributor node when application requested the status information  */
typedef PACKED struct
{
    uint8_t         state;                                      /**< Current distribution state (see @ref FW_DISTRIBUTION_STATE) */
    uint16_t        list_size;                                  /**< Node list size */
    uint16_t        node_index;                                 /**< Index of first node */
    uint16_t        num_nodes;                                  /**< Number of nodes */
    wiced_bt_mesh_fw_distribution_details_t node[1];            /**< Details for each node */
} wiced_bt_mesh_fw_distribution_status_data_t;

/**
 * \brief Firmware Provider callback
 * \details The Firmware Provider callback is called by the Mesh Provision library during the device firmware update process
 *
 * @param[in]       event Event that the application should process (see @ref PROVISION_EVENT "Mesh Provisioner Events")
 * @param[in]       p_event Pointer to the data structure identifying the source of the message
 * @param[in]       p_data Data with DFU information.
 *
 * @return      None
 */
typedef void(wiced_bt_mesh_fw_provider_callback_t)(uint16_t event, wiced_bt_mesh_event_t *p_event, void *p_data);

/**
 * \brief Firmware Provider Model initialization
 * \details The Provisioner Application should call this function during the startup to initialize FW provider.
 *
 * @param none
 *
 * @return      None
 */
void wiced_bt_mesh_model_fw_provider_init(void);

/**
 * \brief Firmware Provider Message Handler
 * \details The mesh core library calls this function for each message received.  The function returns WICED_TRUE if the message is destined for this specific model and successfully processed, and returns WICED_FALSE otherwise. Generic Level Server device.
 * The function parses the message and if appropriate calls the parent back to perform functionality.
 *
 * @param[in]       p_event Mesh event with information about received message.
 * @param[in]       p_data Pointer to the data portion of the message
 * @param[in]       data_len Length of the data in the message
 *
 * @return      WICED_TRUE if the message is for this company ID/Model/Element Index combination, WICED_FALSE otherwise.
 */
wiced_bool_t wiced_bt_mesh_model_fw_provider_message_handler(wiced_bt_mesh_event_t *p_event, uint8_t *p_data, uint16_t data_len);

/**
* \brief The application can call this function to check metadata on remote device.
*
* @param[in]       p_event Mesh event with the remote device address.
* @param[in]       p_data Pointer to the data to send
*
* @return      WICED_TRUE if operation has started successfully
*/
wiced_bool_t wiced_bt_mesh_dfu_metadata_check(wiced_bt_mesh_event_t *p_event, wiced_bt_mesh_dfu_metadata_check_data_t *p_data, wiced_bt_mesh_fw_provider_callback_t *p_callback);

/**
 * \brief The application can call this function to get the state of the current firmware distribution process.
 * The function may register a callback which will be executed when reply from the distributor is received.
 *
 * @param[in]       p_event Mesh event with the distributor's address that has been created by the app for unsolicited message.
 * @param[in]       p_callback The application callback function that will be executed by the Mesh Provisioning library when a reply for the application request has been received.
 *
 * @return      WICED_TRUE if operation has started successfully
 */
wiced_bool_t wiced_bt_mesh_fw_provider_get_status(wiced_bt_mesh_event_t *p_event, wiced_bt_mesh_fw_provider_callback_t *p_callback);

/**
 * \brief The application can call this function to start firmware distribution procedure.
 *
 * @param[in]       p_event Mesh event with the distributor address that has been created by the app for unsolicited message.
 * @param[in]       p_data Pointer to the data to send
 *
 * @return      WICED_TRUE if operation has started successfully
 */
wiced_bool_t wiced_bt_mesh_fw_provider_start(wiced_bt_mesh_event_t *p_event, wiced_bt_mesh_fw_distribution_start_data_t *p_data, wiced_bt_mesh_fw_provider_callback_t *p_callback);

/**
 * \brief The application can call this function to suspend firmware distribution procedure.
 *
 * @param[in]       p_event Mesh event with the distributor address that has been created by the app for unsolicited message.
 *
 * @return      WICED_TRUE if operation has started successfully
 */
wiced_bool_t wiced_bt_mesh_fw_provider_suspend(wiced_bt_mesh_event_t *p_event);

/**
 * \brief The application can call this function to resume firmware distribution procedure.
 *
 * @param[in]       p_event Mesh event with the distributor address that has been created by the app for unsolicited message.
 *
 * @return      WICED_TRUE if operation has started successfully
 */
wiced_bool_t wiced_bt_mesh_fw_provider_resume(wiced_bt_mesh_event_t *p_event);

/**
 * \brief The application can call this function to terminate firmware distribution procedure.
 *
 * @param[in]       p_event Mesh event with the distributor address that has been created by the app for unsolicited message.
 *
 * @return      WICED_TRUE if operation has started successfully
 */
wiced_bool_t wiced_bt_mesh_fw_provider_stop(wiced_bt_mesh_event_t *p_event);

/*
 * \brief Process finish event received from OTA client
 *
 * @param[in]   status  0 if OTA finished successfully, otherwise failed
 *
 * @return      None
 */
void wiced_bt_mesh_fw_provider_ota_finish(uint8_t status);

/*---------------------------------------------------------------------------*/
/*                     wiced_bt_mesh_model_utils.h                           */
/*---------------------------------------------------------------------------*/

/*
 * Extract DFU FW ID from data stream. Return WICED_FALSE if data is too short or format is invalid.
 */
wiced_bool_t mesh_dfu_fw_id_from_data(mesh_dfu_fw_id_t *firmware_id, uint8_t **p_data, uint16_t *data_len);

/*
 * Store DFU FW ID to stream
 */
void mesh_dfu_fw_id_to_data(mesh_dfu_fw_id_t *firmware_id, uint8_t **p_data);

/*
 * Compare DFU FW IDs
 */
wiced_bool_t mesh_dfu_compare_fw_id(mesh_dfu_fw_id_t *firmware_id_src, mesh_dfu_fw_id_t *firmware_id_dst);

/*
 * Put length and data to stream
 */
uint8_t *mesh_dfu_length_data_to_stream(uint8_t *p_data, uint8_t length, uint8_t *p_stream);

/*
 * Get FW ID from active firmware
 */
wiced_bool_t mesh_dfu_get_active_fw_id(mesh_dfu_fw_id_t *p_fw_id);

/*
 * Check if FW ID is acceptable
 */
wiced_bool_t mesh_dfu_metadata_check(uint8_t index, mesh_dfu_metadata_t *p_metadata);

/*
 * Verify received FW with meta data
 */
wiced_bool_t mesh_dfu_fw_verify(uint8_t index, uint32_t fw_size, mesh_dfu_metadata_t *p_metadata);

/*
 * Apply received FW
 */
wiced_bool_t mesh_dfu_fw_apply(uint8_t index);

/*
 * Start OOB firmware upload
 */
uint8_t mesh_dfu_fw_upload_oob(mesh_dfu_uri_t *p_uri, mesh_dfu_fw_id_t *p_fw_id);

/*
 * DFU active states
 */
extern wiced_bool_t mesh_dfu_upload_active;
extern wiced_bool_t mesh_dfu_distribution_active;
extern wiced_bool_t mesh_dfu_update_active;

/*
 * Set one bit in a bit field
 */
void mesh_bitfield_set_bit(uint8_t *p_bitfield, uint32_t bit_number);

/*
 * Check if a bit is set in a bit field
 */
wiced_bool_t mesh_bitfield_check_bit(uint8_t *p_bitfield, uint32_t bit_number);
/*
 * Set all bits in a bit field
 */
void mesh_bitfield_set_all(uint8_t *p_bitfield, uint32_t total_bits);

/*
 * Get next set bit in a bit field
 */
uint32_t mesh_bitfield_get_next_bit(uint8_t *p_bitfield, uint32_t current_bit, uint32_t total_bits);

/*
 * Get number of all set bits in a bit field
 */
uint32_t mesh_bitfield_get_number_of_bits(uint8_t *p_bitfield, uint32_t total_bits);

/*
 * Encode UINT16 value to UTF-8 byte array
 */
void mesh_uint16_to_utf8(uint16_t number, uint8_t **pp);

/*
 * Decode UINT16 value from UTF-8 byte array
 */
uint16_t mesh_uint16_from_utf8(uint8_t **pp);

/*! \endcond */

#ifdef __cplusplus
}
#endif

#endif /* __MESH_MODEL_DFU_H__ */

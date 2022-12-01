/*
 * $ Copyright 1999-YEAR Cypress Semiconductor $
 */

/****************************************************************************
**
**  Name        bt_types.h
**
**  Function    this file contains definitions that are shared between
**              units in the Bluetooth system such as events.
**
****************************************************************************/

#ifndef BT_TYPES_H
#define BT_TYPES_H

#include "wiced_bt_types.h"

#ifndef _TYPEDEF_UINT8
#define _TYPEDEF_UINT8
typedef uint8_t                 UINT8;
#endif
#ifndef _TYPEDEF_BYTE
#define _TYPEDEF_BYTE
typedef uint8_t                 BYTE;
#endif
//! Signed 8-bit integer.
typedef int8_t                  INT8;

//! Unsigned 16-bit integer.
#ifndef _TYPEDEF_UINT16
#define _TYPEDEF_UINT16
typedef uint16_t                UINT16;
#endif
typedef uint16_t                WORD;

//! Signed 16-bit integer.
typedef int16_t                 INT16;
#ifndef int16
typedef int16_t                 int16;
#endif
//! Unsigned 32-bit integer.
#ifndef _TYPEDEF_UINT32
#define _TYPEDEF_UINT32
typedef uint32_t                UINT32;
#endif
#ifndef WIN32
typedef uint32_t                DWORD;
#endif
//! Signed 32-bit integer.
typedef int32_t                 INT32;

//! Unsigned 64-bit integer.
#ifndef _TYPEDEF_UINT64
#define _TYPEDEF_UINT64
typedef uint64_t                UINT64;
#endif
//! Signed 64-bit integer.
typedef int64_t                 INT64;
#ifndef _TYPEDEF_BOOL32
#define _TYPEDEF_BOOL32
typedef unsigned int            BOOL32;
#endif
typedef unsigned char           BOOL8;

/* Any queueable entity (message, control block, etc, should have this structure at its base */
typedef struct _btu_queue_elem
{
    struct _btu_queue_elem  *p_next;           /* Pointer to the next element in the queue. */
} tBTU_Q_ELEM;

/* A queue, to which entities of type tBTU_QUEUE_ENTRY can be enqueued or dequeued. */
typedef struct
{
    tBTU_Q_ELEM   *p_first;
    tBTU_Q_ELEM   *p_last;
    int            count;
} tBTU_QUEUE;

typedef wiced_bt_transport_t tBT_TRANSPORT;


/* Define the Data Transmission Control Block, which is used to send ACL data
** through the stack all the way down to L2CAP.
*/
typedef struct
{
    tBTU_Q_ELEM q_base;                         /* The base class for queueable entities */
    uint32_t    layer_flags;                    /* Layer-specific flags                 */
    void        *pv_app_ctx;                    /* App context                          */
    uint8_t     *p_payload;                     /* Pointer to Payload                   */
    uint16_t    payload_len;                    /* Payload length                       */
    uint16_t    payload_offset;                 /* Payload offset                       */
    uint16_t    prot_hdr_len;                   /* Protocol header length               */
    uint16_t    prot_hdr_offset;                /* Protocol header offset               */
    uint8_t     dtcb_flags;                     /* Flags as defined in DTCB_FLAGS_xxxxx */
    uint8_t     rfcomm_crc;                     /* RFComm CRC (if indicated in flags)   */
    uint8_t     protocol_hdr[1];                /* Protocol header (size varies)        */
} tDTCB;

/* DTCB flag byte (this is actually a bitfield) */
typedef enum
{
    DTCB_FLAGS_NONE                 = 0,
    DTCB_FLAGS_APPLICATION_USE      = 0x01,         /* The DTCB was allocated for app use   */
    DTCB_FLAGS_RFCOMM_CRC_PRESENT   = 0x02,         /* An RFCOMM CRC is present in the DTCB */
    DTCB_FLAGS_MULTI_SEG            = 0x04          /* The DTCB has a multi-segment payload */
}tDTCB_flags;

/* These macros extract the HCI opcodes from a buffer
*/
#define HCI_GET_CMD_HDR_OPCODE(p)    (uint16_t)((*((uint8_t *)((p) + 1) + p->offset) + \
                                              (*((uint8_t *)((p) + 1) + p->offset + 1) << 8)))
#define HCI_GET_CMD_HDR_PARAM_LEN(p) (uint8_t)  (*((uint8_t *)((p) + 1) + p->offset + 2))

#define HCI_GET_EVT_HDR_OPCODE(p)    (uint8_t)(*((uint8_t *)((p) + 1) + p->offset))
#define HCI_GET_EVT_HDR_PARAM_LEN(p) (uint8_t)  (*((uint8_t *)((p) + 1) + p->offset + 1))



#ifndef WICED_BT_DEVICE_ADDRESS
#define WICED_BT_DEVICE_ADDRESS             /**< Bluetooth Device Address Length */
#define BD_ADDR_LEN     6
typedef uint8_t       wiced_bt_device_address_t[BD_ADDR_LEN]; /**< Bluetooth Device Address */
#endif

#ifndef BD_ADDR
typedef uint8_t BD_ADDR[BD_ADDR_LEN];
#endif

//! Macro to obtain the minimum (smaller) of two numbers.
#define BT_MIN( a, b )                                             ( ( (a) < (b) ) ? (a) : (b) )

//! Macro to obtain the maximum (larger) of two numbers.
#define BT_MAX( a, b )                                             ( ( (a) > (b) ) ? (a) : (b) )



#define LINK_KEY_LEN    16
typedef uint8_t LINK_KEY[LINK_KEY_LEN];       /* Link Key */

#define PIN_CODE_LEN    16
typedef uint8_t PIN_CODE[PIN_CODE_LEN];       /* Pin Code (upto 128 bits) MSB is 0 */
typedef uint8_t *PIN_CODE_PTR;                /* Pointer to Pin Code */

#define DEV_CLASS_LEN   3
typedef uint8_t DEV_CLASS[DEV_CLASS_LEN];     /* Device class */
typedef uint8_t *DEV_CLASS_PTR;               /* Pointer to Device class */

#define EXT_INQ_RESP_LEN   3
typedef uint8_t EXT_INQ_RESP[EXT_INQ_RESP_LEN];/* Extended Inquiry Response */
typedef uint8_t *EXT_INQ_RESP_PTR;             /* Pointer to Extended Inquiry Response */

#define BD_NAME_LEN     248
typedef uint8_t BD_NAME[BD_NAME_LEN + 1];         /* Device name */
typedef uint8_t *BD_NAME_PTR;                 /* Pointer to Device name */

#define BT_EVENT_MASK_LEN  8
typedef uint8_t BT_EVENT_MASK[BT_EVENT_MASK_LEN];   /* Event Mask */

#define LAP_LEN         3
typedef uint8_t LAP[LAP_LEN];                 /* IAC as passed to Inquiry (LAP) */
typedef uint8_t INQ_LAP[LAP_LEN];             /* IAC as passed to Inquiry (LAP) */

#define RAND_NUM_LEN    16
typedef uint8_t RAND_NUM[RAND_NUM_LEN];

#define ACO_LEN         12
typedef uint8_t ACO[ACO_LEN];                 /* Authenticated ciphering offset */

#define COF_LEN         12
typedef uint8_t COF[COF_LEN];                 /* ciphering offset number */

#define FLOW_SPEC wiced_bt_flow_spec_t

/* Values for service_type */
#define NO_TRAFFIC      0
#define BEST_EFFORT     1
#define GUARANTEED      2

/* Service class of the CoD */
#define SERV_CLASS_NETWORKING               (1 << 1)
#define SERV_CLASS_RENDERING                (1 << 2)
#define SERV_CLASS_CAPTURING                (1 << 3)
#define SERV_CLASS_OBJECT_TRANSFER          (1 << 4)
#define SERV_CLASS_OBJECT_AUDIO             (1 << 5)
#define SERV_CLASS_OBJECT_TELEPHONY         (1 << 6)
#define SERV_CLASS_OBJECT_INFORMATION       (1 << 7)

/* Second byte */
#define SERV_CLASS_LIMITED_DISC_MODE        (0x20)

/* Field size definitions. Note that byte lengths are rounded up. */
#define ACCESS_CODE_BIT_LEN             72
#define ACCESS_CODE_BYTE_LEN            9
#define SHORTENED_ACCESS_CODE_BIT_LEN   68

typedef uint8_t ACCESS_CODE[ACCESS_CODE_BYTE_LEN];

#define SYNTH_TX                1           /* want synth code to TRANSMIT at this freq */
#define SYNTH_RX                2           /* want synth code to RECEIVE at this freq */

#define SYNC_REPS 1             /* repeats of sync word transmitted to start of burst */

/* Bluetooth CLK27 */
#define BT_CLK27            (2 << 26)

/* Bluetooth CLK12 is 1.28 sec */
#define BT_CLK12_TO_MS(x)    ((x) * 1280)
#define BT_MS_TO_CLK12(x)    ((x) / 1280)
#define BT_CLK12_TO_SLOTS(x) ((x) << 11)

/* Bluetooth CLK is 0.625 msec */
#define BT_CLK_TO_MS(x)      (((x) * 5 + 3) / 8)
#define BT_MS_TO_CLK(x)      (((x) * 8 + 2) / 5)

#define BT_CLK_TO_MICROSECS(x)  (((x) * 5000 + 3) / 8)
#define BT_MICROSECS_TO_CLK(x)  (((x) * 8 + 2499) / 5000)

#define tBT_UUID wiced_bt_uuid_t

#define BT_EIR_FLAGS_TYPE                   0x01
#define BT_EIR_MORE_16BITS_UUID_TYPE        0x02
#define BT_EIR_COMPLETE_16BITS_UUID_TYPE    0x03
#define BT_EIR_MORE_32BITS_UUID_TYPE        0x04
#define BT_EIR_COMPLETE_32BITS_UUID_TYPE    0x05
#define BT_EIR_MORE_128BITS_UUID_TYPE       0x06
#define BT_EIR_COMPLETE_128BITS_UUID_TYPE   0x07
#define BT_EIR_SHORTENED_LOCAL_NAME_TYPE    0x08
#define BT_EIR_COMPLETE_LOCAL_NAME_TYPE     0x09
#define BT_EIR_TX_POWER_LEVEL_TYPE          0x0A
#define BT_EIR_OOB_BD_ADDR_TYPE             0x0C
#define BT_EIR_OOB_COD_TYPE                 0x0D
#define BT_EIR_OOB_SSP_HASH_C_TYPE          0x0E
#define BT_EIR_OOB_SSP_RAND_R_TYPE          0x0F
#define BT_EIR_OOB_SSP_HASH_C_256_TYPE      0x1D
#define BT_EIR_OOB_SSP_RAND_R_256_TYPE      0x1E
#define BT_EIR_3D_SYNC_TYPE                 0x3D
#define BT_EIR_MANUFACTURER_SPECIFIC_TYPE   0xFF

#define BT_OOB_COD_SIZE            3
#define BT_OOB_HASH_C_SIZE         16
#define BT_OOB_RAND_R_SIZE         16
#define BT_OOB_HASH_C_256_SIZE     16
#define BT_OOB_RAND_R_256_SIZE     16

/* Broadcom proprietary UUIDs and reserved PSMs
**
** The lowest 4 bytes byte of the UUID or GUID depends on the feature. Typically,
** the value of those bytes will be the PSM or SCN, but it is up to the features.
*/
#define BRCM_PROPRIETARY_UUID_BASE  0xDA, 0x23, 0x41, 0x02, 0xA3, 0xBB, 0xC1, 0x71, 0xBA, 0x09, 0x6f, 0x21
#define BRCM_PROPRIETARY_GUID_BASE  0xda23, 0x4102, 0xa3, 0xbb, 0xc1, 0x71, 0xba, 0x09, 0x6f, 0x21

/* We will not allocate a PSM in the reserved range to 3rd party apps
*/
#define BRCM_RESERVED_PSM_START	    0x5AE1
#define BRCM_RESERVED_PSM_END	    0x5AFF

#define BRCM_UTILITY_SERVICE_PSM    0x5AE1
#define BRCM_MATCHER_PSM            0x5AE3

/* Connection statistics
*/

/* Structure to hold connection stats */
#ifndef BT_CONN_STATS_DEFINED
#define BT_CONN_STATS_DEFINED

/* These bits are used in the bIsConnected field */
#define BT_CONNECTED_USING_BREDR   1

typedef struct
{
    uint32_t is_connected;
    int32_t    rssi;
    uint32_t bytes_sent;
    uint32_t bytes_rcvd;
    uint32_t duration;
} tBT_CONN_STATS;

#endif

/*****************************************************************************
**                          Low Energy definitions
**
** Address types
*/
#define BLE_ADDR_PUBLIC         0x00
#define BLE_ADDR_RANDOM         0x01
#define BLE_ADDR_PUBLIC_ID      0x02
#define BLE_ADDR_RANDOM_ID      0x03
#define BLE_ADDR_TYPE_ID_BIT    0x02            /* The address is an identity address */
typedef uint8_t tBLE_ADDR_TYPE;


#define BLE_ADDR_TYPE_GET_ADDR_BIT(m) ((m)&1)   /* the address type can be BLE_ADDR_PUBLIC or BLE_ADDR_RANDOM */
#define BLE_ADDR_IS_STATIC(x)   ((x[0] & 0xC0) == 0xC0)

typedef wiced_bt_ble_address_t tBLE_BD_ADDR;

/* Device Types
*/
#define BT_DEVICE_TYPE_BREDR   0x01
#define BT_DEVICE_TYPE_BLE     0x02
#define BT_DEVICE_TYPE_DUMO    0x03
typedef uint8_t tBT_DEVICE_TYPE;
/*****************************************************************************/

/* Define PSMs HID uses */
#define HID_PSM_CONTROL         BT_PSM_HIDC
#define HID_PSM_INTERRUPT       BT_PSM_HIDI

/* Tracing definitions
*/
#define BT_TRACE_L2CAP          0x00000001
#define BT_TRACE_RFCOMM         0x00000002
#define BT_TRACE_SDP            0x00000004
#define BT_TRACE_BTM            0x00000008
#define BT_TRACE_HCI            0x00000010
#define BT_TRACE_HIDD           0x00000020
#define BT_TRACE_HIDH           0x00000040
#define BT_TRACE_AVDT           0x00000080
#define BT_TRACE_AVRC           0x00000100
#define BT_TRACE_AVCT           0x00000200
#define BT_TRACE_SPP            0x00000400
#define BT_TRACE_GAP            0x00000800
#define BT_TRACE_GATT           0x00001000
#define BT_TRACE_SMP            0x00002000
#define BT_TRACE_A2D            0x00004000
#define BT_TRACE_BTU            0x00008000

#define BT_TRACE_ERROR_MSGS     0x80000000
#define BT_TRACE_WARNING_MSGS   0x40000000
#define BT_TRACE_API_MSGS       0x20000000
#define BT_TRACE_EVENT_MSGS     0x10000000
#define BT_TRACE_DEBUG_MSGS     0x08000000
#define BT_TRACE_VERBOSE        0x04000000

#endif

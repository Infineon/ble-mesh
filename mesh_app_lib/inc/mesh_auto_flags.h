#ifdef _WIN32
#include "windows.h"
#include "winsock.h"
#endif

#include "btu.h"
#include "wiced_bt_types.h"
#include "wiced_memory.h"
#include "wiced_bt_ble.h"

#if defined(MESH_HOST_MODE) && !defined(WIN32)
#ifdef WICED_BT_TRACE
#undef WICED_BT_TRACE
#define WICED_BT_TRACE          mesh_trace_debug

#define WICED_BT_TRACE_ENABLE TRUE
#endif
#endif

#define WICED_BT_MESH_TRACE_ENABLE TRUE

#define LE_MULTI_ADV_MAX_NUM_INSTANCES  16

#define TIMER_PARAM_TYPE WICED_TIMER_PARAM_TYPE
#define MULTI_ADV_TX_POWER_MIN MULTI_ADV_TX_POWER_MIN_INDEX
#define MULTI_ADV_TX_POWER_MAX MULTI_ADV_TX_POWER_MAX_INDEX
#define MULTI_ADV_MAX_NUM_INSTANCES LE_MULTI_ADV_MAX_NUM_INSTANCES

#define wiced_memory_permanent_allocate wiced_bt_get_buffer

#define MESH_CORE_DONT_USE_TRACE_ENCODER
#define WICED_BT_MESH_CORE_TRACE_ENABLE

//#define EMBEDDED_PROV_UUID_MAGIC_NUBMER     0xb25d9a2fc46b4b2e

#define REMOTE_PROVISION_SERVER_SUPPORTED       TRUE
#define REMOTE_PROVISION_OVER_GATT_SUPPORTED    TRUE
#define GATT_PROXY_CLIENT_SUPPORTED             TRUE


#define wiced_memory_permanent_allocate wiced_bt_get_buffer
#define MULTI_ADV_MAX_NUM_INSTANCES LE_MULTI_ADV_MAX_NUM_INSTANCES

#define HCI_CONTROL

#define wiced_timer_callback_t wiced_timer_callback_t
#define p_256_init_curve(x) InitCurve(x)

#define WICED_SDK_MAJOR_VER     1
#define WICED_SDK_MINOR_VER     1
#define WICED_SDK_REV_NUMBER    1
#define WICED_SDK_BUILD_NUMBER  1

#if 0

#define WICED_BT_MESH_CORE_MODEL_ID_DIRECTED_FORWARDING_SRV             0x0010
#define WICED_BT_MESH_CORE_MODEL_ID_DIRECTED_FORWARDING_CLNT            0x0011

#define __RTC_H__

#define APPEARANCE_CONTROL_DEVICE_SWITCH 1217  /// Switch Control Device subtype

typedef struct
{
    uint16_t  second;
    uint16_t  minute;
    uint16_t  hour;
    uint16_t  day;
    uint16_t  month;
    uint16_t  year;
} RtcTime;
void rtc_getRTCTime(RtcTime* timebuf);
uint32_t rtc_setRTCTime(RtcTime* newTime);

#endif

/*
 * $ Copyright 2003-YEAR Cypress Semiconductor $
 */
/********************************************************************
*    File Name: Types.h
*
*    Abstract: Basic type definitions for the BCM204X Baseband Controller
*              Firmware
*
*    $History:$
*
********************************************************************
*/

#ifndef P_256_TYPES_H
#define P_256_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

//
// Basic Types
//
#if !defined(__STDINT_DECLS)

#if defined(WIN32)
typedef char                CHAR;
typedef short               SHORT;
typedef long                LONG;
#if 0
typedef unsigned char       uint8_t;
typedef signed   char       int8_t;
typedef unsigned short      uint16_t;
typedef signed   short      int16_t;
typedef unsigned int        uint32_t;
typedef signed   int        int32_t;
typedef unsigned __int64    uint64_t;
#endif
typedef signed   __int64    INT64;

typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;

typedef unsigned char       uint8;
typedef signed   char       int8;
typedef unsigned short      uint16;
typedef signed   short      int16;
typedef unsigned long       uint32;
typedef signed   long       int32;

#else

#if 0
typedef unsigned char       uint8_t;
typedef signed   char       int8_t;
typedef unsigned short      uint16_t;
typedef signed   short      int16_t;
typedef unsigned long       uint32_t;
typedef signed   long       int32_t;
typedef unsigned long long  uint64_t;
#endif

typedef unsigned char       uint8;
typedef signed   char       int8;
typedef unsigned short      uint16;
typedef unsigned long       uint32;
typedef signed   long       int32;

#endif // win32

#endif

// Machine dependent data types, for processor efficiency
typedef unsigned int MBOOL;
typedef unsigned int MU8;
typedef unsigned int MU16;
typedef unsigned int MU32;
typedef int MI8;
typedef int MI16;
typedef int MI32;

#ifndef NULL
#define NULL 0
#endif


//
// Rounding macros
//
#define ROUND2(n)       (((n) + 1) & ~1)
#define ROUND4(n)       (((n) + 3) & ~3)
#define ROUND8(n)       (((n) + 7) & ~7)

//
// inline macro
//

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
//
// #define FOO FREESODA
// printf("%s    %s\n", STRINGIFY(FOO), TOSTRING(FOO));
//
// The code above will generate the following output:
//        FOO    FREESODA

//
// Assume we are compiled under Little Endian system.
#ifndef BE_SWAP_16
    #define BE_SWAP_16(var)         (( uint16_t)( ((var) << 8) | (((var) >> 8) & 0xff) ) )
#endif

#ifndef LE_SWAP_16
    #define LE_SWAP_16(var)  (var)
#endif

#ifndef BE_SWAP_32
    #define BE_SWAP_32(var)  (     ((var & 0x000000FF)<<24)  \
                               |   ((var & 0x0000FF00)<<8)   \
                               |   ((var & 0x00FF0000)>>8)   \
                               |   ((var & 0xFF000000)>>24)  \
                             )
#endif

#ifndef LE_SWAP_32
  #define LE_SWAP_32(var)    (var)
#endif

#ifndef MAX
    #define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif

#ifndef MIN
    #define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif

#ifdef __cplusplus
}
#endif

/*
*******************************************************************************
*
* $HISTORY:$
*
*******************************************************************************
*/
#endif /* P_256_TYPES_H */

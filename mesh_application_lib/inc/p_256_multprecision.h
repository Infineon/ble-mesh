/*
 * $ Copyright 2006-YEAR Cypress Semiconductor $
 */
/*********************************************************************
 *    File Name: multprecision.h
 *
 *    Abstract: simple pairing algorithms
 *
 *    Functions:
 *            --
 *
 *    $History:$
 *
 ********************************************************************
*/
#ifndef P_256_MULTPRECISION_H
#define P_256_MULTPRECISION_H

#include "bt_types.h"
#include "p_256_types.h"

/* Type definitions */

#define DWORD_BITS      32
#define DWORD_BYTES     4
#define DWORD_BITS_SHIFT 5

#define KEY_LENGTH_DWORDS_P192 6
#define KEY_LENGTH_DWORDS_P256 8
/* Arithmetic Operations */


#ifdef __cplusplus
extern "C" {
#endif

int MP_CMP(const DWORD *a, const DWORD *b, uint32_t keyLength);
int MP_isZero(DWORD *a, uint32_t keyLength);
void MP_Init(DWORD *c, uint32_t keyLength);
void MP_Copy(DWORD *c, const DWORD *a, uint32_t keyLength);
uint32_t MP_DWORDBits (DWORD a);
uint32_t MP_MostSignDWORDs(DWORD *a, uint32_t keyLength);
uint32_t MP_MostSignBits(DWORD *a, uint32_t keyLength);
void MP_InvMod(DWORD *aminus, DWORD *a, uint32_t keyLength);

DWORD MP_Add(DWORD *c, const DWORD *a, const DWORD *b, uint32_t keyLength);           // c=a+b
void MP_AddMod(DWORD *c, DWORD *a, DWORD *b, uint32_t keyLength);
DWORD MP_Sub(DWORD *c, const DWORD *a, const DWORD *b, uint32_t keyLength);           // c=a-b
void MP_SubMod(DWORD *c, const DWORD *a, const DWORD *b, uint32_t keyLength);
void MP_RShift(DWORD * c, DWORD * a, uint32_t keyLength);                 // c=a>>1, return carrier
void MP_LShiftMod(DWORD * c, DWORD * a, uint32_t keyLength);              // c=a<<b, return carrier
DWORD MP_LShift(DWORD * c, DWORD * a, uint32_t keyLength);                // c=a<<b, return carrier
void MP_Mult_int(DWORD *c, DWORD *a, DWORD *b, uint32_t keyLength);           // c=a*b
void MP_MersennsMultMod(DWORD *c, DWORD *a, DWORD *b, uint32_t keyLength);
void MP_MersennsSquaMod(DWORD *c, DWORD *a, uint32_t keyLength);
DWORD MP_LShift(DWORD * c, DWORD * a, uint32_t keyLength);
void MP_FastMod_P256(DWORD *c, DWORD *a);

#ifdef __cplusplus
}
#endif



#endif /* P_256_MULTPRECISION_H */

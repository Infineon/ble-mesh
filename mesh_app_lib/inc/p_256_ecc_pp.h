/*
********************************************************************
* THIS INFORMATION IS PROPRIETARY TO
* BROADCOM CORP.
*-------------------------------------------------------------------
*
*           Copyright (c) 2006 Broadcom Corp.
*                      ALL RIGHTS RESERVED
*
********************************************************************

 ********************************************************************
 *    File Name: ecc_pp.h
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

#ifndef P_256_ECC_PP_H
#define P_256_ECC_PP_H

#include "p_256_multprecision.h"

#define width 4

struct _point
{
    DWORD x[KEY_LENGTH_DWORDS_P256];
    DWORD y[KEY_LENGTH_DWORDS_P256];
    DWORD z[KEY_LENGTH_DWORDS_P256];
};
typedef struct _point Point;

typedef struct
{
    // curve's coefficients
    DWORD b[KEY_LENGTH_DWORDS_P256];

    // prime modulus
    DWORD p[KEY_LENGTH_DWORDS_P256];

    // base point, a point on E of order r
    Point G;

}EC;

#ifdef __cplusplus
extern "C" {
#endif

extern EC  *p_curve;
extern EC  *p_curve_p256;

void ECC_PM_B_NAF(Point *q, Point *p, DWORD *n, uint32_t keyLength);

#define ECC_PM(q, p, n, keyLength)  ECC_PM_B_NAF(q, p, n, keyLength)

void InitCurve(uint32_t keyLength);
void InitPoint(Point *q);

#ifdef __cplusplus
}
#endif

#endif /* P_256_ECC_PP_H */

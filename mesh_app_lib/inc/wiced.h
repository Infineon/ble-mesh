/*
 * $ Copyright Cypress Semiconductor $
 */

/****************************************************************************
**
** Name:         wiced.h
**
** Description:  wiced header file for VS2010 projects
**
**
******************************************************************************/
#ifndef WICED_H
#define WICED_H

#include "wiced_data_types.h"
#include "wiced_result.h"
#include "stdint.h"

#define WICED_SUPPRESS_WARNINGS(m) if((m)){;}

/* Suppress unused variable warning */
#ifndef UNUSED_VARIABLE
#define UNUSED_VARIABLE(x) /*@-noeffect@*/ ( (void)(x) ) /*@+noeffect@*/
#endif

#ifndef WICED_FALSE
#define WICED_FALSE 0
#endif // !WICED_FALSE
#ifndef WICED_TRUE
#define WICED_TRUE  1
#endif // !WICED_TRUE
#define WPRINT_BT_APP_INFO(info)   {printf info;}

#endif //WICED_H


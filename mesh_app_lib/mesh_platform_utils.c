/******************************************************************************
* File Name:   mesh_platform_utils.h
*
* Description: This file has platform level utillity function implementation
*
* Related Document: See README.md
*
*******************************************************************************
* $ Copyright 2021-YEAR Cypress Semiconductor $
*******************************************************************************/
#include <unistd.h>
#include "stdio.h"
#include "stdlib.h"
#include <stdarg.h>
#include "time.h"
#include <sys/timeb.h>
#include "cyhal.h"

#include "p_256_ecc_pp.h"
#include "mesh_platform_utils.h"
#include "wiced_bt_mesh_model_defs.h"
#include "wiced_bt_mesh_models.h"
#include "mesh_cfg.h"

#if 0 /* BTSTACK > 3.1*/
EC curve_p256;
EC* p_curve_p256 = &curve_p256;
#endif
uint16_t wiced_bt_mesh_core_nvm_idx_fw_distributor;

uint32_t cy_get_rand()
{
	cy_rslt_t    rslt = CY_RSLT_SUCCESS;

	cyhal_trng_t trng_obj;
	uint32_t     rnd_num = 0;
	// Initialize the true random number generator block
	rslt = cyhal_trng_init(&trng_obj);
	if(CY_RSLT_SUCCESS == rslt)
	{
		// Generate a true random number
		rnd_num = cyhal_trng_generate(&trng_obj);
	}
	// Release the true random number generator block
	// Note: Free only if not required anymore
	cyhal_trng_free(&trng_obj);

	return rnd_num;
}


uint32_t wiced_hal_get_pseudo_rand_number(void)
{
	return mesh_util_rand_gen_num();
}

uint32_t wiced_hal_rand_gen_num(void)
{

	return mesh_util_rand_gen_num();
}

void wiced_hal_wdog_reset_system(void)
{

}

void wiced_hal_rand_gen_num_array(uint32_t* randNumberArrayPtr, uint32_t length)
{
    uint32_t  xx;
    for (xx = 0; xx < length; xx++)
        *randNumberArrayPtr++ = (uint32_t)cy_get_rand();
}

void mesh_util_rand_gen_num_array(uint32_t* randNumberArrayPtr, uint32_t length)
{
    uint32_t  xx;
    for (xx = 0; xx < length; xx++)
        *randNumberArrayPtr++ = (uint32_t)cy_get_rand();
}

uint32_t mesh_util_rand_gen_num(void)
{
    return (uint32_t)cy_get_rand();
}

void utilslib_delayUs(uint32_t delay)
{
    sleep(delay / 1000);
}

void mesh_trace_debug(const char* p_fmt, ...)
{
    va_list ap;

    va_start(ap, p_fmt);
    vprintf(p_fmt, ap);
    va_end(ap);
}


/*
 * The function goes through all models of all elements and returns number of elements on which specified model is present
 */
uint8_t mesh_util_get_number_of_elements(uint16_t model_id)
{
    uint8_t element_idx;
    uint8_t model_idx;
    uint8_t num_elements_with_model = 0;

    for (element_idx = 0; element_idx < mesh_config.elements_num; element_idx++)
    {
        for (model_idx = 0; model_idx < mesh_config.elements[element_idx].models_num; model_idx++)
        {
            if ((mesh_config.elements[element_idx].models[model_idx].company_id == MESH_COMPANY_ID_BT_SIG) &&
                (mesh_config.elements[element_idx].models[model_idx].model_id == model_id))
            {
                num_elements_with_model++;
                break;
            }
        }
    }
    return (num_elements_with_model);
}



/******************************************************************************
* File Name:   mesh_platform_utils.h
*
* Description: This file is the public interface of mesh_platform_utils.c
*
* Related Document: See README.md
*
*******************************************************************************
* $ Copyright 2021-YEAR Cypress Semiconductor $
*******************************************************************************/
#ifndef MESH_PLATFORM_UTILS_H_
#define MESH_PLATFORM_UTILS_H_

void mesh_trace_debug(const char* p_fmt, ...);

#ifdef WICED_BT_TRACE
#undef WICED_BT_TRACE
	#define WICED_BT_TRACE          mesh_trace_debug
#endif

/**
 * @brief Defines the first usable NVRAM Volatile Section Identifier.
 * Application can use the VS ids starting from WICED_NVRAM_VSID_START.to
 * WICED_NVRAM_VSID_END
 * Note: Values less than 0x200 reserved for WICED internal purpose and not advisable to use by applications
 **/
enum
{
    /* Wiced applications */
    WICED_NVRAM_VSID_START              = 0x200,
    WICED_NVRAM_VSID_END                = 0x3FFF
};


/*****************************************************************************
**                          Function Declarations
*****************************************************************************/
void mesh_util_wdog_reset_system(void);
void mesh_util_print_heap_usage(void);
void mesh_util_get_rand_gen_num_array(uint32_t* randNumberArrayPtr, uint32_t length);
uint32_t mesh_util_get_rand_gen_num(void);
uint32_t mesh_util_get_pseudo_rand_number(void);
uint8_t mesh_util_get_number_of_elements(uint16_t model_id);


/**
 * Writes the data to NVRAM,
 * Application can write up to 255 bytes in one VS  id section
 *
 * @param[in] vs_id        : Volatile Section Identifier. Application can use
 *                           the VS ids from WICED_NVRAM_VSID_START to
 *                           WICED_NVRAM_VSID_END
 *
 * @param[in] data_length  : Length of the data to be written to the NVRAM,
 *
 * @param[in] p_data       : Pointer to the data to be written to the NVRAM
 *
 * @param[out] p_status    : Pointer to location where status of the call
 *                           is returned
 *
 *
 * @return  number of bytes written, 0 on error
 */
uint16_t wiced_hal_write_nvram( uint16_t vs_id,
            uint16_t         data_length,
        uint8_t        * p_data,
        wiced_result_t * p_status);

/** Reads the data from NVRAM
 *
 * @param[in]  vs_id       : Volatile Section Identifier. Application can use
 *                           the VS ids from WICED_NVRAM_VSID_START to
 *                           WICED_NVRAM_VSID_END
 *
 * @param[in]  data_length : Length of the data to be read from NVRAM
 *
 * @param[out] p_data      : Pointer to the buffer to which data will be copied
 *
 * @param[out] p_status    : Pointer to location where status of the call
 *                           is returned
 *
 * @return  the number of bytes read, 0 on failure
 */
uint16_t wiced_hal_read_nvram( uint16_t vs_id,
            uint16_t         data_length,
        uint8_t        * p_data,
        wiced_result_t * p_status);

/** deletes data from NVRAM at specified VS id
 *
 * @param[in]  vs_id       : Volatile Section Identifier. Application can use
 *                           the VS ids from WICED_NVRAM_VSID_START to
 *                           WICED_NVRAM_VSID_END
 *
 *
 * @param[out] p_status    : Pointer to location where status of the call
 *                           is returned
 *
 * @return  void
 */
void wiced_hal_delete_nvram( uint16_t vs_id, wiced_result_t * p_status);


extern void mesh_initialize_random_seed();

#endif /* MESH_PLATFORM_UTILS_H_ */

/******************************************************************************
* File Name:   mesh_platform_utils.c
*
* Description: This file has platform level utility function implementation
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
#include <malloc.h>
#include "time.h"
#include <sys/timeb.h>
#include "cyhal.h"

#include "p_256_ecc_pp.h"
#include "mesh_platform_utils.h"

#include "wiced_bt_factory_app_config.h"
#include "wiced_bt_mesh_model_defs.h"
#include "wiced_bt_mesh_models.h"
#include "mtb_kvstore.h"
#include "flash_utils.h"
#include "hcidefs.h"
#include "wiced_bt_trace.h"

uint16_t wiced_bt_mesh_core_nvm_idx_fw_distributor;

static uint32_t wiced_hal_get_trng_rand(void);

extern BOOL32 btsnd_hcic_ble_rand (void *p_cmd_cplt_cback);
extern void mesh_application_init(void);
extern void mesh_application_factory_reset(void);
/*****************************************************************************
**  Constants
*****************************************************************************/
#define SS_READ_LIMIT (8 * 1024)
#define SS_READ_CHUNK (32)
#define TO_KB(size_bytes)  ((float)(size_bytes)/1024)
/*****************************************************************************
**  Data types
*****************************************************************************/
typedef enum
{
    SS_SEEK_FE,
    SS_SEEK_00_1,
    SS_SEEK_00_2,
    SS_SEEK_TYPE,
    SS_TYPE_LEN1,
    SS_TYPE_LEN2,
    SS_SKIP_LEN1,
    SS_SKIP_LEN2,
    SS_SKIP,
    SS_COPY,
    SS_DONE
} static_data_parse_state;


#define BT_OCTET8_LEN    8      /**< octet length: 8 */
#define BT_OCTET16_LEN    16                /**< octet length: 16 */

/* Structure returned with Rand/Encrypt complete callback */
typedef struct
{
    uint8_t   status;
    uint8_t   param_len;
    uint16_t  opcode;
    uint8_t   param_buf[BT_OCTET16_LEN];
} tBTM_RAND_ENC;


#ifndef WICED_FALSE
#define WICED_FALSE 0   /**< Wiced false */
#endif // !WICED_FALSE
#ifndef WICED_TRUE
#define WICED_TRUE  1   /**< Wiced true */
#endif // !WICED_TRUE


void mesh_rand_back(tBTM_RAND_ENC *p)
{
	uint8_t er[16]={0};
	uint8_t* pr;
	uint32_t randseed;

   if (p && p->opcode == HCI_BLE_RAND)
   {
       memcpy(er, p->param_buf, BT_OCTET8_LEN);
       pr = &p->param_buf[0];
       STREAM_TO_UINT32(randseed, pr);
       printf("mesh_rand_back - randseed is %u\n" , (unsigned int) randseed);
       srand(randseed);
       rand();

       // Initialize Mesh after random seed has been set.
       mesh_application_init();
   }
}

void mesh_initialize_random_seed()
{
	BOOL32 bval = 0;

	// Get LE Rand to initialize as seed and then initialize mesh app.
	bval = btsnd_hcic_ble_rand((void *)mesh_rand_back);

	printf("mesh_initialize_random_seed - bval is %d\n", bval);
	if (!bval)
	{
		printf("mesh_initialize_random_seed failed");
		return;
	}
}

void wiced_hal_init_rand_gen(void)
{
	// Nothing here..
}

uint32_t wiced_hal_get_trng_rand(void)
{
#if 0
	cy_rslt_t    rslt = CY_RSLT_SUCCESS;

    cyhal_trng_t trng_obj;
    uint32_t     trng_rnd_num = 0;
    // Initialize the true random number generator block
    rslt = cyhal_trng_init(&trng_obj);


    if(CY_RSLT_SUCCESS == rslt)
    {
        // Generate a true random number
	trng_rnd_num = cyhal_trng_generate(&trng_obj);
    }
    // Release the true random number generator block
    // Note: Free only if not required anymore
    cyhal_trng_free(&trng_obj);
#endif

    wiced_hal_init_rand_gen();
    return (uint32_t)rand();
    //return trng_rnd_num;
}

void wiced_hal_rand_gen_num_array(uint32_t* randNumberArrayPtr, uint32_t length)
{
    uint32_t  xx;
    for (xx = 0; xx < length; xx++)
        *randNumberArrayPtr++ = (uint32_t)rand();
}

uint32_t wiced_hal_rand_gen_num(void)
{
	uint32_t rval = (uint32_t)rand();
	return rval;
}

uint32_t wiced_hal_get_pseudo_rand_number(void)
{
	uint32_t rval = (uint32_t)rand();
	return rval;
}

// Initialize the system watchdog
//
static wiced_bool_t bWatchDogInitialized = WICED_FALSE;
static cyhal_wdt_t wdt_obj;
void wiced_hal_wdog_init(uint32_t timeout_ms)
{
    cy_rslt_t  result = 0;

    // Initialize the watchdog timer
    if (timeout_ms != 0)
    {
        result = cyhal_wdt_init(&wdt_obj, timeout_ms);
        bWatchDogInitialized = WICED_TRUE;
    }

    WICED_BT_TRACE("[%s]  Time: %d ms (0 == no watchdog) Result: %d  Max: %d\n", __FUNCTION__, timeout_ms, result, cyhal_wdt_get_max_timeout_ms());

}

// Called to reboot the system, using the wathdog
//
void wiced_hal_wdog_reset_system(void)
{
    uint32_t    int_state;

    WICED_BT_TRACE("[%s]  Initialized: %d\n", __FUNCTION__, bWatchDogInitialized);

    if (bWatchDogInitialized)
        cyhal_wdt_free(&wdt_obj);

    // Time for the trace to get out
    wiced_hal_wdog_init(100);

    // wait here
    int_state = Cy_SysLib_EnterCriticalSection();

    while (1)
    {
        cy_rtos_delay_milliseconds(1);
    }

    Cy_SysLib_ExitCriticalSection(int_state);

    // Should never get here....
    WICED_BT_TRACE("[%s] ??? end\n", __FUNCTION__);
    cy_rtos_delay_milliseconds(1000);
}

void mesh_util_wdog_reset_system(void)
{
    mesh_application_factory_reset();
}

wiced_bool_t wiced_bt_notify_multi_advertisement_packet_transmissions(uint8_t adv_instance, void (*clientCallback)(uint32_t), uint32_t advanceNoticeInMicroSeconds)
{
    return FALSE;
}

void utilslib_delayUs(uint32_t delay)
{
#ifdef WIN32
    Sleep(delay / 1000);
#else
    cyhal_system_delay_us(delay);
#endif

}

void wiced_transport_free_buffer(void* p)
{
}


/** Load data from a certain location on the serial flash module into
* memory. To have a better read performance, place the destination data
* buffer at a word boundary.
*
* \param addr - The starting source address on the serial flash.
* \param len  - The number of bytes to read.
* \param buf  - Pointer to destination data buffer.
*
* \return The number of bytes read.
*/
uint32_t wiced_hal_sflash_read(uint32_t addr, uint32_t len, uint8_t* buf)
{
    return 0;
}

/**
 * @brief Read record from manufacturer static memory
 *
 * @param item_type     - record designated tag
 * @param buffer        - pointer to the memory buffer to store the record
 * @param read_length   - number of bytes to read
 * @param read_offset   - start offset to read the record
 * @return uint16_t     - number of bytes that was actually read to the buffer
 */
uint16_t wiced_bt_factory_config_read(
    uint8_t item_type, uint8_t *buffer, uint16_t read_length, uint16_t read_offset, uint16_t *record_size)
{
    uint16_t i;
    uint16_t copy_len = 0;
    uint16_t len = 0;
    uint16_t offset = 0;
    static_data_parse_state state = SS_SEEK_FE;
    uint8_t flash_read_buffer[SS_READ_CHUNK];
    *record_size = 0;

    if ((item_type >= WICED_BT_FACTORY_CONFIG_ITEM_FIRST) && (item_type <= WICED_BT_FACTORY_CONFIG_ITEM_LAST)) {
        while ((state != SS_DONE) && (offset < (SS_READ_LIMIT))) {
            if (sizeof(flash_read_buffer) !=
                wiced_hal_sflash_read(offset, sizeof(flash_read_buffer), flash_read_buffer))

            {
                printf("bad flash read\r\n");
                break;
            }
            offset += sizeof(flash_read_buffer);
            for (i = 0; i < sizeof(flash_read_buffer); i++) {
                uint8_t byte = flash_read_buffer[i];
                //WICED_BT_TRACE("flash @%d = %02x, state %d\n", offset+i, byte, state);
                switch (state) {
                    case SS_SEEK_FE:
                        if (byte == 0xfe) {
                            state = SS_SEEK_00_1;
                        }
                        break;
                    case SS_SEEK_00_1:
                        state = (byte == 0) ? SS_SEEK_00_2 : SS_SEEK_FE;
                        break;
                    case SS_SEEK_00_2:
                        state = (byte == 0) ? SS_SEEK_TYPE : SS_SEEK_FE;
                        break;
                    case SS_SEEK_TYPE:
                        state = (byte == item_type)                 ? SS_TYPE_LEN1
                                : ((byte != 0xFF) && (byte > 0x80)) ? SS_SKIP_LEN1
                                                                    : SS_DONE;
                        break;
                    case SS_TYPE_LEN1:
                        len = byte;
                        state = SS_TYPE_LEN2;
                        break;
                    case SS_SKIP_LEN1:
                        len = byte;
                        state = SS_SKIP_LEN2;
                        break;
                    case SS_TYPE_LEN2:
                        len += byte << 8;
                        *record_size = len;
                        state = (read_offset >= len) ? SS_DONE : SS_COPY;
                        len = ((read_length + read_offset) <= len) ? read_length : len - read_offset;
                        break;
                    case SS_SKIP_LEN2:
                        len += byte << 8;
                        state = SS_SKIP;
                        break;
                    case SS_SKIP:
                        if (--len == 0) {
                            state = SS_SEEK_TYPE;
                        }
                        break;
                    case SS_COPY:
                        if (0 == read_offset) {
                            if (len-- == 0) {
                                state = SS_DONE;
                            }
                            else {
                                *buffer++ = byte;
                                copy_len++;
                            }
                        }
                        else {
                            --read_offset;
                        }
                        break;
                    case SS_DONE:
                        break;
                    default:
                        state = SS_SEEK_FE;
                        break;
                }
            }
        }
    }
    return copy_len;
}

/**
 * @brief Get list of provisioning records from manufacturer static memory
 *
 * @param buffer[out]        - pointer to the memory buffer to store the record list
 * @return uint16_t          - number of found provisioning records
 */
uint16_t wiced_bt_factory_config_provisioning_records_get(uint16_t *buffer)
{
    uint16_t count = 0;
    for (uint16_t i = 0; i < WICED_BT_FACTORY_PROVISIONING_RECORD_SIZE; i++) {
        /* try to read only one first byte of the record to probe */
        uint16_t record_size;
        uint8_t byte;
        if (0 != wiced_bt_factory_config_read((WICED_BT_FACTORY_PROVISIONING_RECORD_OFFSET + i) & 0xff,
                                              &byte,
                                              1,
                                              0,
                                              &record_size))
        {
            *buffer++ = i;
            count++;
        }
    }
    return count;
}

/**
 * @brief Get provisioning record from manufacturer static memory
 *
 * @param[in]  item_type        - record designated tag
 * @param[out] buffer           - pointer to the memory buffer to store the record
 * @param[in]  fragment_length  - number of bytes to read
 * @param[in]  fragment_offset  - start offset to read the record
 * @param[out] record_size      - pointer to the output parameter to store total record size
 * @return uint16_t             - number of bytes that was actually read to the buffer
 */
uint16_t wiced_bt_factory_config_provisioning_record_req(
    uint16_t record_id, uint8_t *buffer, uint16_t fragment_length, uint16_t fragment_offset, uint16_t *record_size)
{
    uint8_t item_type = (uint8_t)((record_id + WICED_BT_FACTORY_PROVISIONING_RECORD_OFFSET) & 0xff);
    return wiced_bt_factory_config_read(item_type, buffer, fragment_length, fragment_offset, record_size);
}

uint16_t wiced_hal_write_nvram (uint16_t vs_id, uint16_t data_length, uint8_t *p_data, wiced_result_t *p_status)
{
    flash_memory_write(vs_id, data_length, p_data, p_status);

    return (data_length);
}

uint16_t wiced_hal_read_nvram (uint16_t vs_id, uint16_t data_length, uint8_t *p_data, wiced_result_t *p_status)
{
    uint16_t  len_read;

    len_read = flash_memory_read(vs_id, data_length, p_data, p_status);

   return ((uint16_t)len_read);
}

void wiced_hal_delete_nvram (uint16_t vs_id, wiced_result_t* p_status)
{
	*p_status = flash_memory_delete(vs_id);
}

void mesh_util_get_rand_gen_num_array(uint32_t* randNumberArrayPtr, uint32_t length)
{
    uint32_t  xx;
    for (xx = 0; xx < length; xx++)
        *randNumberArrayPtr++ = (uint32_t)rand();
}

uint32_t mesh_util_get_rand_gen_num(void)
{
    return (uint32_t)wiced_hal_get_trng_rand();
}

uint32_t mesh_util_get_pseudo_rand_number(void)
{
    wiced_hal_init_rand_gen();
    return (uint32_t)rand();
}

void mesh_trace_debug(const char* p_fmt, ...)
{
#if defined(WICED_BT_MESH_TRACE_ENABLE) && (WICED_BT_MESH_TRACE_ENABLE == TRUE)
    va_list ap;

    va_start(ap, p_fmt);
    vprintf(p_fmt, ap);
    va_end(ap);
#endif
}

/*******************************************************************************
* Function Name: print_heap_usage
********************************************************************************
* Summary:
* Prints the available heap and utilized heap by using mallinfo().
*
*******************************************************************************/
void mesh_util_print_heap_usage(void)
{
    /* ARM compiler also defines __GNUC__ */
    struct mallinfo mall_info = mallinfo();

    extern uint8_t __HeapBase;  /* Symbol exported by the linker. */
    extern uint8_t __HeapLimit; /* Symbol exported by the linker. */

    uint8_t* heap_base = (uint8_t *)&__HeapBase;
    uint8_t* heap_limit = (uint8_t *)&__HeapLimit;
    uint32_t heap_size = (uint32_t)(heap_limit - heap_base);

    printf("Total available heap        : %u bytes/%.2f KB\r\n", (unsigned int) heap_size, TO_KB(heap_size));

    printf("Maximum heap utilized so far: %u bytes/%.2f KB, %.2f%% of available heap\r\n",
            mall_info.arena, TO_KB(mall_info.arena), ((float) mall_info.arena * 100u)/heap_size);

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

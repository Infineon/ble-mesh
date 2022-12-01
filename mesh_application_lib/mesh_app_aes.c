/*
 * $ Copyright 2016-YEAR Cypress Semiconductor $
 */

/** @file
 *
 * Mesh AES encryption related functionality
 *
 */

#include "bt_types.h"
#include "wiced_bt_gatt.h"

#if !defined (MESH_HOST_MODE)
#include "wiced_hal_aes.h"
#endif

#include "wiced_bt_mesh_core.h"
#include "mesh_application.h"
#include "wiced_bt_trace.h"

// ---------------------------------- Code for WICED layer
#if defined(CYW20706A2)
static void wiced_request_aes_exclusive_access()
{
}

static void wiced_release_aes_exclusive_access()
{
}
#elif !defined(CYW20829) && !defined (MESH_HOST_MODE)
extern unsigned int _tx_v7m_get_and_disable_int(void);
extern void _tx_v7m_set_int(unsigned int posture);
extern void bcs_pmuWaitForBtClock(void);
extern void bcs_pmuReleaseBtClock(void);


static unsigned int interrupt_save;
#define osapi_LOCK_CONTEXT          { interrupt_save = _tx_v7m_get_and_disable_int(); };
#define osapi_UNLOCK_CONTEXT        { _tx_v7m_set_int(interrupt_save); };

static void wiced_request_aes_exclusive_access()
{
    bcs_pmuWaitForBtClock();
    osapi_LOCK_CONTEXT;
}

static void wiced_release_aes_exclusive_access()
{
    osapi_UNLOCK_CONTEXT;
    bcs_pmuReleaseBtClock();
}
#else

#endif
// ---------------------------------- End of code WICED layer

// 20829 Support for HW AES

#define USE_SMIF_HW_AES

#if defined(USE_SMIF_HW_AES)
#undef USE_SMP_AES_ENCRYPT
#endif


#if defined(USE_SMIF_HW_AES)
#undef USE_SMP_AES_ENCRYPT

#include "cy_smif.h"
#define CRYPTO_KEY_SIZE 16
cy_en_smif_status_t smif_hw_aes_encrypt(uint8_t *in_data, uint8_t *out_data, uint8_t *key);
#endif

void wiced_aes_profile_report(void);


#if defined(USE_SMIF_HW_AES)
CY_SECTION_RAMFUNC_BEGIN
void Cy_SMIF_SetMode_ram(SMIF_Type *base, cy_en_smif_mode_t mode)
{
    CY_ASSERT_L3(CY_SMIF_MODE_VALID(mode));

    /*  Set the register SMIF.CTL.XIP_MODE = TRUE */
    if (CY_SMIF_NORMAL == mode)
    {
        SMIF_CTL(base) &= ~ SMIF_CTL_XIP_MODE_Msk;
    }
    else
    {
#if ((CY_IP_MXSMIF_VERSION==2) || (CY_IP_MXSMIF_VERSION==3))
        uint32_t read_cmd_data_ctl;
        uint8_t idx;

        /* Context variable is not available in this API. To make the API backward compatible
         * we search if any of the device uses XIP and pick the data rate from that device.
         * Multiple devices supporting XIP mode is not supported with the version of driver.
         */

        for(idx = 0U; idx < SMIF_DEVICE_NR; idx++)
        {
            read_cmd_data_ctl = SMIF_DEVICE_IDX_RD_DATA_CTL(base, idx);

            if ((read_cmd_data_ctl & SMIF_DEVICE_RD_DATA_CTL_DDR_MODE_Msk) != 0UL)
            {
                    uint32_t temp;
                    /* Select TX Clock mode SDR/DDR */
                    temp = SMIF_CTL(base);
                    temp &= ~(SMIF_CTL_CLOCK_IF_TX_SEL_Msk);
                    SMIF_CTL(base) =  temp | _VAL2FLD(SMIF_CTL_CLOCK_IF_TX_SEL, CY_SMIF_DDR);
                    break;
            }
        }
#endif /* CY_IP_MXSMIF_VERSION */
        SMIF_CTL(base) |= SMIF_CTL_XIP_MODE_Msk;
    }
}
CY_SECTION_RAMFUNC_END

/* if encrypted XIP is being used, set cryptoKeyXip to XIP key value */
static uint8_t cryptoKeyXip[CRYPTO_KEY_SIZE] = {0,0,0,0,0,0,0,0,
                                                0,0,0,0,0,0,0,0};
uint32_t smif_hw_aes_encrypt_count = 0;

CY_SECTION_RAMFUNC_BEGIN
cy_en_smif_status_t smif_hw_aes_encrypt(uint8_t *in_data, uint8_t *out_data, uint8_t *key)
{
  //  printf("smif_hw_aes_encrypt in_data: 0x%x, out_data: 0x%x key: 0x%x\n", in_data, out_data, key);

    cy_en_smif_status_t SMIF_Status = CY_SMIF_BAD_PARAM;
    uint32_t timeoutUnits = 1000;
    uint32_t input_context[4];
    /* store SMIF mode */
    cy_en_smif_mode_t smif_mode_context = (SMIF0->CTL & SMIF_CTL_XIP_MODE_Msk) ? CY_SMIF_MEMORY : CY_SMIF_NORMAL;
    /* Set SMIF mode to MMIO or non-XIP */
    Cy_SMIF_SetMode_ram(SMIF0, CY_SMIF_NORMAL);

    /* load encryption key; these registers are write-only */
    SMIF_CRYPTO_KEY0(SMIF0) = ((uint32_t *)key)[0];
    SMIF_CRYPTO_KEY1(SMIF0) = ((uint32_t *)key)[1];
    SMIF_CRYPTO_KEY2(SMIF0) = ((uint32_t *)key)[2];
    SMIF_CRYPTO_KEY3(SMIF0) = ((uint32_t *)key)[3];
    /* store hardware context data registers */
    input_context[0] = SMIF_CRYPTO_INPUT0(SMIF0);
    input_context[1] = SMIF_CRYPTO_INPUT1(SMIF0);
    input_context[2] = SMIF_CRYPTO_INPUT2(SMIF0);
    input_context[3] = SMIF_CRYPTO_INPUT3(SMIF0);
    /* load data to encrypt */
    SMIF_CRYPTO_INPUT0(SMIF0) = ((uint32_t *)in_data)[0];
    SMIF_CRYPTO_INPUT1(SMIF0) = ((uint32_t *)in_data)[1];
    SMIF_CRYPTO_INPUT2(SMIF0) = ((uint32_t *)in_data)[2];
    SMIF_CRYPTO_INPUT3(SMIF0) = ((uint32_t *)in_data)[3];
    /* Start the encryption */
    SMIF_CRYPTO_CMD(SMIF0) &= ~SMIF_CRYPTO_CMD_START_Msk;
    SMIF_CRYPTO_CMD(SMIF0) = (uint32_t)(_VAL2FLD(SMIF_CRYPTO_CMD_START,
                                            CY_SMIF_CRYPTO_START));

    while((CY_SMIF_CRYPTO_COMPLETED != _FLD2VAL(SMIF_CRYPTO_CMD_START,
                                            SMIF_CRYPTO_CMD(SMIF0))) &&
                                            (CY_SMIF_EXCEED_TIMEOUT != SMIF_Status))
    {
        /* Wait until the encryption is completed and check the timeout */
        SMIF_Status = Cy_SMIF_TimeoutRun(&timeoutUnits);
    }
    /* copy encrypted data to caller */
    if (CY_SMIF_EXCEED_TIMEOUT != SMIF_Status)
    {
        ((uint32_t *)out_data)[0] = SMIF_CRYPTO_OUTPUT0(SMIF0);
        ((uint32_t *)out_data)[1] = SMIF_CRYPTO_OUTPUT1(SMIF0);
        ((uint32_t *)out_data)[2] = SMIF_CRYPTO_OUTPUT2(SMIF0);
        ((uint32_t *)out_data)[3] = SMIF_CRYPTO_OUTPUT3(SMIF0);
        SMIF_Status = CY_SMIF_SUCCESS;
        smif_hw_aes_encrypt_count++;
    }
    /* restore encryption key */
    SMIF_CRYPTO_KEY0(SMIF0) = ((uint32_t *)cryptoKeyXip)[0];
    SMIF_CRYPTO_KEY1(SMIF0) = ((uint32_t *)cryptoKeyXip)[1];
    SMIF_CRYPTO_KEY2(SMIF0) = ((uint32_t *)cryptoKeyXip)[2];
    SMIF_CRYPTO_KEY3(SMIF0) = ((uint32_t *)cryptoKeyXip)[3];
    /* restore original input data */
    SMIF_CRYPTO_INPUT0(SMIF0) = input_context[0];
    SMIF_CRYPTO_INPUT1(SMIF0) = input_context[1];
    SMIF_CRYPTO_INPUT2(SMIF0) = input_context[2];
    SMIF_CRYPTO_INPUT3(SMIF0) = input_context[3];
    /* restore SMIF mode */
    Cy_SMIF_SetMode_ram(SMIF0, smif_mode_context);
    return SMIF_Status;
}
CY_SECTION_RAMFUNC_END
#endif

/* for profiling aes hardware/software */
uint32_t wiced_aes_encrypt_count = 0;
#define NUM_AES_ELAPSED_SAMPLES 10
uint32_t wiced_aes_encrypt_elapsed[NUM_AES_ELAPSED_SAMPLES] = {0};
uint32_t wiced_aes_encrypt_index = 0;
uint32_t wiced_aes_encrypt_average = 0;

#include "cy_systick.h"

void wiced_aes_profile_report(void)
{
    WICED_BT_TRACE("hw encrypt count %d average ticks %d\n", wiced_aes_encrypt_count, wiced_aes_encrypt_average);
}

/*******************************************************************
 * Function         platform_hw_smif_encrypt
 *
 *                  Calculate AES encryption by using Hardware acceleration
 *
 * @param[in]       in_data     : point to input data buffer
 * @param[in|out]   out_data    : point to output data buffer
 * @param[in]       key         : point to key data buffer
 *
 * @return          void
*******************************************************************/
CY_SECTION_RAMFUNC_BEGIN
void platform_hw_smif_encrypt(uint8_t *in_data, uint8_t *out_data, uint8_t *key)
{
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t wiced_aes_encrypt_elapsed_total = 0;
    uint32_t timer_reload = Cy_SysTick_GetReload();
    uint32_t int_status = Cy_SysLib_EnterCriticalSection();

//    printf("platform_hw_smif_encrypt in_data: 0x%x, out_data: 0x%x key: 0x%x\n", in_data, out_data, key);

    t0 = Cy_SysTick_GetValue();

    smif_hw_aes_encrypt(in_data, out_data, key);

    t1 = Cy_SysTick_GetValue();
    Cy_SysLib_ExitCriticalSection(int_status);

    if(t0 >= t1)
    {
        wiced_aes_encrypt_elapsed[wiced_aes_encrypt_index] = t0 - t1;
    }
    else
    {
        wiced_aes_encrypt_elapsed[wiced_aes_encrypt_index] = t0 + timer_reload - t1;
    }
    wiced_aes_encrypt_index++;
    if(wiced_aes_encrypt_index >= NUM_AES_ELAPSED_SAMPLES)
        wiced_aes_encrypt_index = 0;

    wiced_aes_encrypt_count++;
    for(int i = 0; i < NUM_AES_ELAPSED_SAMPLES; i++)
    {
        if(wiced_aes_encrypt_elapsed[i] == 0)
            continue;
        wiced_aes_encrypt_elapsed_total += wiced_aes_encrypt_elapsed[i];
    }
    wiced_aes_encrypt_average = wiced_aes_encrypt_elapsed_total / NUM_AES_ELAPSED_SAMPLES;
}
CY_SECTION_RAMFUNC_END

CY_SECTION_RAMFUNC_BEGIN
void mesh_app_aes_encrypt(uint8_t* in_data, uint8_t* out_data, uint8_t* key)
{
#if defined(USE_SMIF_HW_AES)
    platform_hw_smif_encrypt(in_data, out_data, key);
#else
    uint8_t temp_data[WICED_BT_MESH_KEY_LEN];
    uint8_t temp_key[WICED_BT_MESH_KEY_LEN];
    int i, j;
    // Copy in_data and key to the temp buffers and revert data in these buffers
    for (i = 0, j = (WICED_BT_MESH_KEY_LEN - 1); i < WICED_BT_MESH_KEY_LEN; i++, j--)
    {
        temp_data[i] = in_data[j];
        temp_key[i] = key[j];
    }

    wiced_request_aes_exclusive_access();
    wiced_bcsulp_AES(temp_key, temp_data, out_data);
    wiced_release_aes_exclusive_access();

    // Revert data in out_data
    i = 0;
    j = WICED_BT_MESH_KEY_LEN - 1;

    while (i < j)
    {
        out_data[i] = out_data[i] ^ out_data[j];
        out_data[j] = out_data[i] ^ out_data[j];
        out_data[i] = out_data[i] ^ out_data[j];
        i++;
        j--;
    }
#endif
}
CY_SECTION_RAMFUNC_END

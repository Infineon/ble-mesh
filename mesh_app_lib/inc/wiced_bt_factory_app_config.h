/*
 * $ Copyright 2016-YEAR Cypress Semiconductor $
 */

/**************************************************************************//**
* \file
*
* \brief Provides definitions for application configuration items and API.
* \details Some per device information may be programmed in the flash during manufacturing.
* The data is saved in the Static Section of the flash and is not changed during
* the DFU or a factory reset. This interface provides standard method for an application
* to retrieve this data.
*
* Programming of the flash can be done using ss_json script which is provided
* in the tools directory of the SDK.
*
*
******************************************************************************/

#ifndef WICED_BT_FACTORY_APP_CONFIG__H
#define WICED_BT_FACTORY_APP_CONFIG__H

#ifdef __cplusplus
extern "C" {
#endif

/**
*
* \addtogroup  wiced_bt_factory_config_api_functions        Factory Config Library API
* \ingroup     wicedsys
* @{
*
* The Factory Config library of the WICED SDK provides a simple method for an application to read
* from the static section of the flash items that has been typically programmed in the factory.
*/

/**
 * @anchor WICED_BT_FACTORY_CONFIG_IDS
 * @name Factory Configuration Item Types
 * \details The following is the list of items types that can be programmed in the flash
 * @{
 */
#define WICED_BT_FACTORY_CONFIG_ITEM_FIRST              0x80
#define WICED_BT_FACTORY_CONFIG_ITEM_UUID               0xE0
#define WICED_BT_FACTORY_CONFIG_ITEM_OOB_STATIC_DATA    0xE1
#define WICED_BT_FACTORY_CONFIG_ITEM_HOMEKIT_TOKEN      0xE2
#define WICED_BT_FACTORY_CONFIG_ITEM_LAST               0xEF
/** @} WICED_BT_FACTORY_CONFIG_IDS */

/******************************************************************************
*
* Function Name: wiced_bt_factory_config_read
*
***************************************************************************//**
*
* \details The application calls this API to retrieve data programmed in the static
* section of flash. If entry with specified type is stored in the static section,
* the function copies data from the flash into the application buffer and returns
* number of bytes that has been copied. If entry with specified type does not
* exist in the static section of the flash, the function returns zero.
*
* \param    type : one of the @ref WICED_BT_FACTORY_CONFIG_IDS types
* \param    buffer : application buffer to return
* \param    buffer_size : size of the application buffer
* \return   number of bytes copied from the flash, or zero if item not found.
*
******************************************************************************/
uint16_t wiced_bt_factory_config_read(uint8_t item_type, uint8_t* buffer, uint16_t buffer_size);

#ifdef __cplusplus
}
#endif

/** @} wiced_bt_factory_config_api_functions */

#endif /* WICED_BT_FACTORY_APP_CONFIG__H */

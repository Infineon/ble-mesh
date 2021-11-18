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

void mesh_util_rand_gen_num_array(uint32_t* randNumberArrayPtr, uint32_t length);
uint8_t mesh_util_get_number_of_elements(uint16_t model_id);
uint32_t mesh_util_rand_gen_num(void);

#endif /* MESH_PLATFORM_UTILS_H_ */

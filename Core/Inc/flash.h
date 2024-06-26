/*
 * flash.h
 *
 *  Created on: Jun 20, 2024
 *      Author: Slawek
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_



/* Includes ------------------------------------------------------------------*/


/* Private includes ----------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/
int16_t Flash_Program_Data (uint32_t StartAddress, uint32_t *pData, uint32_t noOfData);
int16_t Flash_Read_Data(uint32_t StartAddress, uint32_t *pData, uint32_t noOfData);

/* Private defines -----------------------------------------------------------*/

#endif /* INC_FLASH_H_ */

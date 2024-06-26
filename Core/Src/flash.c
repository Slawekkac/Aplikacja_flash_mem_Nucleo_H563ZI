/*
 * flash.c
 *
 *  Created on: Jun 20, 2024
 *      Author: Slawek
 */


/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include <errno.h>
/* Private includes ----------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
#define NO_OF_WORDS_FLASH	4
#define FLASH_WORDS			4

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Global function prototypes ------------------------------------------------*/
int16_t Flash_Program_Data (uint32_t StartAddress, uint32_t *pData, uint32_t noOfData);
int16_t Flash_Read_Data(uint32_t StartAddress, uint32_t *pData, uint32_t noOfData);
/* Private function prototypes -----------------------------------------------*/


/* Private user code ---------------------------------------------------------*/
static uint32_t GetNumberSector(uint32_t Address)
{
	//uint32_t flashsize = FLASH_SIZE;
	//if ((Address < FLASH_BASE) || (Address > flashsize + FLASH_BASE))  return FLASH_SECTOR_NB;
	return (Address - FLASH_BASE) / FLASH_SECTOR_SIZE;
}


/* Global user code -----------------------------------------------------------*/
int16_t Flash_Program_Data (uint32_t StartAddress, uint32_t *pData, uint32_t noOfData)
{
	uint32_t StartSector = GetNumberSector(StartAddress);
	if (StartSector >= FLASH_SECTOR_NB) return -EINVAL;

	uint32_t EndSectorAddress = StartAddress + noOfData * NO_OF_WORDS_FLASH;
	uint32_t EndSector = GetNumberSector(EndSectorAddress);

	if (EndSector >= FLASH_SECTOR_NB) return -EINVAL;

	uint32_t NoOfSector = EndSector - StartSector + 1;



	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t SectorError;
	uint32_t acktNoOfData = 0;

	  /* Fill EraseInit structure*/
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.Banks = FLASH_BANK_1;
	EraseInitStruct.Sector = StartSector;
	EraseInitStruct.NbSectors = NoOfSector;

	taskENTER_CRITICAL();
	HAL_FLASH_Unlock();

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
	{
		HAL_FLASH_Lock();
		return -HAL_ERROR;
	}

	while (acktNoOfData < noOfData)
	{
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, StartAddress, (uint32_t ) &pData[acktNoOfData]) == HAL_OK)
		{
			StartAddress += NO_OF_WORDS_FLASH * FLASH_WORDS;
			acktNoOfData += FLASH_WORDS;
		}
		else
		{
			HAL_FLASH_Lock();
			return -HAL_ERROR;
		}
	}

	HAL_FLASH_Lock();
	taskEXIT_CRITICAL();

	return HAL_OK;
}

int16_t Flash_Read_Data(uint32_t StartAddress, uint32_t *pData, uint32_t noOfData)
{

	uint32_t acktNoOfData = 0;
	do
	{
		*pData = *(__IO uint32_t *)StartAddress;
		StartAddress += NO_OF_WORDS_FLASH;
		pData++;
		acktNoOfData++;

	}while (acktNoOfData <= noOfData);

	return HAL_OK;
}

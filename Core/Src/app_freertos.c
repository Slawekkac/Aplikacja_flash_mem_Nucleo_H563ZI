/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os2.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "flash.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADDRESFLASHMEM		(0x08080000)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for FlashMemTask */
osThreadId_t FlashMemTaskHandle;
const osThreadAttr_t FlashMemTask_attributes = {
  .name = "FlashMemTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 512 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartFlashMem(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of FlashMemTask */
  FlashMemTaskHandle = osThreadNew(StartFlashMem, NULL, &FlashMemTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}
/* USER CODE BEGIN Header_StartDefaultTask */
/**
* @brief Function implementing the defaultTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN defaultTask */
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_TogglePin(LED1_GREEN_GPIO_Port, LED1_GREEN_Pin);
	  osDelay(250);
  }
  /* USER CODE END defaultTask */
}

/* USER CODE BEGIN Header_StartFlashMem */
/**
* @brief Function implementing the FlashMemTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartFlashMem */
void StartFlashMem(void *argument)
{
  /* USER CODE BEGIN FlashMemTask */
	char *data = "Hello from NUCLEO-H563. This is test write and read from memory flash: ";
	uint32_t addressflash = ADDRESFLASHMEM;
	uint32_t noofdatatowrite = 0;
	char str2[20];
	char txdata[strlen(data) + strlen(str2)];
	char rxdata[strlen(data) + strlen(str2)];
	uint16_t noofwrite = 0;
	uint32_t noofdatatoread = 0;
  /* Infinite loop */
  for(;;)
  {
	  addressflash = ADDRESFLASHMEM;
	  memcpy(txdata, data, strlen(data));
	  sprintf(str2, "%d", noofwrite++);
	  strcpy(&txdata[strlen(data)], str2);
	  noofdatatowrite = (strlen(txdata) + strlen(str2))/4;
	  int16_t status = Flash_Program_Data(addressflash, (uint32_t*)txdata, noofdatatowrite);
	  if (status == 0) {
		  printf("Tx data: %s \n", txdata);
	  }else
	  {
		  printf("Error: %d", status);
	  }

	  addressflash = ADDRESFLASHMEM;
	  noofdatatoread = (strlen(txdata) + strlen(str2))/4;
	  Flash_Read_Data(addressflash, (uint32_t*)&rxdata[0], noofdatatoread);
	  printf("RX data: %s \n", rxdata);
	  HAL_GPIO_TogglePin(LED3_RED_GPIO_Port, LED3_RED_Pin);
	  osDelay(5000);
  }
  /* USER CODE END FlashMemTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */


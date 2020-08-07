/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "servo.h"
#include "prothawk.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
ServoTypeDef ServoAccel;
uint8_t RPM = 0x00;
uint8_t FuelCapacity = 0x00;

extern uint8_t SignalStatus;
extern TIM_HandleTypeDef htim3;
/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  volatile uint8_t buf[8];
  CAN_RxHeaderTypeDef RxHeader;

  if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, (uint8_t *)buf) != HAL_OK)
  {
    Error_Handler();
  }
  if (buf[0] == HeaderPropultionCommand)
  {
    ServoAccel.angle = buf[1];
    SignalStatus = 1;
    __HAL_TIM_SET_COUNTER(&htim3,0x00);
  }
  // Обработка принимаемого сообщения
}
/**
* @brief Function implementing the CAN_TX_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_CAN_TX_Task */
void Start_CAN_TX_Task(void const *argument)
{
  /* USER CODE BEGIN Start_CAN_TX_Task */
  /* Infinite loop */
  CAN_TxHeaderTypeDef TxHeader;
  extern CAN_HandleTypeDef hcan;
  uint8_t buf[4] = {0x00, 0x00, 0x00, 0x00};
  uint32_t TxMailBox; //= CAN_TX_MAILBOX0;

  TxHeader.DLC = 4;
  TxHeader.StdId = 0x0000;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.IDE = CAN_ID_STD;
  TxHeader.TransmitGlobalTime = DISABLE;

  for (;;)
  {
    buf[0] = HeaderPropultionCommand;
    buf[1] = RPM;
    buf[2] = FuelCapacity;
    buf[3] = ServoAccel.angle;

    taskENTER_CRITICAL();
    if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, buf, &TxMailBox) != HAL_OK)
    {
      Error_Handler();
    }
    taskEXIT_CRITICAL();
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    vTaskDelay(100);
  }
  /* USER CODE END Start_CAN_TX_Task */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const *argument)
{
  /* USER CODE BEGIN 5 */
  extern TIM_HandleTypeDef htim2;

  ServoAccel.Tim_PWM = &htim2;
  ServoAccel.TimChannel = TIM_CHANNEL_1;
  //ServoAccel.angle = 50;
  /* Infinite loop */
  for (;;)
  {
    if(SignalStatus)
    {
      ServoStart(&ServoAccel);
    }
    else
    {
      ServoStop(&ServoAccel);
    }
    ServoSetAngle(&ServoAccel);
    vTaskDelay(10);
  }
  /* USER CODE END 5 */
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

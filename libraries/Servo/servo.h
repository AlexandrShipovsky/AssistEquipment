/*
 * servo.h
 *
 *  Created on: 18.11.2019
 *      Author: shipovsky
 */
#ifndef SERVO_H_
#define SERVO_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f1xx_hal.h"

  /**
  * @brief  ��������� � ����������� ������������
  * @param TimChannel ����� �������
  * @param Tim_PWM ��������� �� ��������� �������
  * @param angle ����, �� ������� ���������� ���������
  * @retval None
  */
  typedef struct
  {
    uint32_t TimChannel;
    TIM_HandleTypeDef *Tim_PWM;
    int8_t CorrectionNeutral;
    uint8_t angle;
    uint8_t DirOfRot;
  } ServoTypeDef;

  typedef enum
  {
    DIRECT_ROTATION = 0,
    REVERSE_ROTATION = 1
  }ServoDir;

// ���������
static uint16_t ServoNeutral = 500;//usec
static uint16_t ServoMaxWidth = 2000;//2400; //usec
static uint16_t ServoMinWidth = 1100;//544;  //usec
#define ServoMaxAngle 100
#define ServoMinAngle 0

  HAL_StatusTypeDef ServoStop(ServoTypeDef *servo);
  HAL_StatusTypeDef ServoStart(ServoTypeDef *servo);
  HAL_StatusTypeDef ServoSetAngle(ServoTypeDef *servo);

#ifdef __cplusplus
}
#endif

#endif
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
  } ServoTypeDef;

// ���������
#define ServoNeutral (uint16_t)1520;  //usec
#define ServoMaxWidth (uint16_t)2400; //usec
#define ServoMinWidth (uint16_t)544;  //usec
#define ServoMaxAngle 100
#define ServoMinAngle 0

  HAL_StatusTypeDef ServoStop(ServoTypeDef *servo);
  HAL_StatusTypeDef ServoStart(ServoTypeDef *servo);
  HAL_StatusTypeDef ServoSetAngle(ServoTypeDef *servo);

#ifdef __cplusplus
}
#endif

#endif
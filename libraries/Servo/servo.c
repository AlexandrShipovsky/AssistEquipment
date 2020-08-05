/*
 * servo.c
 *
 *  Created on: 18.11.2019
 *      Author: shipovsky
 */
#include "servo.h"

/**
  * @brief  Функция отключает удерживание сервопривода;
  * @retval None
  */
HAL_StatusTypeDef ServoStop(ServoTypeDef *servo)
{
  HAL_StatusTypeDef status;
  status = HAL_TIM_PWM_Stop(servo->Tim_PWM, servo->TimChannel); // Стоп ШИМ
  return status;
}

/**
  * @brief  Функция включает сервопивод;
  * @retval None
  */
HAL_StatusTypeDef ServoStart(ServoTypeDef *servo)
{
  HAL_StatusTypeDef status;
  status = HAL_TIM_PWM_Start(servo->Tim_PWM, servo->TimChannel); // Старт ШИМ
  return status;
}

/**
  * @brief  Функция устанавливает угол сервопривода с заданной скоростью
  * @param angle угол 0 - минимальный, 100 - максимальный, 50 - среднее положение;
  * @retval None
  */
HAL_StatusTypeDef ServoSetAngle(ServoTypeDef *servo)
{
  uint16_t pulse;

  if ((servo->angle > 100))
  {
    return HAL_ERROR;
  }
  else if (servo->angle > ServoMaxAngle)
  {
    servo->angle = ServoMaxAngle;
  }
  else if (servo->angle < ServoMinAngle)
  {
    servo->angle = ServoMinAngle;
  }

  //HAL_TIM_PWM_Stop(servo->Tim_PWM, servo->TimChannel); // Стоп ШИМ
  pulse = 544 + (servo->angle * (1856)) / 100;
  __HAL_TIM_SET_COMPARE(servo->Tim_PWM, servo->TimChannel, (uint16_t)pulse - 1); // Ширина импульса в мкс

  //HAL_TIM_PWM_Start(servo->Tim_PWM, servo->TimChannel); // Старт ШИМ
  return HAL_OK;
}
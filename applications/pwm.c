/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-28     Thinkpad_X1       the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <pwm.h>


#define PWM4_DEV_NAME        "pwm4"  /* PWM设备名称 */
#define PWM9_DEV_NAME        "pwm9"  /* PWM设备名称 */
#define PWM1_DEV_NAME        "pwm1"  /* PWM设备名称 */
#if 0
#define PWM4_DEV_CHANNEL3     3       /* PWM通道 */
#define PWM4_DEV_CHANNEL4     4       /* PWM通道 */
#define PWM9_DEV_CHANNEL1     1       /* PWM通道 */
#define PWM9_DEV_CHANNEL2     2       /* PWM通道 */
#define PWM1_DEV_CHANNEL4     4       /* PWM通道 */
#endif
int PWM4_DEV_CHANNEL3 = 3;       /* PWM通道 */
int PWM4_DEV_CHANNEL4 = 4;       /* PWM通道 */
int PWM9_DEV_CHANNEL1 = 1;       /* PWM通道 */
int PWM9_DEV_CHANNEL2 = 2;       /* PWM通道 */
int PWM1_DEV_CHANNEL4 = 4;       /* PWM通道 */
struct rt_device_pwm *pwm4_dev;      /* PWM设备句柄 */
struct rt_device_pwm *pwm9_dev;      /* PWM设备句柄 */
struct rt_device_pwm *pwm1_dev;      /* PWM设备句柄 */
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim9;

void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}

/**
  * @brief TIM9 Initialization Function
  * @param None
  * @retval None
  */
void MX_TIM9_Init(void)
{

  /* USER CODE BEGIN TIM9_Init 0 */

  /* USER CODE END TIM9_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM9_Init 1 */

  /* USER CODE END TIM9_Init 1 */
  htim9.Instance = TIM9;
  htim9.Init.Prescaler = 0;
  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim9.Init.Period = 65535;
  htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim9) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim9, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim9, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM9_Init 2 */

  /* USER CODE END TIM9_Init 2 */
  HAL_TIM_MspPostInit(&htim9);

}

int pwm4_init(void){
    period = 2500000;    /* 周期为2.5ms，单位为纳秒ns */
    //period = 20000000;    /* 周期为20ms，单位为纳秒ns */
    pulse = 0;          /* PWM脉冲宽度值，单位为纳秒ns */
    pwm4_dev = (struct rt_device_pwm *)rt_device_find(PWM4_DEV_NAME);
    if (pwm4_dev == RT_NULL)
        {
            rt_kprintf("pwm4 initialize failed! can't find %s device!\n", PWM4_DEV_NAME);
            return RT_ERROR;
        }
    /* 设置PWM周期和脉冲宽度默认值 */
    //rt_pwm_set(pwm4_dev, PWM4_DEV_CHANNEL3, period, pulse);
    rt_pwm_set(pwm4_dev, PWM4_DEV_CHANNEL4, period, pulse);
    /* 使能设备 */
    //rt_pwm_enable(pwm4_dev, PWM4_DEV_CHANNEL3);
    rt_pwm_enable(pwm4_dev, PWM4_DEV_CHANNEL4);
    return RT_EOK;
}


int pwm9_init(void){
    period = 100000;    /* 周期为0.1ms，单位为纳秒ns */
    //period = 20000000;    /* 周期为20ms，单位为纳秒ns */
    pulse = 99990;          /* PWM脉冲宽度值，单位为纳秒ns */
    pwm9_dev = (struct rt_device_pwm *)rt_device_find(PWM9_DEV_NAME);
    if (pwm9_dev == RT_NULL)
        {
            rt_kprintf("pwm9 initialize failed! can't find %s device!\n", PWM9_DEV_NAME);
            return RT_ERROR;
        }
    /* 设置PWM周期和脉冲宽度默认值 */
    rt_pwm_set(pwm9_dev, PWM9_DEV_CHANNEL1, period, pulse);
    //rt_pwm_set(pwm9_dev, PWM9_DEV_CHANNEL2, period, pulse);
    /* 使能设备 */
    rt_pwm_enable(pwm9_dev, PWM9_DEV_CHANNEL1);
    //rt_pwm_enable(pwm9_dev, PWM9_DEV_CHANNEL2);
    return RT_EOK;
}

int pwm1_init(void){
    period = 2500000;    /* 周期为2.5ms，单位为纳秒ns */
    //period = 20000000;    /* 周期为20ms，单位为纳秒ns */
    pulse = 0;          /* PWM脉冲宽度值，单位为纳秒ns */
    pwm1_dev = (struct rt_device_pwm *)rt_device_find(PWM1_DEV_NAME);
    if (pwm1_dev == RT_NULL)
        {
            rt_kprintf("pwm1 initialize failed! can't find %s device!\n", PWM1_DEV_NAME);
            return RT_ERROR;
        }
    /* 设置PWM周期和脉冲宽度默认值 */
    rt_pwm_set(pwm1_dev, PWM1_DEV_CHANNEL4, period, pulse);
    /* 使能设备 */
    rt_pwm_enable(pwm1_dev, PWM1_DEV_CHANNEL4);
    return RT_EOK;
}


void pwm_init()
{
    MX_TIM4_Init();
    MX_TIM9_Init();
    MX_TIM1_Init();
    pwm4_init();
    pwm9_init();
    pwm1_init();
}







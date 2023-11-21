/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cJSON.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
typedef enum MSG_TYPE_t MSG_TYPE;
enum MSG_TYPE_t{
	MSG_LED_1 = 1,
	MSG_LED_2,
	MSG_LED_3,
	MSG_LED_4,
	MSG_LED_5,
	MSG_LED_6,
	MSG_MOTOR_1,
	MSG_MOTOR_2,
	MSG_MOTOR_3,
	MSG_MOTOR_4,
	MSG_MOTOR_5
};

typedef struct SENSOR_t SENSOR;
struct SENSOR_t{
	uint8_t state;
};
typedef struct SMH_APP_t SMH_APP;
struct SMH_APP_t{
	SENSOR gas_sensor;
	SENSOR rain_sensor;
	uint8_t led1_state;
	uint8_t led2_state;
	uint8_t led3_state;
	uint8_t led4_state;
	uint8_t led5_state;
	uint8_t led6_state;
	uint8_t motor1_state;
	uint8_t motor2_state;
	uint8_t motor3_state;
	uint8_t motor4_state;
	uint8_t motor5_state;
	uint8_t horn_state;
	uint8_t ledred_state;
};
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
SMH_APP main_app;
uint8_t uart_rx_buff[30];
uint8_t _rxIndex = 0;
uint8_t rx_data;
uint8_t tx_complete_flag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */
void process_rx_data();
void process_led_control();
void process_motor_control();
void process_measure();
void process_control();
void set_motor_radian(uint8_t motor_number, uint8_t radian);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  if(huart->Instance == USART1){
	  uart_rx_buff[_rxIndex++] = rx_data;
	  if(rx_data == '}'){
		  _rxIndex = 0;
		  tx_complete_flag = 1;
	  }
	  HAL_UART_Receive_IT(&huart1, &rx_data, 1);
  }
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_RxCpltCallback could be implemented in the user file
   */
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  main_app.horn_state = 0;
  main_app.ledred_state = 0;
  HAL_UART_Receive_IT(&huart1, &rx_data, 1);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
	  if(tx_complete_flag == 1){
		  process_rx_data();
		  tx_complete_flag = 0;
	  }
	  process_measure();
	  process_control();
	  HAL_Delay(100);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 999;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 159;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
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
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
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
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED1_Pin|LED2_Pin|LED3_Pin|LED4_Pin
                          |LED5_Pin|LED6_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, HORN_Pin|LEDRED_Pin|FAN_CTRL_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : LED_BLUE_Pin */
  GPIO_InitStruct.Pin = LED_BLUE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_BLUE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED1_Pin LED2_Pin LED3_Pin LED4_Pin
                           LED5_Pin LED6_Pin */
  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin|LED3_Pin|LED4_Pin
                          |LED5_Pin|LED6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : GAS_DI_Pin RAIN_DI_Pin PIR_DI_Pin */
  GPIO_InitStruct.Pin = GAS_DI_Pin|RAIN_DI_Pin|PIR_DI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : HORN_Pin LEDRED_Pin FAN_CTRL_Pin */
  GPIO_InitStruct.Pin = HORN_Pin|LEDRED_Pin|FAN_CTRL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void process_rx_data(){
	MSG_TYPE msg_type;
	cJSON* root = cJSON_Parse((char*)uart_rx_buff);
	cJSON* msg_id = cJSON_GetObjectItem(root, "msg_id");
	cJSON* state;
	msg_type = (MSG_TYPE)cJSON_GetNumberValue(msg_id);
	switch(msg_type){
		case MSG_LED_1:
			state = cJSON_GetObjectItem(root, "LED1");
			main_app.led1_state = cJSON_GetNumberValue(state);
			break;
		case MSG_LED_2:
			state = cJSON_GetObjectItem(root, "LED2");
			main_app.led2_state = cJSON_GetNumberValue(state);
			break;
		case MSG_LED_3:
			state = cJSON_GetObjectItem(root, "LED3");
			main_app.led3_state = cJSON_GetNumberValue(state);
			break;
		case MSG_LED_4:
			state = cJSON_GetObjectItem(root, "LED4");
			main_app.led4_state = cJSON_GetNumberValue(state);
			break;
		case MSG_LED_5:
			state = cJSON_GetObjectItem(root, "LED5");
			main_app.led5_state = cJSON_GetNumberValue(state);
			break;
		case MSG_LED_6:
			state = cJSON_GetObjectItem(root, "LED6");
			main_app.led6_state = cJSON_GetNumberValue(state);
			break;
		case MSG_MOTOR_1:
			state = cJSON_GetObjectItem(root, "MOTOR1");
			main_app.motor1_state = cJSON_GetNumberValue(state);
			break;
		case MSG_MOTOR_2:
			state = cJSON_GetObjectItem(root, "MOTOR2");
			main_app.motor2_state = cJSON_GetNumberValue(state);
			break;
		case MSG_MOTOR_3:
			state = cJSON_GetObjectItem(root, "MOTOR3");
			main_app.motor3_state = cJSON_GetNumberValue(state);
			break;
		case MSG_MOTOR_4:
			state = cJSON_GetObjectItem(root, "MOTOR4");
			main_app.motor4_state = cJSON_GetNumberValue(state);
			break;
		case MSG_MOTOR_5:
			state = cJSON_GetObjectItem(root, "MOTOR5");
			main_app.motor5_state = cJSON_GetNumberValue(state);
			break;
	}
	cJSON_Delete(root);

}


void process_control(){

	HAL_GPIO_WritePin(FAN_CTRL_GPIO_Port, FAN_CTRL_Pin, main_app.motor5_state);
	if(main_app.gas_sensor.state == 1){
		HAL_GPIO_WritePin(LEDRED_GPIO_Port, LEDRED_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(HORN_GPIO_Port, HORN_Pin, GPIO_PIN_RESET);
		main_app.ledred_state = 1;
		main_app.horn_state = 1;
	}
	else{
		HAL_GPIO_WritePin(LEDRED_GPIO_Port, LEDRED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(HORN_GPIO_Port, HORN_Pin, GPIO_PIN_SET);
		main_app.ledred_state = 0;
		main_app.horn_state = 0;
	}
	process_led_control();
	process_motor_control();
}

void process_led_control(){
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, main_app.led1_state);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, main_app.led2_state);
	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, main_app.led3_state);
	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, main_app.led4_state);
	HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, main_app.led5_state);
	HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin, main_app.led6_state);
	HAL_GPIO_WritePin(LEDRED_GPIO_Port, LEDRED_Pin, main_app.ledred_state);
}

void process_motor_control(){
	if(main_app.motor1_state == 1){
		set_motor_radian(1, 180);
	}
	else{
		set_motor_radian(1, 0);
	}

	if(main_app.motor2_state == 1){
		set_motor_radian(2, 180);
	}
	else{
		set_motor_radian(2, 0);
	}

	if(main_app.motor4_state == 1){
		set_motor_radian(4, 180);
	}
	else{
		set_motor_radian(4, 0);
	}

	if(main_app.rain_sensor.state == 1){
		set_motor_radian(3, 180);
	}
	else{
		set_motor_radian(3, 0);
	}

}
void process_measure(){
	main_app.gas_sensor.state = !(uint8_t)HAL_GPIO_ReadPin(GAS_DI_GPIO_Port, GAS_DI_Pin);
	main_app.rain_sensor.state = !(uint8_t)HAL_GPIO_ReadPin(RAIN_DI_GPIO_Port, RAIN_DI_Pin);
}

void set_motor_radian(uint8_t motor_number, uint8_t radian){
	switch(motor_number){
		case 1:
			if(radian == 0){
				__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 5);
			}
			else if(radian == 180){
				__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 20);
			}
			break;
		case 2:
			if(radian == 0){
				__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 5);
			}
			else if(radian == 180){
				__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 20);
			}
			break;
		case 3:
			if(radian == 0){
				__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 5);
			}
			else if(radian == 180){
				__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 20);
			}
			break;
		case 4:
			if(radian == 0){
				__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 5);
			}
			else if(radian == 180){
				__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 20);
			}
			break;
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

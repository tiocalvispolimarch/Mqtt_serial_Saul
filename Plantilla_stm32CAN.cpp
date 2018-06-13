#include "main.h"
#include "stm32f3xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */
/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan;

UART_HandleTypeDef huart2;
uint8_t can_buffer[8];
uint8_t can_aviso = 0;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_CAN_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
//funcion para que vuelve a configurar una nueva interupcion: 
void RxIntEnable(CAN_HandleTypeDef *CanHandle) {
	if(CanHandle->State == HAL_CAN_STATE_BUSY_TX)//cuando se transmiten mensajes
	  CanHandle->State = HAL_CAN_STATE_BUSY_TX_RX0;
	else {
	  CanHandle->State = HAL_CAN_STATE_BUSY_RX0;

		/* Set CAN error code to none */
		CanHandle->ErrorCode = HAL_CAN_ERROR_NONE;

		/* Enable Error warning Interrupt */
		__HAL_CAN_ENABLE_IT(CanHandle, CAN_IT_EWG);

		/* Enable Error passive Interrupt */
		__HAL_CAN_ENABLE_IT(CanHandle, CAN_IT_EPV);

		/* Enable Bus-off Interrupt */
		__HAL_CAN_ENABLE_IT(CanHandle, CAN_IT_BOF);

		/* Enable Last error code Interrupt */
		__HAL_CAN_ENABLE_IT(CanHandle, CAN_IT_LEC);

		/* Enable Error Interrupt */
		__HAL_CAN_ENABLE_IT(CanHandle, CAN_IT_ERR);
	  }

	  // Enable FIFO 0 message pending Interrupt
	  __HAL_CAN_ENABLE_IT(CanHandle, CAN_IT_FMP0);
} 

//funciones de interrupcion cuando llega un mensaje: 
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef *CanHandle) {
	//if(CanHandle->pRxMsg->StdId == 0x7E8 && CanHandle->pRxMsg->IDE == CAN_ID_STD && CanHandle->pRxMsg->DLC == 8) {
		if(CanHandle->pRxMsg->IDE == CAN_ID_STD && CanHandle->pRxMsg->DLC == 8) {
		for (char i = 0; i < 8; i++)
		can_buffer[i] = CanHandle->pRxMsg->Data[i];
		can_aviso = 1;
	}

	RxIntEnable(CanHandle);
}

/* USER CODE END 0 */
 uint8_t frase[30] = "Comunicacion por CAN\n\r";
 uint8_t frase2[30] = "Se recibio el numero: "; 
 uint8_t frase3[30] = "Ingrese un numero:\n\r "; 
 uint8_t mensaje[30] = "Ya esta todo shido!!!\n\r";
 uint8_t salto[3] = "\n\r"; 
 uint8_t buffer = 0; 
/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_USART2_UART_Init();
  MX_CAN_Init();
  /* USER CODE BEGIN 2 */
if(HAL_CAN_Receive_IT(&hcan, CAN_FIFO0) == HAL_OK) {
	  HAL_UART_Transmit(&huart2,mensaje,sizeof(mensaje),100);
  }
  /* USER CODE END 2 */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	hcan.pTxMsg->StdId = 0x7E8;
	hcan.pTxMsg->IDE = CAN_ID_STD;
	hcan.pTxMsg->RTR = CAN_RTR_DATA;
	hcan.pTxMsg->DLC = 8;
	hcan.pTxMsg->Data[0] = 1;
	for (char i = 1; i<8; i++)
	  hcan.pTxMsg->Data[i] = 0;
  while (1)
  {
		//HAL_UART_Transmit(&huart2, frase ,sizeof(frase), 100);  
		HAL_Delay(200); 
		if(can_aviso) {
	  		 can_aviso = 0;
		
			HAL_UART_Transmit(&huart2, frase2 ,sizeof(frase2), 100);
			can_buffer[0] = can_buffer[0] + 0x30; 
			HAL_UART_Transmit(&huart2, can_buffer ,1, 100);
			HAL_UART_Transmit(&huart2, salto ,sizeof(salto), 100);
			HAL_CAN_Transmit(&hcan,100); 
			
		}

  /* USER CODE END WHILE 
		HAL_UART_Transmit(&huart2,frase3,sizeof(frase3),100);
		HAL_UART_Receive(&huart2,&buffer,1,10000);
		buffer -= 0x30; 
		hcan.pTxMsg->Data[0] = buffer;
		HAL_CAN_Transmit(&hcan,100);
		HAL_Delay(200); 
   USER CODE BEGIN 3 */
		

  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* CAN init function */
static void MX_CAN_Init(void)
{
	static CanRxMsgTypeDef CanRX;
  static CanTxMsgTypeDef CanTX;
  CAN_FilterConfTypeDef sFilterConfig;

  hcan.Instance = CAN;
  hcan.pRxMsg = &CanRX;
  hcan.pTxMsg = &CanTX;
	
	

  hcan.Instance = CAN;
  hcan.Init.Prescaler = 32;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SJW = CAN_SJW_3TQ;
  hcan.Init.BS1 = CAN_BS1_6TQ;
  hcan.Init.BS2 = CAN_BS2_3TQ;
  hcan.Init.TTCM = DISABLE;
  hcan.Init.ABOM = DISABLE;
  hcan.Init.AWUM = DISABLE;
  hcan.Init.NART = DISABLE;
  hcan.Init.RFLM = DISABLE;
  hcan.Init.TXFP = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
/*##-2- Configure the CAN Filter ###########################################*/
	sFilterConfig.FilterNumber = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = 0xFD00; //SI QUE QUIERE FILTRAR SE PONE 0xFD00
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0xFFFF;//Si es uno compara con el filtro, si es 0 don't care.
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.BankNumber = 14;

	if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
	{
	/* Filter configuration Error */
	Error_Handler();
	}
}
/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
	 huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


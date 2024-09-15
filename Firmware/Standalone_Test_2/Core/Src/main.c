/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
CAN_HandleTypeDef hcan1;

DAC_HandleTypeDef hdac1;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for DeferredTask1 */
osThreadId_t DeferredTask1Handle;
const osThreadAttr_t DeferredTask1_attributes = {
  .name = "DeferredTask1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for DeferredTask2 */
osThreadId_t DeferredTask2Handle;
const osThreadAttr_t DeferredTask2_attributes = {
  .name = "DeferredTask2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal3,
};
/* Definitions for DeferredTask3 */
osThreadId_t DeferredTask3Handle;
const osThreadAttr_t DeferredTask3_attributes = {
  .name = "DeferredTask3",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for DeferredTask4 */
osThreadId_t DeferredTask4Handle;
const osThreadAttr_t DeferredTask4_attributes = {
  .name = "DeferredTask4",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal3,
};
/* Definitions for myBinarySem01 */
osSemaphoreId_t myBinarySem01Handle;
const osSemaphoreAttr_t myBinarySem01_attributes = {
  .name = "myBinarySem01"
};
/* Definitions for myBinarySem02 */
osSemaphoreId_t myBinarySem02Handle;
const osSemaphoreAttr_t myBinarySem02_attributes = {
  .name = "myBinarySem02"
};
/* Definitions for myBinarySem03 */
osSemaphoreId_t myBinarySem03Handle;
const osSemaphoreAttr_t myBinarySem03_attributes = {
  .name = "myBinarySem03"
};
/* Definitions for myBinarySem04 */
osSemaphoreId_t myBinarySem04Handle;
const osSemaphoreAttr_t myBinarySem04_attributes = {
  .name = "myBinarySem04"
};
/* USER CODE BEGIN PV */
CAN_FilterTypeDef Rx_Filter_0, Rx_Filter_1;
CAN_RxHeaderTypeDef CAN_Rx_Header_Id1, CAN_Rx_Header_Id2;
uint8_t CAN_Rx_Data_Id1[8] = {'\0','\0','\0','\0','\0','\0','\0','\0'};
uint8_t CAN_Rx_Data_Id2[8] = {'\0','\0','\0','\0','\0','\0','\0','\0'};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_CAN1_Init(void);
static void MX_DAC1_Init(void);
void StartDefaultTask(void *argument);
void Interpret_CAN_Msg_Id1(void *argument);
void Update_Stepper_Motor_Task(void *argument);
void Interpret_CAN_Msg_Id2(void *argument);
void Update_Standalone_Op_Task(void *argument);

/* USER CODE BEGIN PFP */
void Read_CAN_Frame_From_FIFO0(CAN_HandleTypeDef *hcan);
void Read_CAN_Frame_From_FIFO1(CAN_HandleTypeDef *hcan);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_CAN1_Init();
  MX_DAC1_Init();
  /* USER CODE BEGIN 2 */
  HAL_CAN_Start(&hcan1);
  HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
  Init_Stepper_Motor();
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of myBinarySem01 */
  myBinarySem01Handle = osSemaphoreNew(1, 0, &myBinarySem01_attributes);

  /* creation of myBinarySem02 */
  myBinarySem02Handle = osSemaphoreNew(1, 0, &myBinarySem02_attributes);

  /* creation of myBinarySem03 */
  myBinarySem03Handle = osSemaphoreNew(1, 0, &myBinarySem03_attributes);

  /* creation of myBinarySem04 */
  myBinarySem04Handle = osSemaphoreNew(1, 0, &myBinarySem04_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of DeferredTask1 */
  DeferredTask1Handle = osThreadNew(Interpret_CAN_Msg_Id1, NULL, &DeferredTask1_attributes);

  /* creation of DeferredTask2 */
  DeferredTask2Handle = osThreadNew(Update_Stepper_Motor_Task, NULL, &DeferredTask2_attributes);

  /* creation of DeferredTask3 */
  DeferredTask3Handle = osThreadNew(Interpret_CAN_Msg_Id2, NULL, &DeferredTask3_attributes);

  /* creation of DeferredTask4 */
  DeferredTask4Handle = osThreadNew(Update_Standalone_Op_Task, NULL, &DeferredTask4_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 40;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_8TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_7TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */
  // psc x (tBS1 + tBS2 + 1) = fpclk1 / baud
  // psc x (tBS1 + tBS2 + 1) = 80MHz / 125kHz = 640
  // psc x (8 + 7 + 1) = 640
  // psc = 640 / 16 = 40

  CAN_Rx_Header_Id1.StdId = 0;
  CAN_Rx_Header_Id1.IDE = CAN_ID_STD;
  CAN_Rx_Header_Id1.RTR = CAN_RTR_DATA;
  CAN_Rx_Header_Id1.DLC = 0;
  
  CAN_Rx_Header_Id2.StdId = 0;
  CAN_Rx_Header_Id2.IDE = CAN_ID_STD;
  CAN_Rx_Header_Id2.RTR = CAN_RTR_DATA;
  CAN_Rx_Header_Id2.DLC = 0;

  Rx_Filter_0.FilterIdHigh = ((uint32_t)0x1C0) << 5;
  Rx_Filter_0.FilterIdLow = 0x0000;
  Rx_Filter_0.FilterMaskIdHigh = ((uint32_t)0x7FF) << 5;
  Rx_Filter_0.FilterMaskIdLow = 0x0000;
  Rx_Filter_0.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  Rx_Filter_0.FilterBank = 0;	// Bank 0
  Rx_Filter_0.FilterMode = CAN_FILTERMODE_IDMASK;
  Rx_Filter_0.FilterScale = CAN_FILTERSCALE_32BIT;
  Rx_Filter_0.FilterActivation = CAN_FILTER_ENABLE;
  HAL_CAN_ConfigFilter(&hcan1, &Rx_Filter_0);
  
  Rx_Filter_1.FilterIdHigh = ((uint32_t)0x1C1) << 5;
  Rx_Filter_1.FilterIdLow = 0x0000;
  Rx_Filter_1.FilterMaskIdHigh = ((uint32_t)0x7FF) << 5;
  Rx_Filter_1.FilterMaskIdLow = 0x0000;
  Rx_Filter_1.FilterFIFOAssignment = CAN_FILTER_FIFO1;
  Rx_Filter_1.FilterBank = 1;	// Bank 1
  Rx_Filter_1.FilterMode = CAN_FILTERMODE_IDMASK;
  Rx_Filter_1.FilterScale = CAN_FILTERSCALE_32BIT;
  Rx_Filter_1.FilterActivation = CAN_FILTER_ENABLE;
  HAL_CAN_ConfigFilter(&hcan1, &Rx_Filter_1);
  
  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
  HAL_CAN_RegisterCallback(&hcan1, HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID, Read_CAN_Frame_From_FIFO0);
  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO1_MSG_PENDING);
  HAL_CAN_RegisterCallback(&hcan1, HAL_CAN_RX_FIFO1_MSG_PENDING_CB_ID, Read_CAN_Frame_From_FIFO1);
  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief DAC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC1_Init(void)
{

  /* USER CODE BEGIN DAC1_Init 0 */

  /* USER CODE END DAC1_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC1_Init 1 */

  /* USER CODE END DAC1_Init 1 */

  /** DAC Initialization
  */
  hdac1.Instance = DAC1;
  if (HAL_DAC_Init(&hdac1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC1_Init 2 */

  /* USER CODE END DAC1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 2999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 1499;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
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
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOH);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_SetOutputPin(CFG5_GPIO_Port, CFG5_Pin);

  /**/
  LL_GPIO_SetOutputPin(GPIOB, CFG3_Pin|CFG2_Pin|CFG1_Pin);

  /**/
  LL_GPIO_SetOutputPin(CFG6_EN_GPIO_Port, CFG6_EN_Pin);

  /**/
  LL_GPIO_ResetOutputPin(GPIOA, LD2_Pin|DIR_Pin);

  /**/
  LL_GPIO_ResetOutputPin(GPIOB, CFG4_Pin|CFG0_Pin);

  /**/
  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE13);

  /**/
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_13;
  EXTI_InitStruct.Line_32_63 = LL_EXTI_LINE_NONE;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /**/
  LL_GPIO_SetPinPull(B1_GPIO_Port, B1_Pin, LL_GPIO_PULL_NO);

  /**/
  LL_GPIO_SetPinMode(B1_GPIO_Port, B1_Pin, LL_GPIO_MODE_INPUT);

  /**/
  GPIO_InitStruct.Pin = CFG5_Pin|DIR_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = CFG4_Pin|CFG2_Pin|CFG0_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = CFG3_Pin|CFG1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = CFG6_EN_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(CFG6_EN_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
LL_GPIO_SetPinMode(STEP_GPIO_Port, STEP_Pin, LL_GPIO_MODE_OUTPUT);// if I don't set STEP pin to output a low(0), motor will start moving initially
LL_GPIO_SetPinOutputType(STEP_GPIO_Port, STEP_Pin, LL_GPIO_OUTPUT_PUSHPULL);
LL_GPIO_ResetOutputPin(STEP_GPIO_Port, STEP_Pin);
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void Read_CAN_Frame_From_FIFO0(CAN_HandleTypeDef *hcan)
{
	HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &CAN_Rx_Header_Id1, CAN_Rx_Data_Id1);
	if(CAN_Rx_Header_Id1.DLC >= 4){
		osSemaphoreRelease(myBinarySem01Handle);
	}// else something is wrong, don't proceed with interpreting message
}

void Read_CAN_Frame_From_FIFO1(CAN_HandleTypeDef *hcan)
{
	HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO1, &CAN_Rx_Header_Id2, CAN_Rx_Data_Id2);
	if(CAN_Rx_Header_Id2.DLC >= 6){
		osSemaphoreRelease(myBinarySem03Handle);
	}// else something is wrong, don't proceed with interpreting message
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(osWaitForever);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_Interpret_CAN_Msg_Id1 */
/**
* @brief Function implementing the DeferredTask1 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Interpret_CAN_Msg_Id1 */
void Interpret_CAN_Msg_Id1(void *argument)
{
  /* USER CODE BEGIN Interpret_CAN_Msg_Id1 */
  uint8_t msg_correct_cnt;	// if more message need to be processed I think better use bitwise masking than count
  /* Infinite loop */
  for(;;)
  {
	  osSemaphoreAcquire(myBinarySem01Handle, osWaitForever);
	  // interpret here and fill up hstepmtr
	  msg_correct_cnt = 0;
	  if((CAN_Rx_Data_Id1[0] == STEPPER_MOTOR_STOP) || (CAN_Rx_Data_Id1[0] == STEPPER_MOTOR_START)){
		  msg_correct_cnt++;
		  hstepmtr.run_state = CAN_Rx_Data_Id1[0];
	  }
	  if((CAN_Rx_Data_Id1[1] == MOTOR_DIRECTION_CCW) || (CAN_Rx_Data_Id1[1] == MOTOR_DIRECTION_CW)){
		  msg_correct_cnt++;
		  hstepmtr.direction = CAN_Rx_Data_Id1[1];
	  }
	  if((CAN_Rx_Data_Id1[2] >= MIN_CURRENT_SCALE) && (CAN_Rx_Data_Id1[2] <= MAX_CURRENT_SCALE)){
		  msg_correct_cnt++;
		  hstepmtr.current_scale = CAN_Rx_Data_Id1[2];
	  }
	  if((CAN_Rx_Data_Id1[3] >= MIN_MOTOR_SPEED_RPM) && (CAN_Rx_Data_Id1[3] <= MAX_MOTOR_SPEED_RPM)){
		  msg_correct_cnt++;
		  hstepmtr.speed = CAN_Rx_Data_Id1[3];
	  }
	  if(msg_correct_cnt > 0){
		  osSemaphoreRelease(myBinarySem02Handle);	// pass to the next task
	  }// if 0 (none) of the messages are correct, don't proceed
  }
  /* USER CODE END Interpret_CAN_Msg_Id1 */
}

/* USER CODE BEGIN Header_Update_Stepper_Motor_Task */
/**
* @brief Function implementing the DeferredTask2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Update_Stepper_Motor_Task */
void Update_Stepper_Motor_Task(void *argument)
{
  /* USER CODE BEGIN Update_Stepper_Motor_Task */
  /* Infinite loop */
  for(;;)
  {
	  osSemaphoreAcquire(myBinarySem02Handle, osWaitForever);
	  Update_Stepper_Motor_Operation();
  }
  /* USER CODE END Update_Stepper_Motor_Task */
}

/* USER CODE BEGIN Header_Interpret_CAN_Msg_Id2 */
/**
* @brief Function implementing the DeferredTask3 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Interpret_CAN_Msg_Id2 */
void Interpret_CAN_Msg_Id2(void *argument)
{
  /* USER CODE BEGIN Interpret_CAN_Msg_Id2 */
  uint8_t msg_correct_cnt;
  /* Infinite loop */
  for(;;)
  {
	  osSemaphoreAcquire(myBinarySem03Handle, osWaitForever);
	  // interpret here and fill up hstepmtr
	  msg_correct_cnt = 0;
	  
	  if(CAN_Rx_Data_Id2[0] <= TOFF_332TCLK)
	  {		msg_correct_cnt++; hstepmtr.standalone_cfg.chopper_off_time = CAN_Rx_Data_Id2[0];	}
	  
	  hstepmtr.standalone_cfg.chopper_mode = CAN_Rx_Data_Id2[1] & MODE_STEALTHCHOP;
	  hstepmtr.standalone_cfg.interpolation = CAN_Rx_Data_Id2[1] & USE_INTERPOLATION;
	  CAN_Rx_Data_Id2[1] &= (MICROSTEP_16 | QUARTERSTEP_4 | HALFSTEP_2 | FULLSTEP_1);
	  if((CAN_Rx_Data_Id2[1] == MICROSTEP_16) || (CAN_Rx_Data_Id2[1] == QUARTERSTEP_4) || 
		 (CAN_Rx_Data_Id2[1] == HALFSTEP_2) || (CAN_Rx_Data_Id2[1] == FULLSTEP_1))
	  {		msg_correct_cnt++; hstepmtr.standalone_cfg.microstep_res = CAN_Rx_Data_Id2[1];	}
	  
	  if(CAN_Rx_Data_Id2[2] <= EXT_SENSE_RES_SCALED_AIN)
	  {		msg_correct_cnt++; hstepmtr.standalone_cfg.current_setting = CAN_Rx_Data_Id2[2];	}
	  
	  if(CAN_Rx_Data_Id2[3] <= HYSTERESIS_END_13)
	  {		msg_correct_cnt++; hstepmtr.standalone_cfg.chopper_hysteresis = CAN_Rx_Data_Id2[3];	}
	  
	  if(CAN_Rx_Data_Id2[4] <= BLANK_TIME_36)
	  {		msg_correct_cnt++; hstepmtr.standalone_cfg.chopper_blank_time = CAN_Rx_Data_Id2[4];	}
	  
	  hstepmtr.standalone_cfg.driver_enable = CAN_Rx_Data_Id2[5] & DRIVER_DISABLE;
	  hstepmtr.standalone_cfg.standstill_pwr_down = CAN_Rx_Data_Id2[5] & STANDSTILL_ON;
	  
	  if(msg_correct_cnt > 0){
		  osSemaphoreRelease(myBinarySem04Handle);	// pass to the next task
	  }// if 0 (none) of the messages are correct, don't proceed
  }
  /* USER CODE END Interpret_CAN_Msg_Id2 */
}

/* USER CODE BEGIN Header_Update_Standalone_Op_Task */
/**
* @brief Function implementing the DeferredTask4 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Update_Standalone_Op_Task */
void Update_Standalone_Op_Task(void *argument)
{
  /* USER CODE BEGIN Update_Standalone_Op_Task */
  /* Infinite loop */
  for(;;)
  {
	  osSemaphoreAcquire(myBinarySem04Handle, osWaitForever);
	  Update_Standalone_Configuration();
  }
  /* USER CODE END Update_Standalone_Op_Task */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM7 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM7) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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

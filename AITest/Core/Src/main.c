/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body (Optimized for Debugging & Execution Profiling)
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "ai_platform.h"
#include "network.h"
#include "network_data.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
static ai_handle network_handle = AI_HANDLE_NULL;
volatile float prediction = 0.0f;

// Allocating the activations buffer in RAM
AI_ALIGNED(32)
static ai_u8 activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE];

// Explicit data buffers for the model
AI_ALIGNED(32)
static ai_float in_data[AI_NETWORK_IN_1_SIZE];
AI_ALIGNED(32)
static ai_float out_data[AI_NETWORK_OUT_1_SIZE];

// Pointers to the network's internal buffer configurations
static ai_buffer* ai_input;
static ai_buffer* ai_output;

// Profiling metrics (Read these inside Live Expressions)
volatile uint32_t start_cycles = 0;
volatile uint32_t stop_cycles = 0;
volatile uint32_t total_inference_cycles = 0;
volatile float execution_time_ms = 0.0f;

// 32-element floating-point test array
const float test_window[32] = {
    2.46153808f, 2.46642208f, 2.46642208f, 2.46764302f, 2.41758204f, 2.37851000f,
    2.46642208f, 2.46398020f, 2.45665407f, 2.46520114f, 2.46642208f, 2.46642208f,
    2.46642208f, 2.46398020f, 2.46520114f, 2.46642208f, 2.46520114f, 2.46520114f,
    2.46642208f, 2.19658089f, 2.46398020f, 2.46398020f, 2.46642208f, 2.46520114f,
    2.46275902f, 2.46764302f, 2.46520114f, 2.46642208f, 2.46275902f, 2.46642208f,
    2.46642208f, 2.46642208f
};
const int window_size = 32;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
static void AI_Init(void);
static void AI_Run(float *pIn, float *pOut);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void AI_Init(void) {
  ai_error err;
  ai_u16 n_in = 0;
  ai_u16 n_out = 0;

  // 1. Create the network object instance
  err = ai_network_create(&network_handle, AI_NETWORK_DATA_CONFIG);
  if (err.type != AI_ERROR_NONE) {
    while(1);
  }

  // 2. Initialize parameters cleanly without redundant outer braces
  const ai_network_params params = AI_NETWORK_PARAMS_INIT(
    AI_NETWORK_DATA_WEIGHTS(ai_network_data_weights_get()),
    AI_NETWORK_DATA_ACTIVATIONS(activations)
  );

  if (!ai_network_init(network_handle, &params)) {
    while(1);
  }

  // 3. Match the exact function signature defined in your network.h
  ai_input = ai_network_inputs_get(network_handle, &n_in);
  ai_output = ai_network_outputs_get(network_handle, &n_out);
}

static void AI_Run(float *pIn, float *pOut) {
  ai_i32 batch;

  // Bind data arrays directly to the payload pointers
  ai_input[0].data = AI_HANDLE_PTR(pIn);
  ai_output[0].data = AI_HANDLE_PTR(pOut);

  // Run the inference engine
  batch = ai_network_run(network_handle, ai_input, ai_output);
  if (batch != 1) {
    while(1);
  }
}
/* USER CODE END 0 */

int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */
  // Enable DWT Cycle Counter hardware blocks inside the Cortex-M4 core
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

  AI_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      // Load your 32 custom dataset points directly into the model inputs
      for (int i = 0; i < window_size; i++) {
          if (i < AI_NETWORK_IN_1_SIZE) {
              in_data[i] = test_window[i];
          }
      }

      // Profile inference window start time
      start_cycles = DWT->CYCCNT;

      // Run the model inference forward pass
      AI_Run(in_data, out_data);

      // Profile inference window stop time
      stop_cycles = DWT->CYCCNT;

      // Calculate active system execution cycle counts
      total_inference_cycles = stop_cycles - start_cycles;

      // Convert active cycle metrics into milliseconds (Based on your 64MHz clock config)
      execution_time_ms = (float)total_inference_cycles / 64000.0f;

      // Capture the target prediction result safely
      prediction = out_data[0];

      HAL_Delay(1000);
  }
  /* USER CODE END WHILE */
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16; // 4MHz HSI * 16 = 64MHz Clock Speed
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK

                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

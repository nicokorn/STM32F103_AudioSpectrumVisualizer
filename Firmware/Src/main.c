// ****************************************************************************
/// \file      main.c
///
/// \brief     Main C Source File
///
/// \details   This is the soundbar visualizer sourcecode. A microphone is
///            attached to the microntrollers adc. The average adc values are
///            used to set the visualizer bar.
///            The adc is triggered by a timer wich period is set to 8 kHz thus
///            the adc samples with 8 kHz. Because the interesting sound
///            information can be found up to 4 kHz and to fullfill Nyquists
///            theorem the sampling is set to 8 kHz. An irq is triggered
///            everytime 8 samples has been converted thus every 1 ms.
///            Sampling and loading data into the ws2812b leds is done at the
///            same time by the peripherals. The main loop is used to averrage
///            the sampled adc values, convert them into the visualizer bar
///            level and triggering the peripherals to start write led data
///            into the led stripe. 
///
/// \author    Nico Korn
///
/// \version   1.0.0.0
///
/// \date      05022022
/// 
/// \copyright Copyright (c) 2022 Nico Korn
/// 
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
/// 
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
/// 
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///
/// \pre       
///
/// \bug       
///
/// \warning   
///
/// \todo      
///
// ****************************************************************************

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "ws2812b.h"
#include "visualizer.h"
#include "touch.h"
#include "microphone.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
// If you connect a touch button to gpio PB11 
// for changing effects, uncomment the line below.
/*#define EFFECT_TOUCH_BUTTON*/  
                                 

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static uint16_t*  adcLevelFTT;
static uint8_t    ledLevel;

/* Private function prototypes -----------------------------------------------*/
void  SystemClock_Config(void);

/* Private user code ---------------------------------------------------------*/

// ----------------------------------------------------------------------------
/// \brief     Application entry point.
///
/// \param     none
///
/// \return    int
 int main( void )
{
   // Reset of all peripherals, Initializes the Flash interface and the Systick.
   HAL_Init();
   
   // Configure the system clock
   SystemClock_Config();
   
   // init peripherals for using the ws2812b leds
   WS2812B_init();
   
   // visualizer init
   visualizer_init();
   
#ifdef EFFECT_TOUCH_BUTTON
   // init mode touch button
   touch_init();
#endif
   
   // init microphone
   microphone_init();
   
   while (1)
   {
      // get ftt adc values
      adcLevelFTT = microphone_ftt();

      // clear led buffer
      WS2812B_clearBuffer();
      
      // draw the bars into the led buffer
      for(uint8_t bar=0; bar<NR_OF_COLS; bar++)
      {
         if( bar < NR_OF_COLS-1 )
         {
            // convert adc value into led level
            ledLevel = visualizer_convertDB( adcLevelFTT[2*bar+2] );
            
            // set and show led
            visualizer_setLevelBar( bar, ledLevel );
         }
         else
         {
            // convert adc value into led level
            ledLevel = visualizer_convertDB( adcLevelFTT[31] );
            
            // set and show led
            visualizer_setLevelBar( bar, ledLevel );
         }
      }

      // send the led buffer to the leds
      WS2812B_sendBuffer();      
   }
}

// ----------------------------------------------------------------------------
/// \brief     System Clock configuration according to the blue pills crystals.
///            HSE is set to 8 MHz and LSE is set to 32 kHz.
///
/// \param     none
///
/// \return    none
void SystemClock_Config( void )
{
   RCC_OscInitTypeDef RCC_OscInitStruct = {0};
   RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
   
   /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
   RCC_OscInitStruct.OscillatorType    = RCC_OSCILLATORTYPE_HSE;
   RCC_OscInitStruct.HSEState          = RCC_HSE_ON;
   RCC_OscInitStruct.HSEPredivValue    = RCC_HSE_PREDIV_DIV1;
   RCC_OscInitStruct.HSIState          = RCC_HSI_ON;
   RCC_OscInitStruct.PLL.PLLState      = RCC_PLL_ON;
   RCC_OscInitStruct.PLL.PLLSource     = RCC_PLLSOURCE_HSE;
   RCC_OscInitStruct.PLL.PLLMUL        = RCC_PLL_MUL9;
   if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
   {
      Error_Handler();
   }
   /** Initializes the CPU, AHB and APB buses clocks
   */
   RCC_ClkInitStruct.ClockType         = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
   RCC_ClkInitStruct.SYSCLKSource      = RCC_SYSCLKSOURCE_PLLCLK;
   RCC_ClkInitStruct.AHBCLKDivider     = RCC_SYSCLK_DIV1;
   RCC_ClkInitStruct.APB1CLKDivider    = RCC_HCLK_DIV2;
   RCC_ClkInitStruct.APB2CLKDivider    = RCC_HCLK_DIV1;
   
   if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
   {
      Error_Handler();
   }
}

// ----------------------------------------------------------------------------
/// \brief     Code piece automatically generated by the cube mx tool.
///
/// \param     none
///
/// \return    none
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

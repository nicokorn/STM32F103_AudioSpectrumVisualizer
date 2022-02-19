// ****************************************************************************
/// \file      touch.c
///
/// \brief     touch C Source File
///
/// \details   C file for the touch button.
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

// Include ********************************************************************
#include "touch.h"
#include "visualizer.h"

// Private define *************************************************************

// Private types     **********************************************************

// Private variables **********************************************************

// Private function prototypes ************************************************

// Private variables **********************************************************

// Functions ******************************************************************
// ----------------------------------------------------------------------------
/// \brief     Initialisation of the touch button.
///
/// \param     none
///
/// \return    none
void touch_init( void )
{   
	// init push button gpio
	__HAL_RCC_GPIOB_CLK_ENABLE();                   
	GPIO_InitTypeDef GPIO_InitStruct_Touch_BTN;
	GPIO_InitStruct_Touch_BTN.Pin 		= BUTTON_MODE_PIN; 	
	GPIO_InitStruct_Touch_BTN.Mode 		= GPIO_MODE_IT_FALLING; 			
	GPIO_InitStruct_Touch_BTN.Pull 		= GPIO_NOPULL;							
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct_Touch_BTN);
   
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

// ----------------------------------------------------------------------------
/// \brief     Callback function for touch event.
///
/// \param     none
///
/// \return    none
void touch_pressed( void )
{
   visualizer_nextEffect();
}
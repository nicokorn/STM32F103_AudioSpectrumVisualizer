// ****************************************************************************
/// \file      microphone.h
///
/// \brief     microphone Header File
///
/// \details   Module for microphone.
///
/// \author    Nico Korn
///
/// \version   1.0.0.0
///
/// \date      01012022
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

// Define to prevent recursive inclusion **************************************
#ifndef __MICROPHONE_H
#define __MICROPHONE_H

// Include ********************************************************************
#include "stm32f1xx_hal.h"

// Exported defines ***********************************************************
#define MICROPHONE_ANALOG_PIN       GPIO_PIN_0
#define MICROPHONE_DIGITAL_PIN      GPIO_PIN_1

// Exported types *************************************************************
typedef enum
{
   MICROPHONE_OK       = 0x00U,
   MICROPHONE_ERROR    = 0x01U,
   MICROPHONE_BUSY     = 0x02U,
   MICROPHONE_TIMEOUT  = 0x03U,
   MICROPHONE_READY    = 0x04U,
   MICROPHONE_RESET    = 0x05U
} MICROPHONE_StatusTypeDef;

// Exported functions *********************************************************
MICROPHONE_StatusTypeDef   microphone_init      ( void );
uint32_t                   microphone_getAdc    ( void );
uint16_t*                  microphone_ftt       ( void );
MICROPHONE_StatusTypeDef   microphone_startAdc  ( void );
#endif // __MICROPHONE_H
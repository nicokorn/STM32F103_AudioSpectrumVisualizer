// ****************************************************************************
/// \file      equalizer.h
///
/// \brief     equalizerC HeaderFile
///
/// \details   Module for equalizer.
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
#ifndef __EQUALIZER_H
#define __EQUALIZER_H

// Include ********************************************************************
#include "stm32f1xx_hal.h"

// Exported defines ***********************************************************
#define NR_OF_ROWS        ( 8u )
#define NR_OF_COLS        ( 16u )

// Exported types *************************************************************
typedef enum
{
   EQUALIZER_OK       = 0x00U,
   EQUALIZER_ERROR    = 0x01U,
   EQUALIZER_BUSY     = 0x02U,
   EQUALIZER_TIMEOUT  = 0x03U,
   EQUALIZER_READY    = 0x04U,
   EQUALIZER_RESET    = 0x05U
} EQUALIZER_StatusTypeDef;

// Exported functions *********************************************************
EQUALIZER_StatusTypeDef    equalizer_init          ( void );
void                       equalizer_nextEffect    ( void );
uint8_t                    equalizer_convertABS    ( uint32_t adcValue );
void                       equalizer_setLevelBar   ( uint8_t bar, uint8_t level );
void                       equalizer_setPixel      ( uint8_t x, uint8_t y, uint8_t red, uint8_t green, uint8_t blue );
uint8_t                    equalizer_convertDB     ( uint32_t dbValue );
#endif // __EQUALIZER_H
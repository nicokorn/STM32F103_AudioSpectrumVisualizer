// ****************************************************************************
/// \file      equalizer.c
///
/// \brief     equalizer C Source File
///
/// \details   Module for equalizer leds.
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

// Include ********************************************************************
#include "equalizer.h"
#include "ws2812b.h"

// Private define *************************************************************
#define NR_OF_EFFECTS     ( 4u )
#define EFFECT_OPT_FADE   ( 0x01 )
#define NR_OF_BARS        ( 16u )

// Private types     **********************************************************
typedef __packed struct equalizer_s{
   const uint8_t*    effect;
   uint8_t           effectIndex;
   FlagStatus        effectChanged;
   uint8_t           level;
   uint8_t           levelTop;
   uint8_t           levelFil;
   uint16_t          levelTopTime;
   uint16_t          levelFilTime;
}euqalizer_t;

// Private variables **********************************************************
static const uint8_t effect_1[COL+1][3] =        { { 0x00, 0xff, 0x00 },     // green
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0xff, 0x80, 0x00 },
                                                   { 0xff, 0x80, 0x00 },
                                                   { 0xff, 0x80, 0x00 },
                                                   { 0xff, 0x80, 0x00 },
                                                   { 0xff, 0x00, 0x00 },      // red
                                                   { 0x00, 0x00, 0x00 } };    // reserved effect option flags

static const uint8_t effect_2[COL+1][3] =        { { 0x00, 0x00, 0xff },     
                                                   { 0x11, 0x00, 0xee },
                                                   { 0x22, 0x00, 0xdd },
                                                   { 0x33, 0x00, 0xcc },
                                                   { 0x44, 0x00, 0xbb },
                                                   { 0x55, 0x00, 0xaa },
                                                   { 0x66, 0x00, 0x99 },
                                                   { 0x77, 0x00, 0x88 },
                                                   { 0x99, 0x00, 0x66 },
                                                   { 0xaa, 0x00, 0x55 },
                                                   { 0xbb, 0x00, 0x44 },
                                                   { 0xcc, 0x00, 0x33 },
                                                   { 0xdd, 0x00, 0x22 },
                                                   { 0xee, 0x00, 0x11 },
                                                   { 0xff, 0x00, 0x00 },
                                                   { 0x00, 0x00, 0x00 } };    // reserved effect option flags

static const uint8_t effect_3[COL+1][3] =        { { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xee, 0x11 },
                                                   { 0x00, 0xdd, 0x22 },
                                                   { 0x00, 0xcc, 0x33 },
                                                   { 0x00, 0xbb, 0x44 },
                                                   { 0x00, 0xaa, 0x55 },
                                                   { 0x00, 0x99, 0x66 },
                                                   { 0x00, 0x88, 0x77 },
                                                   { 0x00, 0x77, 0x88 },
                                                   { 0x00, 0x66, 0x99 },
                                                   { 0x00, 0x55, 0xaa },
                                                   { 0x00, 0x44, 0xbb },
                                                   { 0x00, 0x33, 0xcc },
                                                   { 0x00, 0x22, 0xdd },
                                                   { 0x00, 0x11, 0xee },
                                                   { 0x00, 0x00, 0x00 } };    // reserved effect option flags

static const uint8_t effect_4[COL+1][3] =        { { 0xb2, 0x0e, 0x00 },     
                                                   { 0xb8, 0x1d, 0x00 },
                                                   { 0xbd, 0x2b, 0x00 },
                                                   { 0xc3, 0x39, 0x00 },
                                                   { 0xc8, 0x48, 0x00 },
                                                   { 0xce, 0x56, 0x00 },
                                                   { 0xd3, 0x64, 0x00 },
                                                   { 0xd9, 0x73, 0x00 },
                                                   { 0xde, 0x81, 0x00 },
                                                   { 0xe4, 0x8f, 0x00 },
                                                   { 0xe9, 0x9e, 0x00 },
                                                   { 0xef, 0xac, 0x00 },
                                                   { 0xf4, 0xba, 0x00 },
                                                   { 0xfa, 0xc9, 0x00 },
                                                   { 0xff, 0xd7, 0x00 },
                                                   { 0x00, 0x00, 0x00 } };    // reserved effect option flags

static const uint8_t *effects[NR_OF_EFFECTS] = { &effect_1[0][0], &effect_2[0][0], &effect_3[0][0], &effect_4[0][0] };
                                                           
// Private function prototypes ************************************************

// Private variables **********************************************************
static euqalizer_t equalizer;
static euqalizer_t equalizerSet[NR_OF_BARS];
static const float m = ((float)15/(float)2048);

// Functions ******************************************************************
// ----------------------------------------------------------------------------
/// \brief     Initialisation of the graphic equalizer/visualizer.
///
/// \param     none
///
/// \return    EQUALIZER_StatusTypeDef
EQUALIZER_StatusTypeDef equalizer_init( void )
{   
   // only one bar
   equalizer.levelFil      = 0;
   equalizer.levelFilTime  = 0;
   equalizer.levelTop      = 0;
   equalizer.levelTopTime  = 0;
   equalizer.level         = 0;
   equalizer.effectIndex   = 0;
   equalizer.effect        = effects[equalizer.effectIndex%NR_OF_EFFECTS];
   equalizer.effectChanged = SET;
   
   // 16 bars
   for( uint8_t bar=0; bar<NR_OF_BARS; bar++ )
   {
      equalizerSet[bar].levelFil      = 0;
      equalizerSet[bar].levelFilTime  = 0;
      equalizerSet[bar].levelTop      = 0;
      equalizerSet[bar].levelTopTime  = 0;
      equalizerSet[bar].level         = 0;
      equalizerSet[bar].effectIndex   = 0;
      equalizerSet[bar].effect        = effects[equalizer.effectIndex%NR_OF_EFFECTS];
      equalizerSet[bar].effectChanged = SET;
   }
   
   return EQUALIZER_OK;
}

// ----------------------------------------------------------------------------
/// \brief     Set equalizer bar level.
///
/// \param     [in]  uint8_t level
///
/// \return    none
void equalizer_setLevel( uint8_t level )
{   
   static uint8_t levelFilCounter;
   levelFilCounter++;
   static uint8_t levelTopCounter;
   levelTopCounter++;
   
   if( level >= COL )
   {
      return;
   }
   
   // level of the bar
   if( equalizer.effectChanged != RESET )
   {
      equalizer.level         = COL-1;
      equalizer.effectChanged = RESET;
   }
   else
   {
      equalizer.level = level;
   }
   
   // set filled level bar
   if( equalizer.levelFil < equalizer.level )
   {
      equalizer.levelFil      = equalizer.level;
      equalizer.levelFilTime  = 50;
   }
   else if( equalizer.levelFilTime )
   {
      equalizer.levelFilTime--;
   }
   else if( equalizer.levelFil > 0 && !(levelFilCounter%30) )
   {
      equalizer.levelFil--;
      levelFilCounter = 0;
   }

   // set max top level bar
   if( equalizer.levelTop <= equalizer.levelFil )
   {
      equalizer.levelTop      = equalizer.levelFil;
      equalizer.levelTopTime  = 100;
   }
   else if( equalizer.levelTopTime )
   {
      equalizer.levelTopTime--;
   }
   else if( equalizer.levelTop > 0 && !(levelTopCounter%80) )
   {
      equalizer.levelTop--;
      levelTopCounter = 0;
   }
   
   // draw the bar
   WS2812B_clearBuffer();
   for( uint8_t i=0; i<equalizer.levelFil; i++ )
   {
      if( EFFECT_OPT_FADE == *(equalizer.effect+COL*3+0))
      {
         WS2812B_setPixel( 0, i, *(equalizer.effect+i*3+0)*level, *(equalizer.effect+i*3+1)*level, *(equalizer.effect+i*3+2)*level ); 
      }
      else
      {
         WS2812B_setPixel( 0, i, *(equalizer.effect+i*3+0), *(equalizer.effect+i*3+1), *(equalizer.effect+i*3+2) ); 
      }
   }
   WS2812B_setPixel( 0, equalizer.levelTop, 0xFF, 0x00, 0x00 );
   WS2812B_sendBuffer();
}

// ----------------------------------------------------------------------------
/// \brief     Set equalizer bar level.
///
/// \param     [in]  uint8_t bar
/// \param     [in]  uint8_t level
///
/// \return    none
void equalizer_setLevelBar( BAR_StatusTypeDef bar, uint8_t level )
{   
   static uint8_t levelFilCounter;
   levelFilCounter++;
   static uint8_t levelTopCounter;
   levelTopCounter++;
   
   if( level >= COL )
   {
      return;
   }
   
   // level of the bar
   if( equalizerSet[bar].effectChanged != RESET )
   {
      equalizerSet[bar].level         = COL-1;
      equalizerSet[bar].effectChanged = RESET;
   }
   else
   {
      equalizerSet[bar].level = level;
   }
   
   // set filled level bar
   if( equalizerSet[bar].levelFil < equalizerSet[bar].level )
   {
      equalizerSet[bar].levelFil      = equalizerSet[bar].level;
      equalizerSet[bar].levelFilTime  = 50;
   }
   else if( equalizerSet[bar].levelFilTime )
   {
      equalizerSet[bar].levelFilTime--;
   }
   else if( equalizerSet[bar].levelFil > 0 && !(levelFilCounter%30) )
   {
      equalizerSet[bar].levelFil--;
      levelFilCounter = 0;
   }

   // set max top level bar
   if( equalizerSet[bar].levelTop <= equalizerSet[bar].levelFil )
   {
      equalizerSet[bar].levelTop      = equalizerSet[bar].levelFil;
      equalizerSet[bar].levelTopTime  = 100;
   }
   else if( equalizerSet[bar].levelTopTime )
   {
      equalizerSet[bar].levelTopTime--;
   }
   else if( equalizerSet[bar].levelTop > 0 && !(levelTopCounter%80) )
   {
      equalizerSet[bar].levelTop--;
      levelTopCounter = 0;
   }
   
   // draw the bar
   WS2812B_clearBuffer();
   for( uint8_t i=0; i<equalizerSet[bar].levelFil; i++ )
   {
      if( EFFECT_OPT_FADE == *(equalizerSet[bar].effect+COL*3+0))
      {
         WS2812B_setPixel( 0, i, *(equalizerSet[bar].effect+i*3+0)*level, *(equalizerSet[bar].effect+i*3+1)*level, *(equalizerSet[bar].effect+i*3+2)*level ); 
      }
      else
      {
         WS2812B_setPixel( 0, i, *(equalizerSet[bar].effect+i*3+0), *(equalizerSet[bar].effect+i*3+1), *(equalizerSet[bar].effect+i*3+2) ); 
      }
   }
   WS2812B_setPixel( 0, equalizerSet[bar].levelTop, 0xFF, 0x00, 0x00 );
   WS2812B_sendBuffer();
}

// ----------------------------------------------------------------------------
/// \brief     Choose next effect.
///
/// \param     none
///
/// \return    none
void equalizer_nextEffect( void )
{   
   equalizer.effectIndex++;
   equalizer.effect        = effects[equalizer.effectIndex%NR_OF_EFFECTS];
   equalizer.effectChanged = SET;
}

// ----------------------------------------------------------------------------
/// \brief     Convert adc value to 15 leds.
///
/// \param     [in] uint32_t adcValue
///
/// \return    uint8_t led level
uint8_t equalizer_convert( uint32_t adcValue )
{  
   return (uint8_t)(m*(float)adcValue);
}

// ----------------------------------------------------------------------------
/// \brief     Set pixel.
///
/// \param     ...
///
/// \return    none
void equalizer_setPixel( uint8_t x, uint8_t y, uint8_t red, uint8_t green, uint8_t blue )
{  
   uint8_t ledNr;
   
   // select tile
   if( x > 7 )
   {
      x -= 8;
      ledNr = y*8+7-x;
      WS2812B_setPixel( 1, ledNr, red, green, blue ); 
   }
   else
   {
      ledNr = y*8+7-x;
      WS2812B_setPixel( 0, ledNr, red, green, blue ); 
   }
}
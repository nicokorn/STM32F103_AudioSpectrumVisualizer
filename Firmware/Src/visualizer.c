// ****************************************************************************
/// \file      visualizer.c
///
/// \brief     visualizer C Source File
///
/// \details   Module for visualizer. The visualizer is used to draw the bars
///            on to the led stripe.
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
#include "visualizer.h"
#include "ws2812b.h"

// Private define *************************************************************
#define NR_OF_EFFECTS     ( 4u )
#define NR_OF_BARS        ( 16u )
#define EFFECT_OPT_FADE   ( 0x01 )

// Private types     **********************************************************
typedef __packed struct visualizer_s{
   const uint8_t*    effect;
   uint8_t           effectIndex;
   FlagStatus        effectChanged;
   uint8_t           level;
   uint8_t           levelTop;
   uint8_t           levelFil;
   uint16_t          levelTopTime;
   uint16_t          levelFilTime;
   uint8_t           levelFilCounter;
   uint8_t           levelTopCounter;
}euqalizer_t;

// Private variables **********************************************************
static const uint8_t effect_1[NR_OF_ROWS+1][3] = { { 0x00, 0xff, 0x00 },     // 1. bar
                                                   { 0x00, 0xff, 0x00 },     // 2. bar
                                                   { 0x00, 0xff, 0x00 },     // 3. bar
                                                   { 0x00, 0xff, 0x00 },     // 4. bar
                                                   { 0x00, 0xff, 0x00 },     // 5. bar
                                                   { 0xff, 0x80, 0x00 },     // 6. bar
                                                   { 0xff, 0x80, 0x00 },     // 7. bar
                                                   { 0xff, 0x00, 0x00 },     // 8. bar
                                                   { 0x00, 0x00, 0x00 } };    // reserved for effect option flags

static const uint8_t effect_2[NR_OF_ROWS+1][3] = { { 0x00, 0x00, 0xff },     
                                                   { 0x22, 0x00, 0xdd },
                                                   { 0x44, 0x00, 0xbb },
                                                   { 0x66, 0x00, 0x99 },
                                                   { 0x99, 0x00, 0x66 },
                                                   { 0xbb, 0x00, 0x44 },
                                                   { 0xdd, 0x00, 0x22 },
                                                   { 0xff, 0x00, 0x00 },
                                                   { 0x00, 0x00, 0x00 } };    // reserved for effect option flags

static const uint8_t effect_3[NR_OF_ROWS+1][3] = { { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xdd, 0x22 },
                                                   { 0x00, 0xbb, 0x44 },
                                                   { 0x00, 0x99, 0x66 },
                                                   { 0x00, 0x77, 0x88 },
                                                   { 0x00, 0x55, 0xaa },
                                                   { 0x00, 0x33, 0xcc },
                                                   { 0x00, 0x11, 0xee },
                                                   { 0x00, 0x00, 0x00 } };    // reserved for effect option flags

static const uint8_t effect_4[NR_OF_ROWS+1][3] = { { 0xb2, 0x0e, 0x00 },     
                                                   { 0xbd, 0x2b, 0x00 },
                                                   { 0xc8, 0x48, 0x00 },
                                                   { 0xd3, 0x64, 0x00 },
                                                   { 0xde, 0x81, 0x00 },
                                                   { 0xe9, 0x9e, 0x00 },
                                                   { 0xf4, 0xba, 0x00 },
                                                   { 0xff, 0xd7, 0x00 },
                                                   { 0x00, 0x00, 0x00 } };    // reserved for effect option flags

static const uint8_t *effects[NR_OF_EFFECTS] = { &effect_1[0][0], &effect_2[0][0], &effect_3[0][0], &effect_4[0][0] };
static euqalizer_t visualizerSet[NR_OF_BARS];
static const float mABS = ((float)8/(float)2048);
static const float mDB  = ((float)8/(float)11);

// Private function prototypes ************************************************

// Functions ******************************************************************
// ----------------------------------------------------------------------------
/// \brief     Initialisation of the graphic visualizer.
///
/// \param     none
///
/// \return    VISUALIZER_StatusTypeDef
VISUALIZER_StatusTypeDef visualizer_init( void )
{   
   // 16 bars
   for( uint8_t bar=0; bar<NR_OF_BARS; bar++ )
   {
      visualizerSet[bar].levelFil      = 0;
      visualizerSet[bar].levelFilTime  = 0;
      visualizerSet[bar].levelTop      = 0;
      visualizerSet[bar].levelTopTime  = 0;
      visualizerSet[bar].level         = 0;
      visualizerSet[bar].effectIndex   = 2;
      visualizerSet[bar].effect        = effects[visualizerSet[bar].effectIndex%NR_OF_EFFECTS];
      visualizerSet[bar].effectChanged = SET;
   }
   
   return VISUALIZER_OK;
}

// ----------------------------------------------------------------------------
/// \brief     Set visualizer bar level.
///
/// \param     [in]  uint8_t bar
/// \param     [in]  uint8_t level
///
/// \return    none
void visualizer_setLevelBar( uint8_t bar, uint8_t level )
{   
   visualizerSet[bar].levelFilCounter++;
   visualizerSet[bar].levelTopCounter++;
   
   if( level >= NR_OF_ROWS )
   {
      return;
   }
   
   // level of the bar
   if( visualizerSet[bar].effectChanged != RESET )
   {
      visualizerSet[bar].level         = NR_OF_ROWS-1;
      visualizerSet[bar].effectChanged = RESET;
   }
   else
   {
      visualizerSet[bar].level = level;
   }
   
   // set filled level bar
   if( visualizerSet[bar].levelFil < visualizerSet[bar].level )
   {
      visualizerSet[bar].levelFil      = visualizerSet[bar].level;
      visualizerSet[bar].levelFilTime  = 6;
   }
   else if( visualizerSet[bar].levelFilTime )
   {
      visualizerSet[bar].levelFilTime--;
   }
   else if( visualizerSet[bar].levelFil > 0 && !(visualizerSet[bar].levelFilCounter%3) )
   {
      visualizerSet[bar].levelFil--;
      visualizerSet[bar].levelFilCounter = 0;
   }

   // set max top level bar
   if( visualizerSet[bar].levelTop <= visualizerSet[bar].levelFil )
   {
      visualizerSet[bar].levelTop      = visualizerSet[bar].levelFil;
      visualizerSet[bar].levelTopTime  = 12;
   }
   else if( visualizerSet[bar].levelTopTime )
   {
      visualizerSet[bar].levelTopTime--;
   }
   else if( visualizerSet[bar].levelTop > 0 && !(visualizerSet[bar].levelTopCounter%10) )
   {
      visualizerSet[bar].levelTop--;
      visualizerSet[bar].levelTopCounter = 0;
   }
   
   // draw the bar
   for( uint8_t i=0; i<visualizerSet[bar].levelFil; i++ )
   {
      visualizer_setPixel_180deg( bar, i, *(visualizerSet[bar].effect+i*3+0), *(visualizerSet[bar].effect+i*3+1), *(visualizerSet[bar].effect+i*3+2) ); 
   }
   visualizer_setPixel_180deg( bar, visualizerSet[bar].levelTop, 0xFF, 0x00, 0x00 );
}

// ----------------------------------------------------------------------------
/// \brief     Choose next effect.
///
/// \param     none
///
/// \return    none
void visualizer_nextEffect( void )
{   
   // 16 bars
   for( uint8_t bar=0; bar<NR_OF_BARS; bar++ )
   {
      visualizerSet[bar].effectIndex++;
      visualizerSet[bar].effect        = effects[visualizerSet[bar].effectIndex%NR_OF_EFFECTS];
      visualizerSet[bar].effectChanged = SET;
   }
}

// ----------------------------------------------------------------------------
/// \brief     Convert absolute adc value to 8 leds.
///
/// \param     [in] uint32_t adcValue
///
/// \return    uint8_t led level
uint8_t visualizer_convertABS( uint32_t absValue )
{  
   if( absValue >= 2048u )
   {
      return 7u;
   }
   return (uint8_t)(mABS*(float)absValue);
}

// ----------------------------------------------------------------------------
/// \brief     Convert decibel adc value to 15 leds.
///
/// \param     [in] uint32_t dbValue
///
/// \return    uint8_t led level
uint8_t visualizer_convertDB( uint32_t dbValue )
{  
   if( dbValue >= 11u )
   {
      return 7u;
   }
   return (uint8_t)(mDB*(float)dbValue);
}

// ----------------------------------------------------------------------------
/// \brief     Set pixel with x/y coordinates. The x/y coordinates will be
///            converted to the led number on the stripe. Color is controlled
///            with the red, green, blue arguments which will be interpreted as
///            rgb color.
///
/// \param     [in]  uint8_t x
/// \param     [in]  uint8_t y
/// \param     [in]  uint8_t red
/// \param     [in]  uint8_t green
/// \param     [in]  uint8_t blue
///
/// \return    none
void visualizer_setPixel_0deg( uint8_t x, uint8_t y, uint8_t red, uint8_t green, uint8_t blue )
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

void visualizer_setPixel_180deg( uint8_t x, uint8_t y, uint8_t red, uint8_t green, uint8_t blue )
{  
   uint8_t ledNr;
   
   // select tile
   if( x > 7 )
   {
      x -= 8;
      ledNr = (7-y)*8+x;
      WS2812B_setPixel( 0, ledNr, red, green, blue ); 
   }
   else
   {
      ledNr = (7-y)*8+x;
      WS2812B_setPixel( 1, ledNr, red, green, blue ); 
   }
}
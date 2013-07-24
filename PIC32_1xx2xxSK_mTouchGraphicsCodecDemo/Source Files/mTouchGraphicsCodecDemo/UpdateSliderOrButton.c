/*****************************************************************************
* FileName:         UpdateSliderOrButton.c
* Includes:
*   void UpdateSlider( UINT16 SliderValue )
*   void UpdateButton( UINT16 iButton, BOOL ButtonStatus, BOOL ButtonAssert, char *ButtonLabel )
*
* Dependencies:     ??
* Processor:        PIC32
* Compiler:         C32
* Linker:           MPLAB LINK32
* Company:          Microchip Technology Incorporated
*
* Software License Agreement
*
* Copyright © 2011 Microchip Technology Inc.
* Microchip licenses this software to you solely for use with Microchip
* products, according to the terms of the accompanying click-wrap software
* license. Microchip and its licensors retain all right, title and interest in
* and to the software.  All rights reserved. This software and any accompanying
* information is for suggestion only. It shall not be deemed to modify
* Microchip’s standard warranty for its products.  It is your responsibility to
* ensure that this software meets your requirements.
*
* SOFTWARE IS PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR
* IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
* NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL
* MICROCHIP OR ITS LICENSORS BE LIABLE FOR ANY DIRECT OR INDIRECT DAMAGES OR
* EXPENSES INCLUDING BUT NOT LIMITED TO INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
* OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
* SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, OR ANY CLAIMS BY THIRD PARTIES
* (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*
* The aggregate and cumulative liability of Microchip and its licensors for
* damages related to the use of the software will in no event exceed $1000 or
* the amount you paid Microchip for the software, whichever is greater.
*
* MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
* TERMS AND THE TERMS OF THE ACCOMPANYING CLICK-WRAP SOFTWARE LICENSE.
*
*
* Author                    Date                Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MWM           2-Aug-2011 Created for PIC32MX1xx/2xx Starter Kit Demo app
*******************************************************************************/
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "Graphics.h"
#include DISPLAY_FONT_HEADER

// From AssertToDisplay.c
extern char MessageString[250];

UINT16 SliderValue = 0; // Used by WM8731 interrupt to scale incoming buffers of music

#define WAIT_UNTIL_FINISH(x)    while(!x)

void UpdateSlider( UINT16 NewSliderValue )
{
    static UINT16 OldSliderValue = 0;
    UINT16 LengthGray, LengthRed;

    NewSliderValue = ( NewSliderValue > 255 ? 255 : NewSliderValue ); // Max value = 255
  //WM8731CodecSetVolume(0,((5*NewSliderValue)>>3)+48);

    if ( NewSliderValue != OldSliderValue )
    {
        SliderValue = NewSliderValue;
        OldSliderValue = NewSliderValue;

        LengthRed = NewSliderValue*GetMaxY()/256;
        LengthGray = GetMaxY()-LengthRed;
        SetColor(DARKGRAY);
//      WAIT_UNTIL_FINISH( Bar(GetMaxX()-12,1, GetMaxX()-8,LengthGray-1) );
        Bar(GetMaxX()-12,1, GetMaxX()-8,LengthGray-1);
        SetColor(BRIGHTRED);
//      WAIT_UNTIL_FINISH( Bar(GetMaxX()-12,LengthGray, GetMaxX()-8,GetMaxY()-1) );
        Bar(GetMaxX()-12,LengthGray, GetMaxX()-8,GetMaxY()-1);
    }
}


void UpdateButton( UINT16 iButton, BOOL ButtonStatus, BOOL ButtonAssert, char *ButtonLabel )
{
    static BOOL OldButtonStatus[4] = { TRUE, TRUE, TRUE, TRUE }; // Initialized to support initial draw
    static UINT16 OldLabelWidths[4], // Dimensions of previous labels
                  OldLabelHeights[4];
    UINT16 X0, Y0, X1, Y1; // Dimensions for erasure bar to remove previous label
    char * pButtonLabel, * pNextLabelString, * pEndButtonLabel;
    UINT16 nLines; // Number of lines used in this button label

    if      ( ButtonStatus == -1 )
    {
            SetColor(DARKGRAY);
        WAIT_UNTIL_FINISH(FillCircle(GetMaxX()-33,GetMaxY()-(22+(iButton-1)*44),10));
    }
    else if ( ButtonStatus != OldButtonStatus[iButton-1] )
    {
        OldButtonStatus[iButton-1] = ButtonStatus;
        if ( ButtonStatus == TRUE )
        {
            SetColor(BRIGHTRED);
        }
        else
        {
            SetColor(DARKGRAY);
        }
        WAIT_UNTIL_FINISH(FillCircle(GetMaxX()-33,GetMaxY()-(22+(iButton-1)*44),10));
    }

    // Always redraw assert holes
    if ( ButtonAssert == TRUE )
    {
        SetColor(BRIGHTRED);
    }
    else
    {
        SetColor(LIGHTGRAY);
    }
    WAIT_UNTIL_FINISH(FillCircle(GetMaxX()-33,GetMaxY()-(22+(iButton-1)*44),4));

    if ( ButtonLabel != 0 ) // Lable/Relabel  button
    {
        strcpy(MessageString,ButtonLabel);

        UINT16 LabelHeight, LabelWidth;

        SetFont((void *) &DISPLAY_FONT);
        LabelHeight = GetTextHeight((void *) &DISPLAY_FONT);

        X0 = GetMaxX()-(44+OldLabelWidths[iButton-1]);
        Y0 = GetMaxY()-(22+(iButton-1)*44)-LabelHeight/2;
        X1 = X0 + OldLabelWidths[iButton-1];
        Y1 = Y0 + OldLabelHeights[iButton-1];

        SetColor(BLACK);
        WAIT_UNTIL_FINISH( Bar(X0,Y0,X1,Y1) ); // Erase previous label

        if ( iButton == 1 ) // Redraw bottom of screen, it get's erased on bottom label.
        {
            SetColor(BRIGHTBLUE);
            WAIT_UNTIL_FINISH(Line(0,GetMaxY(),GetMaxX(),GetMaxY()));
        }

        nLines = 1;
        pButtonLabel = MessageString;
        pNextLabelString = pEndButtonLabel = pButtonLabel + strlen(pButtonLabel);
        OldLabelWidths[iButton-1] = 0;  // Zero out to calculate max width;

        SetColor(WHITE);
        while( pEndButtonLabel - pButtonLabel > 0 )
        {
            pNextLabelString = strchr(pButtonLabel,';');
            if ( pNextLabelString != NULL )
            {
                *pNextLabelString = 0;
            }

            LabelWidth  = GetTextWidth(pButtonLabel, (void *) &DISPLAY_FONT);
            OldLabelWidths[iButton-1] = max( OldLabelWidths[iButton-1], LabelWidth );

            X0 = GetMaxX()-(44+LabelWidth);
            Y0 = GetMaxY()
                 - ( 22 + (iButton-1)*44 + LabelHeight/2 )
                 + (nLines-1)*LabelHeight;
            WAIT_UNTIL_FINISH( OutTextXY(X0,Y0,(char *)pButtonLabel) );

            nLines++;
            pButtonLabel = pNextLabelString+1;
        }
        OldLabelHeights[iButton-1] = (nLines-1)*(1+LabelHeight);


    }

}

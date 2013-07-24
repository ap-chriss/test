/*****************************************************************************
* FileName:         InitDisplay.c
* Includes:
*   void InitDisplay(BOOL WithLabels)
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
#include "SwitchCode.h"
#include "UpdateSliderOrButton.h"
#include DISPLAY_FONT_HEADER

enum
{
    DISABLE=0,
    ENABLE
};

#define WAIT_UNTIL_FINISH(x)    while(!x)

void InitDisplay(BOOL WithLabels)
{
    UINT16 iTitle;
    char  *pAppTitles[] = { "PIC32MX2XX",
                            " Starter Kit",
                            "  mTouch",
                            "   +CODEC",
                            "    +Graphics",
                            "       Demo" };
    UINT16 nAppTitles = 6;
    char *pSwitchTitles[] = { "Switch 1", "Switch 2", "Switch 3" };
    UINT16 nSwitchTitles = 3;

    char *pButtonTitles[] = { "Button 1", "Button 2", "Button 3", "Button 4" };
    UINT16 nButtonTitles = 4;

    UINT16 LabelHeight;

    SetColor(BLACK);
    WAIT_UNTIL_FINISH(Bar(0,0,GetMaxX(),GetMaxY()));

    SetColor(BRIGHTBLUE);
    WAIT_UNTIL_FINISH(Line(0,0,GetMaxX(),0));
    WAIT_UNTIL_FINISH(Line(GetMaxX(),0,GetMaxX(),GetMaxY()));
    WAIT_UNTIL_FINISH(Line(0,GetMaxY(),GetMaxX(),GetMaxY()));
    WAIT_UNTIL_FINISH(Line(0,0,0,GetMaxY()));

    // Write out titles
    SetColor(BRIGHTRED);
    SetFont((void *) &DISPLAY_FONT);
    LabelHeight =  GetTextHeight((void *) &DISPLAY_FONT);
    for ( iTitle = 0; iTitle < nAppTitles; iTitle++ )
    {
        WAIT_UNTIL_FINISH( OutTextXY(2,2+iTitle*(LabelHeight-2),pAppTitles[iTitle]) );
    }


    // Draw background for slider
    SetColor(LIGHTGRAY);
    WAIT_UNTIL_FINISH( Bar(GetMaxX()-21,1, GetMaxX() -1,GetMaxY()-1) );
    SetColor(BRIGHTBLUE);
    WAIT_UNTIL_FINISH(Line(GetMaxX()-22,1, GetMaxX()-22,GetMaxY()-1) );

    // Draw background for buttons
    SetColor(LIGHTGRAY);
    WAIT_UNTIL_FINISH( Bar(GetMaxX()-43,1, GetMaxX()-23,GetMaxY()-1) );
    SetColor(BRIGHTBLUE);
    WAIT_UNTIL_FINISH(Line(GetMaxX()-44,1, GetMaxX()-44,GetMaxY()-1));

    // Draw Switches
    for ( iTitle = 0; iTitle < nSwitchTitles; iTitle ++ )
    {
        if ( WithLabels == TRUE )
        {
            UpdateSwitch( iTitle+1, OFF, pSwitchTitles[iTitle] );
        }
        else
        {
            UpdateSwitch( iTitle+1, OFF, 0 );
        }
    }

    // Draw Buttons
    for ( iTitle = 0; iTitle < nButtonTitles; iTitle ++ )
    {
        if ( WithLabels == TRUE )
        {
            UpdateButton( iTitle+1, -1, FALSE, pButtonTitles[iTitle] );
        }
        else
        {
            UpdateButton( iTitle+1, -1, FALSE, 0 );
        }
    }

    // Draw slider
    UpdateSlider( 0 );

}

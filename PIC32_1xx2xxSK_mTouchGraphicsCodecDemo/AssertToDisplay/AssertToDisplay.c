/*****************************************************************************
* FileName:         AssertToDisplay.c
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
* MWM          28 July 2011 Initial implementatoin
*******************************************************************************/

#include "Graphics.h"
#include DISPLAY_FONT_HEADER
#include <string.h>
#include "AssertToDisplay.h"
#include "SwitchCode.h"

#define WAIT_UNTIL_FINISH(x)    while(!x)

char MessageString[250];
char DisplayString[250];

void AssertToDisplay(char *Expression, UINT16 Line, char *Filename)
{
    char *pDisplayString, *pEndDisplayString, *pEndString;
    UINT16 LabelHeight;
    UINT16 nLines;
    enum SWITCHST8S Switch1St8;

    SetColor(BLACK);
    WAIT_UNTIL_FINISH(Bar(0,0,GetMaxX(),GetMaxY()));

    SetColor(BRIGHTRED);
    SetFont((void *) &DISPLAY_FONT);
    LabelHeight =  GetTextHeight((void *) &DISPLAY_FONT);

//  tfp_sprintf(MessageString,"Failed assertion `%s' at line %d of file `%s'",Expression,Line,Filename);
    strcpy(MessageString,"Failed assertion ");
    strcat(MessageString,Expression);
    strcat(MessageString," at line ");
    UnsignedInt2String(Line,10,MessageString + strlen(MessageString));
    strcat(MessageString," of file ");
    strcat(MessageString,Filename);

    nLines = 1;
    strcpy(DisplayString,MessageString);
    pDisplayString = DisplayString;
    pEndDisplayString = pEndString = pDisplayString + strlen(pDisplayString);

    while( pEndString - pDisplayString > 0 )
    {
        while( GetTextWidth(pDisplayString,(void *)&DISPLAY_FONT) > GetMaxX() )
        {
            *(pEndString--)=0;
        }
        WAIT_UNTIL_FINISH( OutTextXY(1,(nLines-1)*(1+LabelHeight),pDisplayString) );
        nLines++;
        strcpy(DisplayString,MessageString);
        pDisplayString = pEndString+1;
        pEndString = pEndDisplayString;
    }

    UpdateSwitch( 1, OFF, "Restart?" );

    while(1)
    {
        DelayMs(100);
        Switch1St8 = UpdateSwitch( 1, CheckSwitch1(), (char *)0 );
        if ( Switch1St8 == ON )
        {
            mSysUnlockOpLock(RSWRSTbits.SWRST=1); // reset device
            mSysUnlockOpLock(RSWRSTbits.SWRST=1); // reset device
        }
    }
}

void UnsignedInt2String(UINT16 UnsignedInt,const UINT16 NumberBase,char * String)
{
    INT16  iDigit;
    UINT16 iMaxDigit  = 0;
    UINT16 DigitPower = 1;

    while (UnsignedInt/DigitPower >= NumberBase)
    {
        DigitPower *= NumberBase;
        iMaxDigit++;
    }

    for (iDigit = iMaxDigit; iDigit >= 0; iDigit--)
    {
        UINT16 DigitValue = UnsignedInt / DigitPower;
        UnsignedInt %= DigitPower;
        DigitPower/=NumberBase;
        *String++ = DigitValue + (DigitValue<10 ? '0' : 'A'-10);
    }

    *String=0; //Terminate string
}

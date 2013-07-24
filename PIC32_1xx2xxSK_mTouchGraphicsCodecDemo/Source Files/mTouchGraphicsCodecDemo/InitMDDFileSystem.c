/*****************************************************************************
* FileName:         InitMDDFileSystem.c
* Includes:
*   void InitMDDFileSystem(void)
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
* Author        Date       Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MWM           2-Aug-2011 Created for PIC32MX1xx/2xx Starter Kit Demo app
*******************************************************************************/
#include "HardwareProfile.h"
#include "AssertToDisplay.h"
#include "FSIO.h"
#include "peripheral\pps.h"
#include "Graphics.h"
#include "SwitchCode.h"
#include DISPLAY_FONT_HEADER

#define WAIT_UNTIL_FINISH(x)    while(!x)

#include "Wavefile.h"
// For MDD File System operations
// From FSIO.c
extern DISK gDiskData;
extern BYTE gDataBuffer[MEDIA_SECTOR_SIZE];

void InitMDDFileSystem(void)
{
	enum SWITCHST8S Switch1St8;
    UINT16 LabelHeight;
    // For MDD File System operations
    MEDIA_INFORMATION   *mediaInformation;
    BYTE error;

    PPSInput(3,SDI2,RPA4); // SDI2 on RPA4
    SPIIN = INPUT;         // Configure RPA4 as input

    PPSOutput(2,RPA8,SDO2);// SDO2 on RPA8
    SPIOUT = OUTPUT;       // RPA8 is output

    ANSELA = ANSELB = ANSELC = 0; // Disable all ADC inputs

    SD_CS_TRIS = OUTPUT;   // ChipSelectBar manually controlled, is output

#  if( SD_CD != 0 )
    SD_CD_TRIS = INPUT;            //Card Detect - input
#  endif
#  if( SD_WE != 0 )
    SD_WE_TRIS = INPUT;            //Write Protect - input
#  endif

    while (SD_CD);

    gDiskData.buffer = gDataBuffer;
    gDiskData.mount = FALSE;

    mediaInformation = MDD_SDSPI_MediaInitialize();
  //assert(mediaInformation->errorCode == MEDIA_NO_ERROR); //SDD Card Found?
    if ( mediaInformation->errorCode != MEDIA_NO_ERROR )
    {
        SetColor(BLACK);
        WAIT_UNTIL_FINISH(Bar(0,0,GetMaxX(),GetMaxY()));

        SetColor(BRIGHTRED);
        SetFont((void *) &DISPLAY_FONT);
        LabelHeight =  GetTextHeight((void *) &DISPLAY_FONT);
        OutTextXY(1,(GetMaxY()-LabelHeight)/2,"Insert memory card and start over!");
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


    if (mediaInformation->validityFlags.bits.sectorSize)
    {
        gDiskData.sectorSize = mediaInformation->sectorSize;
        assert(mediaInformation->sectorSize <= MEDIA_SECTOR_SIZE);
    }

    // Load the Master Boot Record (partition)
    if((error = LoadMBR(&gDiskData)) == CE_GOOD)
    {
        // Now the boot sector
        if((error = LoadBootSector(&gDiskData)) == CE_GOOD)
            gDiskData.mount = TRUE;
    }
    assert(error==CE_GOOD);
}

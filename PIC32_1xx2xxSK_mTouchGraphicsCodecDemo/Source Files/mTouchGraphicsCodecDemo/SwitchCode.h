/*****************************************************************************
* FileName:         SwitchCode.h
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
* Author       Date        Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MWM          21 Jul 2011  Moved from main file to this file for simplicity
*******************************************************************************/
#if !defined( _SWITCH_CODE_H_ )
#define       _SWITCH_CODE_H_

#define SWITCH_DEBOUNCE_COUNT 100

#define PORT_BIT_15 (0x8000)
#define PORT_BIT_14 (0x4000)
#define PORT_BIT_13 (0x2000)
#define PORT_BIT_12 (0x1000)
#define PORT_BIT_11 (0x0800)
#define PORT_BIT_10 (0x0400)
#define PORT_BIT_09 (0x0200)
#define PORT_BIT_08 (0x0100)
#define PORT_BIT_07 (0x0080)
#define PORT_BIT_06 (0x0040)
#define PORT_BIT_05 (0x0020)
#define PORT_BIT_04 (0x0010)
#define PORT_BIT_03 (0x0008)
#define PORT_BIT_02 (0x0004)
#define PORT_BIT_01 (0x0002)
#define PORT_BIT_00 (0x0001)

enum SWITCHST8S { OFF = 0, ON };

void ResetSwitches(void);
enum SWITCHST8S CheckSwitch1(void);
enum SWITCHST8S CheckSwitch2(void);
enum SWITCHST8S CheckSwitch3(void);
enum SWITCHST8S UpdateSwitch( INT16 iSwitch, enum SWITCHST8S SwitchSt8, char *SwitchLabel );

#endif//!defined( _SWITCH_CODE_H_ )

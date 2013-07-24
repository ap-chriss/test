
/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  PMP driver
 *****************************************************************************
 * FileName:        pmp.c
 * Dependencies:    Graphics.h
 * Processor:       PIC24, PIC32
 * Compiler:        MPLAB C30, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2010 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Anton Alkhimenok     01/12/10
 *****************************************************************************/
#ifndef _GFX_PMP_H_FILE
#define _GFX_PMP_H_FILE

#include "Graphics.h"
#include "Compiler.h"

#ifdef USE_GFX_PMP

/*********************************************************************
 * Graphic Controller PMP wait states
 *
 * The setup time required when running the SSD1926 at 80MHz is
 * 50ns or (20 MHz).
 *
 * To calculate the number of required wait states, we will use the
 * following equation.
 *
 * wait states = Peripheral Clock (Hz) / setup time (Hz)
 *
 * For example if you peripheral clock is running at 80 MHz, the
 * wait states required:
 * wait states = 80,000,000 / 20,000,000
 * wait states = 4
 *********************************************************************/
#if (DISPLAY_CONTROLLER == LGDP4531)
#define DISPLAY_CONTROLLER_SETUP_Hz    (20000000)
#endif
#if (DISPLAY_CONTROLLER == SSD1926)
#define DISPLAY_CONTROLLER_SETUP_Hz    (20000000)
#endif

#define PMP_WAIT_STATES     (GetPeripheralClock() / DISPLAY_CONTROLLER_SETUP_Hz)
#ifdef __PIC32MX__
#define PMDIN1              PMDIN
#endif
/*********************************************************************
* Macros:  PMPWaitBusy()
*
* Overview: waits for PMP cycle end.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#define PMPWaitBusy()   while(PMMODEbits.BUSY);

/*********************************************************************
* Macros:  DeviceWrite(data)
*
* PreCondition:  none
*
* Input: data - value to be written to RAM
*
* Output: none
*
* Side Effects: none
*
* Overview: writes data into controller's RAM
*
* Note: chip select should be enabled
*
********************************************************************/
#ifdef USE_16BIT_PMP

extern inline void __attribute__ ((always_inline)) DeviceWrite(WORD data)
{
    PMDIN1 = data;
    PMPWaitBusy();
}

#else

extern inline void __attribute__ ((always_inline)) DeviceWrite(BYTE data)
{
    PMDIN1 = data;
    PMPWaitBusy();
}

#endif

/*********************************************************************
* Macros:  DeviceRead()
*
* PreCondition:  none
*
* Input: none
*
* Output: data read
*
* Side Effects: none
*
* Overview: reads data from controller's RAM
*
* Note: chip select should be enabled
*
********************************************************************/
#ifdef USE_16BIT_PMP

extern inline WORD __attribute__ ((always_inline)) DeviceRead()
{
WORD value;
    value = PMDIN1;
    PMPWaitBusy();
    PMCONbits.PMPEN = 0; // disable PMP
    value = PMDIN1;
    PMPWaitBusy();
    PMCONbits.PMPEN = 1; // enable  PMP
    return value;
}

#else


extern inline BYTE __attribute__ ((always_inline)) DeviceRead(){
BYTE value;
    value = PMDIN1;
    PMPWaitBusy();
    PMCONbits.PMPEN = 0; // disable PMP
    value = PMDIN1;
    PMCONbits.PMPEN = 1; // enable  PMP
    return value;
}

#endif


/*********************************************************************
* Macros:  SingleDeviceRead()
*
* PreCondition:  none
*
* Input: none
*
* Output: data read
*
* Side Effects: none
*
* Overview: a single read is performed. This is useful in issuing
*           one read acess only.
*
* Note: chip select should be enabled
*
********************************************************************/
#ifdef USE_8BIT_PMP
extern inline BYTE __attribute__ ((always_inline)) SingleDeviceRead()
{
BYTE value;
    value = PMDIN1;
    PMPWaitBusy();
    return value;
}

#endif

/*********************************************************************
* Function:  DeviceInit()
*
* PreCondition:  none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: initializes the device
*
* Note: none
*
********************************************************************/

extern inline void __attribute__ ((always_inline)) DeviceInit(void)
{
    DisplayResetEnable();            // hold in reset by default
    DisplayResetConfig();            // enable RESET line
    DisplayWriteConfig();            // enable Write line
    DisplayReadConfig();             // enable Read Line
    DisplayRSConfig();               // enable Data/Command Line
    DisplayDisable();                // not selected by default
    DisplayCSConfig();               // enable CS Line

    // PMP setup
    PMMODE = 0;
//  PMAEN = 0;
    PMCON = 0;

    PMMODEbits.MODE = 2;                    // Intel 80 master interface

    #if defined(__PIC32MX__)
     #if defined( __32MX220F032D__ )
     PMMODEbits.WAITB = 0;
     PMMODEbits.WAITM = 2;//PMP_WAIT_STATES;
     #else
     PMMODEbits.WAITB = 1;//0;
     PMMODEbits.WAITM = 4;//PMP_WAIT_STATES;
     #endif
    #elif defined(__dsPIC33F__) || defined(__PIC24H__)
    PMMODEbits.WAITB = 1;
    PMMODEbits.WAITM = 3;
    #else
    PMMODEbits.WAITB = 0;
    PMMODEbits.WAITM = 2;
    #endif
    PMMODEbits.WAITE = 1;//0;

    #if defined( __32MX220F032D__ )
        #ifdef USE_16BIT_PMP
            #error "16 bit wide PMP not supported by this device."
        #endif
    #else
        #ifdef USE_16BIT_PMP
            PMMODEbits.MODE16 = 1;          // 16 bit mode
        #else
            PMMODEbits.MODE16 = 0;          // 8 bit mode
        #endif
    #endif//defined( __32MX220F032D__ )

    PMCONbits.PTRDEN = 1;                   // enable RD line
    PMCONbits.PTWREN = 1;                   // enable WR line
    PMCONbits.PMPEN = 1;                    // enable PMP

    DelayMs(40);
    DisplayResetDisable();                         // release from reset
    DelayMs(400);
}
#endif
#endif

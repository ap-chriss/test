/********************************************************************
* FileName:     HardwareProfile.h
* Dependencies: See INCLUDES section
* Processor:    PIC32MX
* Hardware:
* Complier:     Microchip C32
* Company:       Microchip Technology, Inc.
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
********************************************************************
 File Description:

 Change History:
  Rev   Date         Description
  1.0   11/19/2004   Initial release
  2.1   02/26/2007   Updated for simplicity and to use common
                     coding style
  2.2    04/01/08    cleaned up hardware profile selection for PKS24
--------------------------------------------------------------------
NMS/NK      10-Feb-2009 Folder/Files restructuring
NK          24-Apr-2009 Porting for 18F46J50 Eval Board
MC          22-Ian-2010 Porting for PIC32MX795F512H
MWM         38 Mar 2011 Added defines for all buttons/sliders so that
                        this file can be used for all hardware configs
********************************************************************/

#ifndef __HARDWARE_PROFILE_H
#define __HARDWARE_PROFILE_H

#include "GenericTypeDefs.h"
#include "mTouchConfig.h"
//#include "mTouchCap_PIC32MX_CVD_Physical.h"
#include "mTouchCapPhy.h"

/******************************************************************************
    HARDWARE PROFILE
******************************************************************************/
#define PIC32MX220_STARTER_KIT_BOARD
#define MAX_ADC_CHANNELS 13

/******************************************************************************
    SYSTEM CLOCK
******************************************************************************/
// Clock speed for PIC32
#if defined (__PIC32MX__)
    #define GetSystemClock()       40000000UL
    #define SYS_FREQ              (40000000UL)
    #define GetPeripheralClock()  (GetSystemClock())
    #define GetInstructionClock() (GetSystemClock())
#endif

// Clock values
#define MILLISECONDS_PER_TICK  10                // Definition for use with a tick timer
#define TIMER_PRESCALER        TIMER_PRESCALER_8 // Definition for use with a tick timer
#define TIMER_PERIOD           37500             // Definition for use with a tick timer


/******************************************************************************
    MDD SPI Interface Setup
******************************************************************************/
// Description: Macro used to enable the SD-SPI physical layer (SD-SPI.c and .h)
#define USE_SD_INTERFACE_WITH_SPI

/* SD Card definitions: Change these to fit your application when using
   an SD-card-based physical layer                                   */

#if defined( USE_SD_INTERFACE_WITH_SPI )
    // Registers for the SPI module you want to use
    //#define MDD_USE_SPI_1
    #define MDD_USE_SPI_2

    //SPI Configuration
    #define SPI_START_CFG_1     (PRI_PRESCAL_64_1 | SEC_PRESCAL_8_1 | MASTER_ENABLE_ON | SPI_CKE_ON | SPI_SMP_ON)
    #define SPI_START_CFG_2     (SPI_ENABLE)

    // Define the SPI frequency
    #define SPI_FREQUENCY           (20000000)

    #if defined MDD_USE_SPI_1
        // Description: SD-SPI Chip Select Output bit
        //#define SD_CS_ADDR_BIT      4 // PMA4 -> RC4 on part
        #if   defined(SD_CS_ADDR_BIT)
            #define SD_CS_LO PMADDRCLR=1<<SD_CS_ADDR_BIT
            #define SC_CS_HI PMADDRSET=1<<SD_CS_ADDR_BIT
        #else
            #define SD_CS_LO   LATBbits.LATB1=0
            #define SD_CS_HI   LATBbits.LATB1=1
        #endif//defined(SD_CS_ADDR_BIT)
        #define SD_CS_TRIS TRISBbits.TRISB1

        // Description: SD-SPI Card Detect Input bit
        #define SD_CD               PORTFbits.RF0
        // Description: SD-SPI Card Detect TRIS bit
        #define SD_CD_TRIS          TRISFbits.TRISF0

        // Description: SD-SPI Write Protect Check Input bit
        #define SD_WE               PORTFbits.RF1
        // Description: SD-SPI Write Protect Check TRIS bit
        #define SD_WE_TRIS          TRISFbits.TRISF1

        // Description: The main SPI control register
        #define SPICON1             SPI1CON
        // Description: The SPI status register
        #define SPISTAT             SPI1STAT
        // Description: The SPI Buffer
        #define SPIBUF              SPI1BUF
        // Description: The receive buffer full bit in the SPI status register
        #define SPISTAT_RBF         SPI1STATbits.SPIRBF
        // Description: The bitwise define for the SPI control register (i.e. _____bits)
        #define SPICON1bits         SPI1CONbits
        // Description: The bitwise define for the SPI status register (i.e. _____bits)
        #define SPISTATbits         SPI1STATbits
        // Description: The enable bit for the SPI module
        #define SPIENABLE           SPICON1bits.ON
        // Description: The definition for the SPI baud rate generator register (PIC32)
        #define SPIBRG              SPI1BRG

        // Tris pins for SCK/SDI/SDO lines
        #if   defined(__32MX460F512L__)
            // Description: The TRIS bit for the SCK pin
            #define SPICLOCK            TRISDbits.TRISD10
            // Description: The TRIS bit for the SDI pin
            #define SPIIN               TRISCbits.TRISC4
            // Description: The TRIS bit for the SDO pin
            #define SPIOUT              TRISDbits.TRISD0
        #elif defined( __32MX360F512L__ )
            // Description: The TRIS bit for the SCK pin
            #define SPICLOCK            TRISFbits.TRISF6
            // Description: The TRIS bit for the SDI pin
            #define SPIIN               TRISFbits.TRISF7
            // Description: The TRIS bit for the SDO pin
            #define SPIOUT              TRISFbits.TRISF8
        #endif

            //SPI library functions
        #define putcSPI             putcSPI1
        #define getcSPI             getcSPI1
        #define OpenSPI(config1, config2)   OpenSPI1(config1, config2)

    #elif defined MDD_USE_SPI_2
        // Description: SD-SPI Chip Select Output bit
        #define SD_CS_ADDR_BIT      4 // PMA4 -> RC4 on part
        #if   defined(SD_CS_ADDR_BIT)
            #define SD_CS_LO PMADDRCLR=1<<SD_CS_ADDR_BIT
            #define SD_CS_HI PMADDRSET=1<<SD_CS_ADDR_BIT
        #else
            #define SD_CS_LO   LATCbits.LATC4=0
            #define SD_CS_HI   LATCbits.LATC4=1
        #endif//defined(SD_CS_ADDR_BIT)
        #define SD_CS_TRIS TRISCbits.TRISC4

        // Description: SD-SPI Card Detect Input bit
        #define SD_CD               0 // There is no Card Detect Bar on the board.
#      if( SD_CD != 0 )
        // Description: SD-SPI Card Detect TRIS bit
        #define SD_CD_TRIS          TRISBbits.TRISB14
#      endif

        // Description: SD-SPI Write Protect Check Input bit
        #define SD_WE               0 // There is no Write Enable Bar on the board, we don't do writes anyway
#      if ( SD_WE != 0 )
        // Description: SD-SPI Write Protect Check TRIS bit
        #define SD_WE_TRIS          TRISBbits.TRISB13
#      endif

        // Description: The main SPI control register
        #define SPICON1             SPI2CON
        // Description: The SPI status register
        #define SPISTAT             SPI2STAT
        // Description: The SPI Buffer
        #define SPIBUF              SPI2BUF
        // Description: The receive buffer full bit in the SPI status register
        #define SPISTAT_RBF         SPI2STATbits.SPIRBF
        // Description: The bitwise define for the SPI control register (i.e. _____bits)
        #define SPICON1bits         SPI2CONbits
        // Description: The bitwise define for the SPI status register (i.e. _____bits)
        #define SPISTATbits         SPI2STATbits
        // Description: The enable bit for the SPI module
        #define SPIENABLE           SPI2CONbits.ON
        // Description: The definition for the SPI baud rate generator register (PIC32)
        #define SPIBRG              SPI2BRG

        // Tris pins for SCK/SDI/SDO lines

        // Description: The TRIS bit for the SCK pin
        #define SPICLOCK            TRISBbits.TRISB15
        // Description: The TRIS bit for the SDI pin
        #define SPIIN               TRISAbits.TRISA4
        // Description: The TRIS bit for the SDO pin
        #define SPIOUT              TRISAbits.TRISA8

        //SPI library functions
        #define putcSPI             putcSPI2
        #define getcSPI             getcSPI2
        #define OpenSPI(config1, config2)   OpenSPI2(config1, config2)
    #endif
    // Will generate an error if the clock speed is too low to interface to the card
    #if (GetSystemClock() < 100000)
        #error Clock speed must exceed 100 kHz
    #endif

#endif//defined( USE_SD_INTERFACE_WITH_SPI )


/******************************************************************************
    Pin Configuration Values
******************************************************************************/
#define     TRISA_SET_VALUE        (1<<7) | (1<<9) //Switch 1 on A9, Switch 3 on RA7

#if !defined( DUMP_DIAGNOSTICS_VIA_UART )
#   define     TRISB_SET_VALUE     1<<4 //Switch 2 on RB4
#else
#   define     INIT_TRISB_VALUE     0x0000
#endif// !defined( DUMP_DIAGNOSTICS_VIA_UART )

#define     ITRISC_SET_VALUE        0x0000


/******************************************************************************
    CHANNEL ASSIGNMENT
    Set the appropriate ADC Channel number to each Direct Key
******************************************************************************/
#define DIRECTKEY1_CHANNEL    CHANNEL_AN6
#define DIRECTKEY2_CHANNEL    CHANNEL_AN7
#define DIRECTKEY3_CHANNEL    CHANNEL_AN8
#define DIRECTKEY4_CHANNEL    CHANNEL_AN12



#endif  // __HARDWARE_PROFILE_H

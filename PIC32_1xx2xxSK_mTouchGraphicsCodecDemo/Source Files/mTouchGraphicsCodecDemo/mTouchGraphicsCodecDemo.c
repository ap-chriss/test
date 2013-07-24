/*****************************************************************************
* FileName:         mTouchGraphicsCodecDemo.c
* Includes:
*   int main(void)
*   void mTouchCapApp_PortSetup(void)
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

//adding a comment here to test out git

//What music will the app play??
#include "MusicDefines.h"

#include "GenericTypeDefs.h"

#include "mTouchConfig.h"
#include "HardwareProfile.h"
#include "AssertToDisplay.h"
#include "mtouchCapAPI.h"
#include "mTouchCapApp_DirectKeys.h"
#include "mTouchCapStatus.h"

#include "InitDisplay.h"
#include "InitMDDFileSystem.h"

#include <string.h>
#include "AssertToDisplay.h"
#include "Graphics.h"
#include DISPLAY_FONT_HEADER

#include "FSIO.h"
#include "dsplib_dsp.h"

#include "Wavefile.h"
#include "SwitchCode.h"
#include "UpdateSliderOrButton.h"
#include "WM8731Drv.h"

volatile BOOL IgnoreCurrentDataFlag = FALSE; // Flag set by application to signal when mTouch

// Device setup
#pragma config FNOSC    = PRIPLL   // Oscillator Selection: Primary oscillator (XT, HS, EC) w/ PL
#pragma config FPLLIDIV = DIV_2    // PLL Input Divider: Divide by 2
#pragma config FPLLMUL  = MUL_20   // PLL Multiplier: Multiply by 20
#pragma config FPLLODIV = DIV_2    // PLL Output Divider: Divide by 2
#pragma config POSCMOD  = HS       // Primary Oscillator: HS oscillator
#pragma config FCKSM    = CSDCMD   // Clock Switching & Fail Safe Monitor: Clock Switching Disabled, Clock Monitoring Disabled
#pragma config OSCIOFNC = OFF      // CLKO Enable: Disabled
#pragma config IESO     = OFF      // Internal/External Switch-over: Disabled
#pragma config FSOSCEN  = OFF      // Secondary Oscillator Enable: Disabled

#pragma config FPBDIV   = DIV_1    // Peripheral Clock divisor: Divide by 1

#pragma config UPLLEN   = OFF      // USB PLL Disabled
#pragma config UPLLIDIV = DIV_2    // USB PLL Input Divider

#pragma config FWDTEN   = OFF     // Watchdog Timer: Disabled
#pragma config WDTPS    = PS1      // Watchdog Timer Postscale: 1:1

#pragma config CP       = OFF      // Code Protect: OFF
#pragma config BWP      = OFF      // Boot Flash Write Protect: OFF
#pragma config PWP      = OFF      // Program Flash Write Protect: OFF

#pragma config ICESEL   = ICS_PGx2 // ICE/ICD Comm Channel Select: ICE pins shared with PGC1,PGD1
#pragma config DEBUG    = OFF      // Background Debugger Enable: OFF

/* NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE

    Be sure the clock setups, as defined by above, are compatible with the
    clock definitions macros ("GetSystemClock()","GetPeripheralClock()", and
    "GetInstructionClock()") in HardwareProfile.h and the ADC clock setup in
    "InitADC2" found in mTouchCap_ADC.c .

   NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE */



// Audio Codec data, from WM8731.c
extern STEREO_AUDIO_DATA StereoDataIn[SRC_IN_PROC_SIZE];
extern STEREO_AUDIO_DATA DataIn[SRC_IN_PROC_SIZE/2]; // Buffer for MDD file reads
extern STEREO_AUDIO_DATA *pBufferStart[2];
extern volatile BOOL BufferEmpty[2];

static UINT16 nPeriod = 44;
static  INT16 SampleVal[44] __attribute__((aligned(4)))
                                            = { 30000*( 0.000000000000000),
                                                30000*( 0.142314838273285),
                                                30000*( 0.281732556841430),
                                                30000*( 0.415415013001886),
                                                30000*( 0.540640817455598),
                                                30000*( 0.654860733945285),
                                                30000*( 0.755749574354258),
                                                30000*( 0.841253532831181),
                                                30000*( 0.909631995354518),
                                                30000*( 0.959492973614497),
                                                30000*( 0.989821441880933),
                                                30000*( 1.000000000000000),
                                                30000*( 0.989821441880933),
                                                30000*( 0.959492973614497),
                                                30000*( 0.909631995354518),
                                                30000*( 0.841253532831181),
                                                30000*( 0.755749574354258),
                                                30000*( 0.654860733945285),
                                                30000*( 0.540640817455598),
                                                30000*( 0.415415013001886),
                                                30000*( 0.281732556841430),
                                                30000*( 0.142314838273285),
                                                30000*( 0.000000000000000),
                                                30000*(-0.142314838273285),
                                                30000*(-0.281732556841429),
                                                30000*(-0.415415013001886),
                                                30000*(-0.540640817455598),
                                                30000*(-0.654860733945285),
                                                30000*(-0.755749574354258),
                                                30000*(-0.841253532831181),
                                                30000*(-0.909631995354518),
                                                30000*(-0.959492973614497),
                                                30000*(-0.989821441880933),
                                                30000*(-1.000000000000000),
                                                30000*(-0.989821441880933),
                                                30000*(-0.959492973614497),
                                                30000*(-0.909631995354519),
                                                30000*(-0.841253532831181),
                                                30000*(-0.755749574354258),
                                                30000*(-0.654860733945285),
                                                30000*(-0.540640817455597),
                                                30000*(-0.415415013001886),
                                                30000*(-0.281732556841430),
                                                30000*(-0.142314838273285)
                                               };

static  INT16 ScaledVal[44] __attribute__((aligned(4))) ;
static UINT16 iSample = 0;




//#define TWELVEMEG

#ifdef TWELVEMEG

#	define RODIV	4
#	define REFTRIM	0

#else

#	define RODIV	4						
#	define REFTRIM	0x40800000	//11.2891MHz

#endif

// some local data
volatile int		DmaTxIntFlag;			// flag used in interrupts, signal that DMA transfer ended
volatile int		DmaRxIntFlag;			// flag used in interrupts, signal that DMA transfer ended



int main(void)
{


    DDPCONbits.JTAGEN = 0; // Disable JTAG

    // Initalize global interrupt enable
    INTEnableSystemMultiVectoredInt();

    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

	PPSOutput(3, RPC3, REFCLKO);	//REFCLK0: RPC3 - Out


	REFOTRIM = REFTRIM;	
	OSCREFConfig(OSC_REFOCON_USBPLL, 				//USB-PLL clock output used as REFCLKO source
				 OSC_REFOCON_OE | OSC_REFOCON_ON, 	//Enable and turn on the REFCLKO
				 RODIV);


	Nop(); //added for git test
   // Setup CODEC interface after setting up display.
    
    LRCLK_PPS_SETUP;
    LRCLK_TRIS_SETUP;
    DAC_DATA_PPS_SETUP;
    DAC_DATA_TRIS_SETUP;


    // Setup I2S on SPI1
    I2SCON = I2SCON2 = 0;   // Reset settings
    I2SSTATBITS.SPIROV = 1; // Clear overflow, if set
    I2SSTAT = 0;




    IFS0bits.CS0IF = 0; // Clear interrupt flag for software
    IPC0bits.CS0IP = 3; // FileReadISR priority < I2S priority
    IPC0bits.CS0IS = 0; // Secondary priority = 0;

#define SLAVE_CODEC
#ifdef SLAVE_CODEC
//	REFOCON = 0x00008006;	//set refclk on and source to USBPLL

    I2SBRG  = 0x0d;                    // Set baud rate, not needed for slave
    
    I2SCON  = SPI_CONFIG_CKP_HIGH |    // Invert input SCK polarity
    		  SPI_CONFIG_FRMEN	  |
    		  SPI_CONFIG_MSSEN	  |
        	  SPI_CONFIG_MSTEN	  |	   // Master mode enabled	
              SPI_CONFIG_ENHBUF   ;    // Enhanced buffer enabled
              
    I2SCON  = 0x94030078;
    
    I2SCONBITS.STXISEL = 0x2;          // Interrupt when half empty
    I2SCON2 = SPI_CONFIG2_AUDMOD_I2S | // I2S mode
              SPI_CONFIG2_IGNROV     | // Ignore receive overflow
              SPI_CONFIG2_IGNTUR     | // Ignore transmit underrun
              SPI_CONFIG2_AUDEN      ; // Enable Audio Codec Support
#else
    I2SBRG  = 0x0d;                    // Set baud rate
    I2SCON  = SPI_CONFIG_CKP_HIGH |    // Invert input SCK polarity
              SPI_CONFIG_ENHBUF   ;    // Enhanced buffer enabled
    I2SCONBITS.STXISEL = 0x1;          // Interrupt when half empty
    I2SCON2 = SPI_CONFIG2_AUDMOD_I2S | // I2S mode
              SPI_CONFIG2_IGNROV     | // Ignore receive overflow
              SPI_CONFIG2_IGNTUR     | // Ignore transmit underrun
              SPI_CONFIG2_AUDEN      ; // Enable Audio Codec Support
#endif

    
    
    
    CMDInit(); // Setup command interface to configure CODEC
    Delay10us(1);
    WM8731CodecLoadStartup(0); // Configure the CODEC
    WM8731CodecSetVolume( 0, 0x70 );
    CMDDeinit();


    // LEDs on the board
    PMAENSET  = 1<<5 | 1<<10;
    PMADDRCLR = 1<<5 | 1<<10;
    
	int			ix;

	// open and configure the DMA channel.
	DmaChnOpen(DMA_CHANNEL0, DMA_CHN_PRI2, DMA_OPEN_DEFAULT);

	// set the events: we want the SPI transmit buffer empty interrupt to start our transfer
	DmaChnSetEventControl(DMA_CHANNEL0, DMA_EV_START_IRQ_EN|DMA_EV_START_IRQ(_SPI1_TX_IRQ));

	// set the transfer:
	// source is our buffer, dest is the SPI transmit buffer
	// source size is the whole buffer, destination size is one byte
	// cell size is one byte: we want one byte to be sent per each SPI TXBE event
	DmaChnSetTxfer(DMA_CHANNEL0, SampleVal, (void*)&SPI1BUF, sizeof(SampleVal), 1, 1);

	DmaChnSetEvEnableFlags(DMA_CHANNEL0, DMA_EV_BLOCK_DONE);	// enable the transfer done interrupt, when all buffer transferred

	INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
	INTEnableInterrupts();

	INTSetVectorPriority(INT_VECTOR_DMA(DMA_CHANNEL0), INT_PRIORITY_LEVEL_5);		// set INT controller priority
	INTSetVectorSubPriority(INT_VECTOR_DMA(DMA_CHANNEL0), INT_SUB_PRIORITY_LEVEL_3);		// set INT controller sub-priority

	INTEnable(INT_SOURCE_DMA(DMA_CHANNEL0), INT_ENABLED);		// enable the chn interrupt in the INT controller

	DmaTxIntFlag=0;			// clear the interrupt flag we're  waiting on

	DmaChnStartTxfer(DMA_CHANNEL0, DMA_WAIT_NOT, 0);	// force the DMA transfer: the SPI TBE flag it's already been active

	// wait for the transfer to complete
	// In a real application you can do some other stuff while the DMA transfer is taking place
	while(!DmaTxIntFlag);

    while(1);

}//end main()


// handler for the DMA channel 1 interrupt
void __ISR(_DMA0_VECTOR, IPL5SOFT) DmaHandler1(void)
{
	int	evFlags;				// event flags when getting the interrupt

	INTClearFlag(INT_SOURCE_DMA(DMA_CHANNEL0));	// acknowledge the INT controller, we're servicing int

	evFlags=DmaChnGetEvFlags(DMA_CHANNEL0);	// get the event flags

    if(evFlags&DMA_EV_BLOCK_DONE)
    { // just a sanity check. we enabled just the DMA_EV_BLOCK_DONE transfer done interrupt
    	DmaTxIntFlag=1;
        DmaChnClrEvFlags(DMA_CHANNEL0, DMA_EV_BLOCK_DONE);
    }
}





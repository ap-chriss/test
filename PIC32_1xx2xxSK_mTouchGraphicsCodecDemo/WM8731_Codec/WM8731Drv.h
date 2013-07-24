/**********************************************************************
* © 2010 Microchip Technology Inc.
*
* FileName:        WM8731Drv.h
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       PIC32MX
* Compiler:        MPLAB® C32
*
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

************************************************************************/

#ifndef _WM8731_DRV_H_
#define _WM8731_DRV_H_

#include <p32xxxx.h>
#include <plib.h>
#include <GenericTypeDefs.h>
#include <sys/kmem.h>
#include "FSIO.h"


//#define SLAVE_CODEC

#define SRC_IN_PROC_SIZE    512 // Number of stereo samples in buffer

/*****************************************************************************
  This specifies the music module connected to the WM8731 Digital Audio
  Interface. In custom hardware this can be specified to any valid device SPI
  module such as SPI2A, SPI3A etc.  To use an I2S interface instead of a
  straight SPI, define I2S_MODULE instead.
 *****************************************************************************/
//#define WM8731DRV_SPI_MODULE     SPI1
#define WM8731DRV_I2S_MODULE     SPI1

#ifdef SLAVE_CODEC
	#define LRCLK_PPS_SETUP PPSOutput(1,RPC7,SS1)   //Left Right Clock to CODEC
	#define LRCLK_TRIS_SETUP TRISCbits.TRISC7=0
#else
	// These PPS setups are required for 1xx/2xx parts
	#define LRCLK_PPS_SETUP PPSInput(1,SS1,RPC7)   //Left Right Clock from CODEC
	#define LRCLK_TRIS_SETUP TRISCbits.TRISC7=1
#endif

	#define DAC_DATA_PPS_SETUP  PPSOutput(3,RPC6,SDO1) //DAC data line to CODEC
	#define DAC_DATA_TRIS_SETUP TRISCbits.TRISC6=0

/*****************************************************************************
  This specifies the command module connected to the WM8731 Control Interface.
  In custom hardware this can be specified to any valid device I2C module such
  as I2C1.  To use a 3-wire command interface instead of a 2-wire, Define
  CMD_MODULE instead.
 *****************************************************************************/
//#define WM8731DRV_I2C_MODULE     I2C2
#define WM8731DRV_CMD_MODULE      SPI2
#define WM8731DRV_CMD_SPI_CHANNEL SPI_CHANNEL2


/*****************************************************************************
  This specifies the two DMA channels which the driver will use to implement
  ping pong buffering. Channels should be consecutive with PING channel being
  the lower of the two. For example channel 0 is ping and channel 1 is pong or
  channel 2 is ping and channel 3 is pong.
 *****************************************************************************/
#define WM8731DRV_TX_DMA_PING_CHANNEL DCH0
#define WM8731DRV_TX_DMA_PONG_CHANNEL DCH1


/*****************************************************************************
  This specifies the DMA channel which will receive data from the SPI module
  specified by WM8731DRV_SPI_MODULE or WM8731DRV_I2S_MODULE and store it codec
  driver buffer. In custom hardware this can be specified to be any valid device
  DMA channel such as DCH0, DCH3, DCH4 etc. It should be different from
  WM8731DRV_TX_DMA_CHANNEL.
 *****************************************************************************/
#define WM8731DRV_RX_DMA_CHANNEL DCH2


/*****************************************************************************
  These macros specify the IRQ source for WM8731DRV_TX_DMA_CHANNEL. It should
  match the TX IRQ of the SPI module specified in WM8731DRV_SPI_MODULE. Specify
  the interrupt vector, interrupt flag and interrupt enable for the ping/pong
  DMA channels.
 ****************************************************************************/
#if   defined( WM8731DRV_SPI_MODULE )

#   define WM8731DRV_DMA_TX_IRQ                     _SPI1_TX_IRQ

#   define WM8731DRV_TX_PONG_DMA_VECTOR             _DMA_1_VECTOR
#   define WM8731DRV_TX_PONG_DMA_IPL                ipl5
#   define WM8731DRV_TX_PONG_DMA_INT_FLAG           IFS1bits.DMA1IF
#   define WM8731DRV_TX_PONG_DMA_INT_ENABLE         IEC1bits.DMA1IE
#   define WM8731DRV_TX_PONG_DMA_INT_PRIORITY       IPC10bits.DMA1IP
#   define WM8731DRV_TX_PONG_DMA_INT_SUB_PRIORITY   IPC10bits.DMA1IS

#   define WM8731DRV_TX_PING_DMA_VECTOR             _DMA_0_VECTOR
#   define WM8731DRV_TX_PING_DMA_IPL                ipl5
#   define WM8731DRV_TX_PING_DMA_INT_FLAG           IFS1bits.DMA0IF
#   define WM8731DRV_TX_PING_DMA_INT_ENABLE         IEC1bits.DMA0IE
#   define WM8731DRV_TX_PING_DMA_INT_PRIORITY       IPC10bits.DMA0IP
#   define WM8731DRV_TX_PING_DMA_INT_SUB_PRIORITY   IPC10bits.DMA0IS

#elif defined( WM8731DRV_I2S_MODULE) //Placeholder - Now a duplicate of above

#   define WM8731DRV_DMA_TX_IRQ                     _SPI1_TX_IRQ
#
#   define WM8731DRV_TX_PONG_DMA_VECTOR             _DMA_1_VECTOR
#   define WM8731DRV_TX_PONG_DMA_IPL                ipl5
#   define WM8731DRV_TX_PONG_DMA_INT_FLAG           IFS1bits.DMA1IF
#   define WM8731DRV_TX_PONG_DMA_INT_ENABLE         IEC1bits.DMA1IE
#   define WM8731DRV_TX_PONG_DMA_INT_PRIORITY       IPC10bits.DMA1IP
#   define WM8731DRV_TX_PONG_DMA_INT_SUB_PRIORITY   IPC10bits.DMA1IS
#
#   define WM8731DRV_TX_PING_DMA_VECTOR             _DMA_0_VECTOR
#   define WM8731DRV_TX_PING_DMA_IPL                ipl5
#   define WM8731DRV_TX_PING_DMA_INT_FLAG           IFS1bits.DMA0IF
#   define WM8731DRV_TX_PING_DMA_INT_ENABLE         IEC1bits.DMA0IE
#   define WM8731DRV_TX_PING_DMA_INT_PRIORITY       IPC10bits.DMA0IP
#   define WM8731DRV_TX_PING_DMA_INT_SUB_PRIORITY   IPC10bits.DMA0IS

// For use with Non-DMA operation of I2S module
#   define WM8731DRV_TX_XFER_DONE_VECTOR            _SPI_1_VECTOR
#   define WM8731DRV_TX_XFER_DONE_IPL               IPL4SOFT
#   define WM8731DRV_TX_XFER_DONE_INT_FLAG          IFS1bits.SPI1TXIF
#   define WM8731DRV_TX_XFER_DONE_INT_ENABLE        IEC1bits.SPI1TXIE
#   define WM8731DRV_TX_XFER_DONE_INT_PRIORITY      IPC7bits.SPI1IP
#   define WM8731DRV_TX_XFER_DONE_INT_SUB_PRIORITY  IPC7bits.SPI1IS


#else
#   error "Music interface not defined"
#endif//defined( WM8731DRV_SPI_MODULE )


/*****************************************************************************
  This macro specifies the IRQ source for WM8731DRV_RX_DMA_CHANNEL. It should
  match the RX IRQ of the SPI module specified in WM8731DRV_SPI_MODULE.
 *****************************************************************************/
#if   defined( WM8731DRV_SPI_MODULE )
#   define WM8731DRV_DMA_SPI_RX_IRQ _SPI1_RX_IRQ
#elif defined( WM8731DRV_I2S_MODULE)
#   define WM8731DRV_DMA_SPI_RX_IRQ _SPI1_RX_IRQ
#else
#   error "Music interface not defined"
#endif//defined( WM8731DRV_SPI_MODULE )


/*****************************************************************************
  Defines the playback driver buffer size. The size is specified in stereo
  samples. Each stereo sample is made up of two 16 bit words. Increasing the
  buffer sizing increases the availability of the buffers for input and output.
  But this also requires more memory space. Make this 0 if playback is not
  required.
 *****************************************************************************/
#define WM8731DRV_TX_BUFFER_SIZE    (1152 * 2)


/*****************************************************************************
  Defines the record driver buffer size. The size is specified in stereo
  samples. Each stereo sample is made up of two 16 bit words. Increasing the
  buffer sizing increases the availability of the buffers for input and output.
  But this also requires more memory space. Make this 0 if record is not
  required.
 *****************************************************************************/
#define WM8731DRV_RX_BUFFER_SIZE    0


/*****************************************************************************
  Specify the clockspeed at which the PIC32 connected to the codec works.
 *****************************************************************************/
#define WM8731DRV_SYSTEM_CLOCK 40000000


/*****************************************************************************
  Specify the I2C address of codec. This is defined in hardware by the CSB pin
  of the codec. Refer to WM8731 datasheet for more details
 *****************************************************************************/
#if defined ( WM8731DRV_I2C_MODULE )
#   define WM8731DRV_I2C_ADDR 0x34
#endif//defined ( WM8731DRV_I2C_MODULE )

/*****************************************************************************
 *****************************************************************************
                     DO NOT CHANGE ANYTHING BELOW THIS LINE!
 *****************************************************************************
 *****************************************************************************/

/*****************************************************************************
  Refer to Kernighan Ritchie for more details on the following macro
 *****************************************************************************/
#define CAT(x,y)    x ## y
#define XCAT(x,y)   CAT(x,y)

/*****************************************************************************
  These macros create the registers to use based on selection made by the user.
 *****************************************************************************/

/*****************************************************************************
  These definitions for the selected command module.
 *****************************************************************************/
#if defined( WM8731DRV_I2C_MODULE )
#   define I2CCON      XCAT(XCAT(WM8731DRV_I2C_MODULE, ),CON)
#   define I2CCONBITS  XCAT(XCAT(WM8731DRV_I2C_MODULE, ),CONbits)
#   define I2CSTAT     XCAT(XCAT(WM8731DRV_I2C_MODULE, ),STAT)
#   define I2CSTATBITS XCAT(XCAT(WM8731DRV_I2C_MODULE, ),STATbits)
#   define I2CADD      XCAT(XCAT(WM8731DRV_I2C_MODULE, ),ADD)
#   define I2CBRG      XCAT(XCAT(WM8731DRV_I2C_MODULE, ),BRG)
#   define I2CTRN      XCAT(XCAT(WM8731DRV_I2C_MODULE, ),TRN)
#   define I2CRCV      XCAT(XCAT(WM8731DRV_I2C_MODULE, ),RCV)
#endif//defined( WM8731DRV_I2C_MODULE )

#if defined( WM8731DRV_CMD_MODULE )
#   define CMDCON      XCAT(XCAT(WM8731DRV_CMD_MODULE, ),CON)
#   define CMDCONBITS  XCAT(XCAT(WM8731DRV_CMD_MODULE, ),CONbits)
#   define CMDSTAT     XCAT(XCAT(WM8731DRV_CMD_MODULE, ),STAT)
#   define CMDSTATBITS XCAT(XCAT(WM8731DRV_CMD_MODULE, ),STATbits)
#   define CMDBUF      XCAT(XCAT(WM8731DRV_CMD_MODULE, ),BUF)
#   define CMDBRG      XCAT(XCAT(WM8731DRV_CMD_MODULE, ),BRG)
#endif//defined( WM8731DRV_CMD_MODULE )

/*****************************************************************************
  These definitions for the selected music module.
 *****************************************************************************/
#if defined( WM8731DRV_SPI_MODULE )
#   define SPICON      XCAT(XCAT(WM8731DRV_SPI_MODULE, ),CON)
#   define SPICONBITS  XCAT(XCAT(WM8731DRV_SPI_MODULE, ),CONbits)
#   define SPISTAT     XCAT(XCAT(WM8731DRV_SPI_MODULE, ),STAT)
#   define SPISTATBITS XCAT(XCAT(WM8731DRV_SPI_MODULE, ),STATbits)
#   define SPIBUF      XCAT(XCAT(WM8731DRV_SPI_MODULE, ),BUF)
#   define SPIBRG      XCAT(XCAT(WM8731DRV_SPI_MODULE, ),BRG)
#endif//defined( WM8731DRV_SPI_MODULE )

#if defined( WM8731DRV_I2S_MODULE )
#   define I2SCON      XCAT(XCAT(WM8731DRV_I2S_MODULE, ),CON)
#   define I2SCONBITS  XCAT(XCAT(WM8731DRV_I2S_MODULE, ),CONbits)
#   define I2SCON2     XCAT(XCAT(WM8731DRV_I2S_MODULE, ),CON2)
#   define I2SCON2BITS XCAT(XCAT(WM8731DRV_I2S_MODULE, ),CON2bits)
#   define I2SSTAT     XCAT(XCAT(WM8731DRV_I2S_MODULE, ),STAT)
#   define I2SSTATBITS XCAT(XCAT(WM8731DRV_I2S_MODULE, ),STATbits)
#   define I2SBUF      XCAT(XCAT(WM8731DRV_I2S_MODULE, ),BUF)
#   define I2SBRG      XCAT(XCAT(WM8731DRV_I2S_MODULE, ),BRG)
#endif//defined( WM8731DRV_I2S_MODULE )


/*****************************************************************************
  These definitions are for the selected TX and RX DMA channel.
 *****************************************************************************/

#define PING_TXDCHCON        XCAT(XCAT(WM8731DRV_TX_DMA_PING_CHANNEL, ),CON)
#define PING_TXDCHCONSET     XCAT(XCAT(WM8731DRV_TX_DMA_PING_CHANNEL, ),CONSET)
#define PING_TXDCHCONBITS    XCAT(XCAT(WM8731DRV_TX_DMA_PING_CHANNEL, ),CONbits)
#define PING_TXDCHCONCLR     XCAT(XCAT(WM8731DRV_TX_DMA_PING_CHANNEL, ),CONCLR)
#define PING_TXDCHECON       XCAT(XCAT(WM8731DRV_TX_DMA_PING_CHANNEL, ),ECON)
#define PING_TXDCHECONBITS   XCAT(XCAT(WM8731DRV_TX_DMA_PING_CHANNEL, ),ECONbits)
#define PING_TXDCHINT        XCAT(XCAT(WM8731DRV_TX_DMA_PING_CHANNEL, ),INT)
#define PING_TXDCHINTBITS    XCAT(XCAT(WM8731DRV_TX_DMA_PING_CHANNEL, ),INTbits)
#define PING_TXDCHSSA        XCAT(XCAT(WM8731DRV_TX_DMA_PING_CHANNEL, ),SSA)
#define PING_TXDCHDSA        XCAT(XCAT(WM8731DRV_TX_DMA_PING_CHANNEL, ),DSA)
#define PING_TXDCHSSIZ       XCAT(XCAT(WM8731DRV_TX_DMA_PING_CHANNEL, ),SSIZ)
#define PING_TXDCHDSIZ       XCAT(XCAT(WM8731DRV_TX_DMA_PING_CHANNEL, ),DSIZ)
#define PING_TXDCHSPTR       XCAT(XCAT(WM8731DRV_TX_DMA_PING_CHANNEL, ),SPTR)
#define PING_TXDCHDPTR       XCAT(XCAT(WM8731DRV_TX_DMA_PING_CHANNEL, ),DPTR)
#define PING_TXDCHCSIZ       XCAT(XCAT(WM8731DRV_TX_DMA_PING_CHANNEL, ),CSIZ)
#define PING_TXDCHCPTR       XCAT(XCAT(WM8731DRV_TX_DMA_PING_CHANNEL, ),CPTR)

#define PONG_TXDCHCON        XCAT(XCAT(WM8731DRV_TX_DMA_PONG_CHANNEL, ),CON)
#define PONG_TXDCHCONSET     XCAT(XCAT(WM8731DRV_TX_DMA_PONG_CHANNEL, ),CONSET)
#define PONG_TXDCHCONBITS    XCAT(XCAT(WM8731DRV_TX_DMA_PONG_CHANNEL, ),CONbits)
#define PONG_TXDCHCONCLR     XCAT(XCAT(WM8731DRV_TX_DMA_PONG_CHANNEL, ),CONCLR)
#define PONG_TXDCHECON       XCAT(XCAT(WM8731DRV_TX_DMA_PONG_CHANNEL, ),ECON)
#define PONG_TXDCHECONBITS   XCAT(XCAT(WM8731DRV_TX_DMA_PONG_CHANNEL, ),ECONbits)
#define PONG_TXDCHINT        XCAT(XCAT(WM8731DRV_TX_DMA_PONG_CHANNEL, ),INT)
#define PONG_TXDCHINTBITS    XCAT(XCAT(WM8731DRV_TX_DMA_PONG_CHANNEL, ),INTbits)
#define PONG_TXDCHSSA        XCAT(XCAT(WM8731DRV_TX_DMA_PONG_CHANNEL, ),SSA)
#define PONG_TXDCHDSA        XCAT(XCAT(WM8731DRV_TX_DMA_PONG_CHANNEL, ),DSA)
#define PONG_TXDCHSSIZ       XCAT(XCAT(WM8731DRV_TX_DMA_PONG_CHANNEL, ),SSIZ)
#define PONG_TXDCHDSIZ       XCAT(XCAT(WM8731DRV_TX_DMA_PONG_CHANNEL, ),DSIZ)
#define PONG_TXDCHSPTR       XCAT(XCAT(WM8731DRV_TX_DMA_PONG_CHANNEL, ),SPTR)
#define PONG_TXDCHDPTR       XCAT(XCAT(WM8731DRV_TX_DMA_PONG_CHANNEL, ),DPTR)
#define PONG_TXDCHCSIZ       XCAT(XCAT(WM8731DRV_TX_DMA_PONG_CHANNEL, ),CSIZ)
#define PONG_TXDCHCPTR       XCAT(XCAT(WM8731DRV_TX_DMA_PONG_CHANNEL, ),CPTR)

#define RXDCHCON        XCAT(XCAT(WM8731DRV_RX_DMA_CHANNEL, ),CON)
#define RXDCHCONBITS    XCAT(XCAT(WM8731DRV_RX_DMA_CHANNEL, ),CONbits)
#define RXDCHECON       XCAT(XCAT(WM8731DRV_RX_DMA_CHANNEL, ),ECON)
#define RXDCHECONBITS   XCAT(XCAT(WM8731DRV_RX_DMA_CHANNEL, ),ECONbits)
#define RXDCHINT        XCAT(XCAT(WM8731DRV_RX_DMA_CHANNEL, ),INT)
#define RXDCHSSA        XCAT(XCAT(WM8731DRV_RX_DMA_CHANNEL, ),SSA)
#define RXDCHDSA        XCAT(XCAT(WM8731DRV_RX_DMA_CHANNEL, ),DSA)
#define RXDCHSSIZ       XCAT(XCAT(WM8731DRV_RX_DMA_CHANNEL, ),SSIZ)
#define RXDCHDSIZ       XCAT(XCAT(WM8731DRV_RX_DMA_CHANNEL, ),DSIZ)
#define RXDCHSPTR       XCAT(XCAT(WM8731DRV_RX_DMA_CHANNEL, ),SPTR)
#define RXDCHDPTR       XCAT(XCAT(WM8731DRV_RX_DMA_CHANNEL, ),DPTR)
#define RXDCHCSIZ       XCAT(XCAT(WM8731DRV_RX_DMA_CHANNEL, ),CSIZ)
#define RXDCHCPTR       XCAT(XCAT(WM8731DRV_RX_DMA_CHANNEL, ),CPTR)

/******************************************************************************
  This defines the I2C module baud rate register value. The I2C module is set to
  operate at 100kbps. Refer to the PIC32 I2C FRM section for more details
 ******************************************************************************/
#if defined( WM8731DRV_I2C_MODULE )
#   define WM8731DRV_I2C_SPEED 100000
#   define WM8731DRV_I2CBAUD ((WM8731DRV_SYSTEM_CLOCK/(2*WM8731DRV_I2C_SPEED))-2)
#endif//defined ( WM8731DRV_I2C_MODULE )

/******************************************************************************
  Defines of a stereo audio data sample
 ******************************************************************************/
typedef union {
    struct
    {
        INT16 LeftChannel;
        INT16 RightChannel;
    };
    UINT32 AudioWord;

}STEREO_AUDIO_DATA;


/*****************************************************************************
  Ping pong buffer status. Maintains data about a ping pong buffer.
 *****************************************************************************/
typedef struct
{
    STEREO_AUDIO_DATA * buffer;
    BOOL bufferIsBusy;
    UINT bufferSize;
}PING_PONG_BUFFER;

#define WM8731DRV_BUFFER_DESC PING_PONG_BUFFER


/*****************************************************************************
  Codec Data Structure. Maintains the current state of the codec.
 *****************************************************************************/
typedef struct
{
    STEREO_AUDIO_DATA * rxBuffer;
    STEREO_AUDIO_DATA * txBuffer;
    UINT16 rxBufferReadPtr;
    UINT16 txBufferReadPtr;
    UINT16 rxBufferWritePtr;
    UINT16 txBufferWritePtr;
    volatile UINT pingPongBufferFlag;
    volatile PING_PONG_BUFFER ppBuffers[2];
    INT16 gain;
}
WM8731_CODEC;

typedef union
{
    INT32 result;
    struct
    {
        UINT16 loResult;
        UINT16 hiResult;
    };
}MUL_RESULT;


/*****************************************************************************
  This enumeration details all the WM8731 registers. Refer to the WM8731 data
  sheet table 29 for more details.
 *****************************************************************************/
typedef enum
{
    WM8731_LINVOL = 0x00,
    WM8731_RINVOL = 0x01,
    WM8731_LOUT1V = 0x02,
    WM8731_ROUT1V = 0x03,
    WM8731_APANA  = 0x04,
    WM8731_APDIGI = 0x05,
    WM8731_PWR    = 0x06,
    WM8731_IFACE  = 0x07,
    WM8731_SRATE  = 0x08,
    WM8731_ACTIVE = 0x09,
    WM8731_RESET  = 0x0F

} WM8731_REGISTER;


/*****************************************************************************
  The following macros specify the default values of the codec register.
  These are the values that the codec registers will have after reset.
 *****************************************************************************/

// LEFT LINE IN - Register 0x00
typedef union {
    struct {
        unsigned LINVOL  :5; // Left channel input volume: 0x1F = +12 dB, 0x00 = -34.5 dB in 1.5 dB steps
        unsigned LINMUT  :1; // Left channel input mute to ADC: 1 = Enable Mute, 0 = Disable
        unsigned         :2;
        unsigned LRINBOTH:1; // Left to Right Channel Line Input Volume and Mute Data Load control:
                             //  1 = enable simultaneous load of left to right volume and mute
    };
    struct {
        unsigned LINVOL0:1;
        unsigned LINVOL1:1;
        unsigned LINVOL2:1;
        unsigned LINVOL3:1;
        unsigned LINVOL4:1;
    };
    struct {
        unsigned w:16;
    };
} _WM8731_LINVOLbits;

typedef enum {
    WM8731_LINVOL_LINVOL   = 0x1F,
    WM8731_LINVOL_LINVOL0  = (1<<0),
    WM8731_LINVOL_LINVOL1  = (1<<1),
    WM8731_LINVOL_LINVOL2  = (1<<2),
    WM8731_LINVOL_LINVOL3  = (1<<3),
    WM8731_LINVOL_LINVOL4  = (1<<4),
    WM8731_LINVOL_LINMUTE  = (1<<7),
    WM8731_LINVOL_LRINBOTH = (1<<8),
} WM8731_LINVOL_Flags;


// RIGHT LINE IN - Register 0x01
typedef union {
    struct {
        unsigned RINVOL  :5; // Right channel input volume: 0x1F = +12 dB, 0x00 = -34.5 dB
        unsigned RINMUT  :1; // Right channel input mute to ADC: 1 = Enable Mute, 0 = Disable
        unsigned         :2;
        unsigned RLINBOTH:1; // Right to Left Channel Line Input Volume and Mute Data Load control:
    };                       //  1 = enable simultaneous load of right to left volume and mute
    struct {
        unsigned RINVOL0:1;
        unsigned RINVOL1:1;
        unsigned RINVOL2:1;
        unsigned RINVOL3:1;
        unsigned RINVOL4:1;
    };
    struct {
        unsigned w:16;
    };
} _WM8731_RINVOLbits;

typedef enum {
    WM8731_RINVOL_RINVOL   = 0x1F,
    WM8731_RINVOL_RINVOL0  = (1<<0),
    WM8731_RINVOL_RINVOL1  = (1<<1),
    WM8731_RINVOL_RINVOL2  = (1<<2),
    WM8731_RINVOL_RINVOL3  = (1<<3),
    WM8731_RINVOL_RINVOL4  = (1<<4),
    WM8731_RINVOL_RINMUTE  = (1<<7),
    WM8731_RINVOL_RLINBOTH = (1<<8),
} WM8731_RINVOL_Flags;


// LEFT HEADPHONE OUT - Register 0x02
typedef union{
    struct {
        unsigned LHPVOL  :7; // Left Channel Headphone Output:
                             //   0x7F = +6 dB to 0x30 = -73 dB in 1 dB steps
                             //   0x2F to 0x00 is mute
        unsigned LZCEN   :1; // Left Channel Zero Cross Detect Enable: 1 = Enable, 0 = disable
        unsigned LRHPBOTH:1; // Left to Right Channel Headphone Volume,
                             // Mute, and Zero Cross Data Load Control:
                             //   1 = enable simultaneous load of Left to Right, 0 = disable
    };
    struct {
        unsigned LHPVOL0:1;
        unsigned LHPVOL1:1;
        unsigned LHPVOL2:1;
        unsigned LHPVOL3:1;
        unsigned LHPVOL4:1;
        unsigned LHPVOL5:1;
        unsigned LHPVOL6:1;
    };
    struct {
        unsigned w:16;
    };
} _WM8731_LOUT1Vbits;

typedef enum {
    WM8731_LOUT1V_LHPVOL   = 0x7F, // Left Channel Headphone Output
    WM8731_LOUT1V_LHPVOL0  = (1<<0),
    WM8731_LOUT1V_LHPVOL1  = (1<<1),
    WM8731_LOUT1V_LHPVOL2  = (1<<2),
    WM8731_LOUT1V_LHPVOL3  = (1<<3),
    WM8731_LOUT1V_LHPVOL4  = (1<<4),
    WM8731_LOUT1V_LHPVOL5  = (1<<5),
    WM8731_LOUT1V_LHPVOL6  = (1<<6),
    WM8731_LOUT1V_LZCEN    = (1<<7), // Left Channel Zero Cross Detect Enable
    WM8731_LOUT1V_LRHPBOTH = (1<<8), // Left to Right Channel Headphone Volume
} WM8731_LOUT1V_Flags;


// RIGHT HEADPHONE OUT - Register 0x03
typedef union {
    struct {
        unsigned RHPVOL  :7; // Right Channel Headphone Output:
        unsigned RZCEN   :1; //   0x7F = +6 dB to 0x30 = -73 dB in 1 dB steps
        unsigned RLHPBOTH:1; //   0x2F to 0x00 is mute
                             // Right Channel Zero Cross Detect Enable: 1 = Enable, 0 = disable
                             // Right to LeftChannel Headphone Volume,
    };                       // Mute, and Zero Cross Data Load Control:
    struct {                 //   1 = enable simultaneous load of Right to Left, 0 = disable
        unsigned RHPVOL0:1;
        unsigned RHPVOL1:1;
        unsigned RHPVOL2:1;
        unsigned RHPVOL3:1;
        unsigned RHPVOL4:1;
        unsigned RHPVOL5:1;
        unsigned RHPVOL6:1;
    };
    struct {
        unsigned w:16;
    };
} _WM8731_ROUT1Vbits;

typedef enum {
    WM8731_ROUT1V_RHPVOL   = 0x7F, // Right Channel Headphone Output
    WM8731_ROUT1V_RHPVOL0  = (1<<0),
    WM8731_ROUT1V_RHPVOL1  = (1<<1),
    WM8731_ROUT1V_RHPVOL2  = (1<<2),
    WM8731_ROUT1V_RHPVOL3  = (1<<3),
    WM8731_ROUT1V_RHPVOL4  = (1<<4),
    WM8731_ROUT1V_RHPVOL5  = (1<<5),
    WM8731_ROUT1V_RHPVOL6  = (1<<6),
    WM8731_ROUT1V_RZCEN    = (1<<7), // Right Channel Zero Cross Detect Enable
    WM8731_ROUT1V_RLHPBOTH = (1<<8), // Right to Right Channel Headphone Volume

} WM8731_ROUT1V_Flags;


// ANALOG AUDIO PATH CONTROL - Register 0x04
typedef union {
    struct {
        unsigned MICBOOST:1; // Microphone Input Level Boost: 1 = enable boost, 0 = Disable
        unsigned MUTEMIC :1; // Mic Input Mute to ADC: 1 = Enable Mute, 0 = Disable
        unsigned INSEL   :1; // Mic/Line input Select to ADC: 1=MIC -> ADC, 0=Line -> ADC
        unsigned BYPASS  :1; // Bypass Switch: 1 = Enable Bypass, 0 = Disable
        unsigned DACSEL  :1; // DAC Select: 1 = Select DAC, 0 = Don't select DAC
        unsigned SIDETONE:1; // Sidetone Switch: 1 = Enable side tone, 0 = disable
        unsigned SIDEATT :2; // Side Tone Attenuation: 11 = -15 dB, 10 = -12 dB, 01 = -9 dB, 00 = -6 dB
    };
    struct {
        unsigned         :6;
        unsigned SIDEATT0:1;
        unsigned SIDEATT1:1;
    };
    struct {
        unsigned w:16;
    };
} _WM8731_APANAbits;

typedef enum {
    WM8731_APANA_MICBOOST = (1<<0), //
    WM8731_APANA_MUTEMIC  = (1<<1), //
    WM8731_APANA_INSEL    = (1<<2), //
    WM8731_APANA_BYPASS   = (1<<3), //
    WM8731_APANA_DACSEL   = (1<<4), //
    WM8731_APANA_SIDETONE = (1<<5), //
    WM8731_APANA_SIDEATT  = (0x3<<6), //
    WM8731_APANA_SIDEATT0 = (0x1<<6),
    WM8731_APANA_SIDEATT1 = (0x2<<6),
    WM8731_APANA_SIDEATT_15dB = (0x3<<6), // 11
    WM8731_APANA_SIDEATT_12dB = (0x2<<6), // 10
    WM8731_APANA_SIDEATT_9dB  = (0x1<<6), // 01
    WM8731_APANA_SIDEATT_6dB  = (0x0<<6), // 00
} WM8731_APANA_Flags;


// DIGITAL AUDIO PATH CONTROL - Register 0x05
typedef union {
    struct {
        unsigned ADCHPD:1; // ADC High Pass Filter Enable: 1 = Enable HP filter, 0 = disable
        unsigned DEEMPH:2; // De-emphasis Control: 11 = 48 kHz, 10 = 44.1 kHz, 01 = 32 kHz, 00 = disable
        unsigned DACMU :1; // DAC Soft Mute Control: 1 = Enable soft mute, 0 = disable
        unsigned HPOR  :1; // Store DC Offset when HP Filter Disabled: 1 = store offset, 0 = clear offset
    };
    struct {
        unsigned        :1;
        unsigned DEEMPH0:1;
        unsigned DEEMPH1:1;
    };
    struct {
        unsigned w:16;
    };
} _WM8731_APDIGIbits;

typedef enum {
    WM8731_APDIGI_ADCHPD  = (1<<0),
    WM8731_APDIGI_DEEMPH  = (3<<1),
    WM8731_APDIGI_DEEMPH0 = (1<<1),
    WM8731_APDIGI_DEEMPH1 = (2<<1),
    WM8731_APDIGI_DEEMPH_48kHz   = (3<<1), // 11
    WM8731_APDIGI_DEEMPH_44p1kHz = (2<<1), // 10
    WM8731_APDIGI_DEEMPH_32kHz   = (1<<1), // 01
    WM8731_APDIGI_DEEMPH_None    = (0<<1), // 00
    WM8731_APDIGI_DACMU = (1<<3),
    WM8731_APDIGI_HPOR  = (1<<4),
} WM8731_APDIGI_Flags;


// POWER DOWN CONTROL - Register 0x06
typedef union {
    struct {
        unsigned LINEINPD:1; // Line Input Power Down: 1 = Enable power down
        unsigned MICPD   :1; // Mic Input and Bias Power Down: 1 = Enable power down
        unsigned ADCPD   :1; // ADC Power Down: 1 = Enable power down
        unsigned DACPD   :1; // DAC Power Down: 1 = Enable power down
        unsigned OUTPD   :1; // Outputs Power Down: 1 = Enable power down
        unsigned OSCPD   :1; // Oscillator Power Down: 1 = Enable power down
        unsigned CLKOUTPD:1; // CLKOOUT Power Down: 1 = Enable power down
        unsigned POWEROFF:1; // POWEROFF Mode: 1 = Enable POWEROFF
    };
    struct {
        unsigned w:16;
    };
} _WM8731_PWRbits;

typedef enum {
    WM8731_PWR_LINEINPD = (1<<0),
    WM8731_PWR_MICPD    = (1<<1),
    WM8731_PWR_ADCPD    = (1<<2),
    WM8731_PWR_DACPD    = (1<<3),
    WM8731_PWR_OUTPD    = (1<<4),
    WM8731_PWR_OSCPD    = (1<<5),
    WM8731_PWR_CLKOUTPD = (1<<6),
    WM8731_PWR_POWEROFF = (1<<7),
} WM8731_PWR_Flags;


// DIGITAL INTERFACE FORMAT - Register 0x07
typedef union {
    struct {
        unsigned FORMAT :2; // Audio Data Format Select: 11 = DSP Mode,  10 = I2S
                            //                           01 = Left Just, 00 = Right Just
        unsigned IWL    :2; // Input Audio Bit Length: 11 = 32 bits, 10 = 24 bits,
                            //                         10 = 20 bits, 00 = 16 bits
        unsigned LRP    :1; // DACLRC Phase Control
        unsigned LRSWAP :1; // DAC Left Right Clock Swap: 1 = Swap Channels, 0 = don't
        unsigned MS     :1; // Master Slave Mode Control: 1 = Enable Master, 0 = Enable Slave
        unsigned BCLKINV:1; // Bit Clock Invert: 1 = Invert BCLK
    };
    struct {
        unsigned FORMAT0:1;
        unsigned FORMAT1:1;
        unsigned IWL0   :1;
        unsigned IWL1   :1;
    };
    struct {
        unsigned w:16;
    };
} _WM8731_IFACEbits;

typedef enum {
    WM8731_IFACE_FORMAT     = (0x3),
    WM8731_IFACE_FORMAT0    = (0x1),
    WM8731_IFACE_FORMAT1    = (0x2),
    WM8731_IFACE_FORMAT_DSP = (0x3),
    WM8731_IFACE_FORMAT_I2S = (0x2),
    WM8731_IFACE_FORMAT_LJ  = (0x1),
    WM8731_IFACE_FORMAT_RJ  = (0x0),
    WM8731_IFACE_IWL        = (0x3 << 2),
    WM8731_IFACE_IWL0       = (0x1 << 2),
    WM8731_IFACE_IWL1       = (0x2 << 2),
    WM8731_IFACE_IWL_32bits = (0x3 << 2), //11
    WM8731_IFACE_IWL_24bits = (0x2 << 2), //10
    WM8731_IFACE_IWL_20bits = (0x1 << 2), //01
    WM8731_IFACE_IWL_16bits = (0x0 << 2), //00
    WM8731_IFACE_LRP        = (1 << 5),
    WM8731_IFACE_MS         = (1 << 6),
    WM8731_IFACE_BCLKINV    = (1 << 7),
} WM8731_IFACE_Flags;


// SAMPLING CONTROL - Register 0x08
typedef union {
    struct {
        unsigned USB_NORMAL:1; // USB/NORMAL: 1 = USB Mode    (250/272 fs),
                               //             0 = Normal Mode (256/384 fs)
        unsigned BOSR      :1; // Base Oversampling Rate: 0 = 1st rate, 1 = 2nd rate
        unsigned SR        :4; // ADC and DAC Sample Rate: see data sheet
        unsigned CLKIDIV2  :1; // Core Clock Divider Select: 1 = CoreClk = MCLK/2, 0 = CoreClk = MCLK
        unsigned CLKODIV2  :1; // CLKOUT Divider Select: 1 => CLKOUT = CoreCLK/2, 0 => CLKOUT = CoreCLK
    };
    struct {
        unsigned    :2;
        unsigned SR0:1;
        unsigned SR1:1;
        unsigned SR2:1;
        unsigned SR3:1;
    };
    struct {
        unsigned w:16;
    };
} _WM8731_SRATEbits;

typedef enum {
    WM8731_SRATE_USB_NORMAL = (1<<0),
    WM8731_SRATE_BOSR       = (1<<1),
    WM8731_SRATE_SR         = (0xF<<2),
    WM8731_SRATE_SR0        = (0x1<<2),
    WM8731_SRATE_SR1        = (0x2<<2),
    WM8731_SRATE_SR2        = (0x4<<2),
    WM8731_SRATE_SR3        = (0x8<<2),
    WM8731_SRATE_CLKIDIV2   = (1<<6),
    WM8731_SRATE_CLKODIV2   = (1<<7),
} WM8731_SRATE_Flags;


// ACTIVE CONTROL - Register 0x09
typedef union {
    struct {
        unsigned Active:1; // Activate Interface: 1 = Active, 0 = Inactive
    };
    struct {
        unsigned w:16;
    };
} _WM8731_ACTIVEbits;

// RESET REGISTER - Register 0x0F
typedef union {
    struct {
        unsigned RESET:9; // Reset: Writing 0 0000 0000 resets device
    };
    struct {
        unsigned RESET0:1;
        unsigned RESET1:1;
        unsigned RESET2:1;
        unsigned RESET3:1;
        unsigned RESET4:1;
        unsigned RESET5:1;
        unsigned RESET6:1;
        unsigned RESET7:1;
        unsigned RESET8:1;
    };
    struct {
        unsigned w:16;
    };
} _WM8731_RESETbits;


/*****************************************************************************
  Defines The Possible Read Write Modes For The Codec Driver.
 *****************************************************************************/
typedef enum
{
    O_RDONLY,   /* Open the codec for a read operation only */
    O_WRONLY,   /* Open the codec for write operation only   */
    O_RDWR      /* Open the codec for both read and write   */
} O_MODE;


/*****************************************************************************
  Needed for I2C errata fix only
 *****************************************************************************/
#if defined( WM8731DRV_I2C_MODULE )
#   define WM8731DRV_I2C2_SDA_LAT  LATAbits.LATA3
#   define WM8731DRV_I2C2_SDA_TRIS TRISAbits.TRISA3
#endif//defined( WM8731DRV_I2C_MODULE )


/*****************************************************************************
  Basic Functions
 *****************************************************************************/
WM8731_CODEC * WM8731CodecOpen(WM8731_CODEC * codecHandle,
                               STEREO_AUDIO_DATA* txBuffer,
                               STEREO_AUDIO_DATA * rxBuffer,
                               O_MODE mode);

UINT WM8731Codec_read(WM8731_CODEC * codecHandle,
                      STEREO_AUDIO_DATA * data,
                      UINT nSamples);

UINT WM8731Codec_write(WM8731_CODEC * codecHandle,
                       STEREO_AUDIO_DATA * data,
                       UINT nSamples);

void WM8731CodecIOCtl(WM8731_CODEC * codecHandle,
                      WM8731_REGISTER commandRegister,
                      void * data);


/*****************************************************************************
  Helper Functions
 *****************************************************************************/
void I2SInit(void);
void CMDInit(void);
void CMDDeinit(void);


void WM8731CodecLoadStartup(WM8731_CODEC * codecHandle);

void WM8731CodecSetVolume( WM8731_CODEC * pCodecHandle, UINT16 Volume_In_dB );

void WM8731Codec_MusicOn( BOOL MusicOn );

void WM8731DRV_WhatsTheNewWavFile( FSFILE * NewWavFile );

void WM8731DRV_SetBufferCount( UINT32 Nbuffers );

UINT16 WM8731DRV_GetSongElapsedTime(void);

BOOL WM8731_MusicOnOff(void);

void WM8731Codec_Reset(void);

#endif//indef _WM8731_DRV_H_


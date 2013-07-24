/**********************************************************************
* © 2010 Microchip Technology Inc.
*
* FileName:        WM8731Drv.c
* Includes:
*     PRIVATE void SPIInit(void)
*
*     void I2SInit(void)
*
*     PRIVATE void I2CInit(void)
*
*     void CMDInit(void)
*
*     void CMDDeinit(void)
*
*     PRIVATE BOOL CMDWriteI2C(UINT reg, UINT val)
*
*     PRIVATE BOOL CMDWriteSPI(UINT reg, UINT val)
*
*     void WM8731CodecIOCtl(WM8731_CODEC * codecHandle,
*                           WM8731_REGISTER commandRegister,
*                           void * data)
*
*     void WM8731CodecLoadStartup(WM8731_CODEC * pCodecHandle)
*
*     void WM8731CodecSetVolume( INT volume)
*
*
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       PIC32MX
* Compiler:        MPLAB® C32
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and
* intellectual property rights in the code accompanying this message and in all
* derivatives hereto.  You may use this code, and any derivatives created by
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY),
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL,
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and
* determining its suitability.  Microchip has no obligation to modify, test,
* certify, or support the code.
************************************************************************/
#include "WM8731Drv.h"
#include <stdlib.h>
#include <GenericTypeDefs.h>
#include "TimeDelay.h"
#include "peripheral\pps.h"

#include "AssertToDisplay.h"
#include "dsplib_dsp.h"


void I2SInit(void)
{
    UINT32 ReadData;

    LRCLK_PPS_SETUP;
    LRCLK_TRIS_SETUP;
    DAC_DATA_PPS_SETUP;
    DAC_DATA_TRIS_SETUP;


    // Setup I2S on SPI1
    I2SCON = I2SCON2 = 0;   // Reset settings
    I2SSTATBITS.SPIROV = 1; // Clear overflow, if set
    I2SSTAT = 0;
    ReadData = I2SBUF;      // Read any data



    IFS0bits.CS0IF = 0; // Clear interrupt flag for software
    IPC0bits.CS0IP = 3; // FileReadISR priority < I2S priority
    IPC0bits.CS0IS = 0; // Secondary priority = 0;


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
    // Fill xmit buffer
    SPI1BUF = 0;
    SPI1BUF = 0;
    SPI1BUF = 0;
    SPI1BUF = 0;
    SPI1BUF = 0;
    SPI1BUF = 0;
    SPI1BUF = 0;
    SPI1BUF = 0;
    SPI1BUF = 0;

}

void CMDInit(void)
{
    SpiChannel   ChanSPI= WM8731DRV_CMD_SPI_CHANNEL; // the SPI channel to use
    SpiOpenFlags oFlags = SPI_OPEN_MODE16   | // 16 bit serial word length
                          SPI_OPEN_CKE_REV  | // Reverse polarity of clock
                          SPI_OPEN_MSTEN    | // Master
                          SPI_OPEN_DISSDI   | // Disable SDI input
                          SPI_OPEN_FRMEN    | // Enable frame to keep SCK on
                          SPI_OPEN_ON       ; // Turn on SPI

    // Configure SPI output pin SDO2
    TRISACLR = BIT_8;
    PPSOutput(2,RPA8,SDO2);

    // Configure SCK2 pin as output
    TRISBCLR = BIT_15;

    if ( PMCONbits.ON == 1 ) // if PMP module is on, use PMA6 instead
    {
        PMAENSET  = 1 << 6; // Use PMA6 instead of RC9
        PMADDRSET = 1 << 6; // Drive SS2_Bar high
    }
    else // PMP not on, can use GPIO
    {
        TRISCCLR = BIT_9; // manually drive SSX pin
        LATCSET  = BIT_9; // Drive SS2_Bar high
    }

    SpiChnOpen(ChanSPI, oFlags, 160); // 40 MHz / 160 = 250 KHz SCLK
    Delay1us(16);
}
void CMDDeinit(void)
{
    SpiChannel ChanSPI = WM8731DRV_CMD_SPI_CHANNEL; // the SPI channel to use

    if ( PMCONbits.ON == 1 ) // if PMP module is on, use PMA6 instead
    {
        PMADDRCLR = 1 << 6; // clear address bit
        PMAENCLR  = 1 << 6; // Stop using PMA6
    }
    else // PMP not on, can use GPIO
    {
        LATCCLR  = BIT_9; // Drive SS2_Bar low
    }
    // Shutdown SPI command interface to CODEC
    SpiChnClose(ChanSPI);

}


PRIVATE BOOL CMDWriteSPI(UINT reg, UINT val)
{
    SpiChannel ChanSPI = WM8731DRV_CMD_SPI_CHANNEL; // the SPI channel to use
    UINT16_BITS Command;

    Command.byte.LB = (BYTE) (val & 0xFF);
    Command.byte.HB = (BYTE) ((reg << 1) | (val >> 8));

    if ( PMCONbits.ON == 1 ) // if PMP module is on, use PMA6 instead
    {
        PMADDRCLR = 1 << 6;  // Drive SS2_Bar low
    }
    else // PMP not on, can use GPIO
    {
        LATCCLR  = BIT_9;    // Drive SS2_Bar low
    }

    SpiChnPutS(ChanSPI, (unsigned int*)&Command, 1);    // write packet

    while ( SPI2STATbits.SPIBUSY )
    {
        //Do nothing
    }

    if ( PMCONbits.ON == 1 ) // if PMP module is on, use PMA6 instead
    {
        PMADDRSET = 1 << 6;  // Drive SS2_Bar high
    }
    else // PMP not on, can use GPIO
    {
        LATCSET  = BIT_9;    // Drive SS2_Bar high
    }
    Delay10us(20);   // Make sure chip select bar is high long enough

    return(TRUE);
}


void WM8731CodecIOCtl(WM8731_CODEC * codecHandle,
                      WM8731_REGISTER commandRegister,
                      void * data)
{
  /* Send the specified command to the specified WM8731 register */

    UINT command = *((UINT *)(data));
#  if   defined( WM8731DRV_I2C_MODULE )
    *(BOOL *)data = CMDWriteI2C(commandRegister, command);
#  elif defined( WM8731DRV_CMD_MODULE )
    *(BOOL *)data = CMDWriteSPI(commandRegister, command);
#  else
#      error("No command interface defined")
#  endif//defined( WM8731DRV_I2C_MODULE )
}


/*****************************************************************************
  The following macros specify the start up codec configuration. Change this
  to match the applicaiton needs. This values will be loaded by the
  WM8731CodecLoadStartup() function. Note that you can still change the codec
  settings at run time using the WM8731CodecIOCtl() function. Refer to the
  WM8731 data sheet for interpretation of these values.
 *****************************************************************************/
void WM8731CodecLoadStartup(WM8731_CODEC * pCodecHandle)
{
/* This function loads the codec register with the starup values specified in
   the WM8731Drv.h file
 */
    _WM8731_LINVOLbits WM8731_LINVOLbits; // Left  Line In
    _WM8731_RINVOLbits WM8731_RINVOLbits; // Right Line In
    _WM8731_LOUT1Vbits WM8731_LOUT1Vbits; // Left  Headphone Out
    _WM8731_ROUT1Vbits WM8731_ROUT1Vbits; // Right Headphone Out
    _WM8731_APANAbits  WM8731_APANAbits;  // Analog  Audio Path Control
    _WM8731_APDIGIbits WM8731_APDIGIbits; // Digital Audio Path Control
    _WM8731_PWRbits    WM8731_PWRbits;    // Power Down Control
    _WM8731_IFACEbits  WM8731_IFACEbits;  // Digital Audio Interface Format

    _WM8731_SRATEbits  WM8731_SRATEbits;  // Sampling Control
    _WM8731_ACTIVEbits WM8731_ACTIVEbits; // Active Control
    _WM8731_RESETbits  WM8731_RESETbits;  // Reset Register

    UINT16 Command = 0;
    UINT16 AltCommand;

    Command = 0x00; // Disable power downs
    WM8731CodecIOCtl(pCodecHandle, WM8731_PWR, &Command);

    Command = 0; // Reset CODEC
    WM8731CodecIOCtl(pCodecHandle, WM8731_RESET,&Command );

    Command = 0x0; // Deactivate interface??
    WM8731CodecIOCtl(pCodecHandle, WM8731_ACTIVE, &Command);

    WM8731_APANAbits.w = 0; //reset
    WM8731_APANAbits.MUTEMIC = 1; // Mute Mic
    WM8731_APANAbits.DACSEL = 1;  // Select DAC
    Command = WM8731_APANAbits.w;
    AltCommand = WM8731_APANA_MUTEMIC | WM8731_APANA_DACSEL;
    WM8731CodecIOCtl(pCodecHandle, WM8731_APANA, &Command);

    WM8731_APDIGIbits.w = 0; //Reset
    WM8731_APDIGIbits.ADCHPD = 1; // Disable High Pass Filter
    WM8731_APDIGIbits.DEEMPH = 0x2; // 44.1 kHz deemphasis
    Command = WM8731_APDIGIbits.w;
    AltCommand = WM8731_APDIGI_ADCHPD | WM8731_APDIGI_DEEMPH_44p1kHz;
    WM8731CodecIOCtl(pCodecHandle, WM8731_APDIGI,  &Command);

    WM8731_PWRbits.w = 0; // Reset
    WM8731_PWRbits.LINEINPD = 1; // Line In power down
    WM8731_PWRbits.CLKOUTPD = 1; // Clock out powered down
    WM8731_PWRbits.ADCPD = 1; // Power down ADC
    WM8731_PWRbits.MICPD = 1; // Power down Mic
    Command = WM8731_PWRbits.w;
    AltCommand = WM8731_PWR_LINEINPD |
                 WM8731_PWR_ADCPD    |
                 WM8731_PWR_MICPD    ;
    WM8731CodecIOCtl(pCodecHandle, WM8731_PWR,&Command );

    WM8731_IFACEbits.w = 0; // Reset
    WM8731_IFACEbits.FORMAT = 0x2; // I2S mode
#ifdef SLAVE_CODEC    
    WM8731_IFACEbits.MS     = 0x0; // Slave enabled
#else
    WM8731_IFACEbits.MS     = 0x1; // Master enabled
#endif    
    
    Command = WM8731_IFACEbits.w;
    AltCommand = WM8731_IFACE_FORMAT_I2S    |
                 WM8731_IFACE_MS |
                 WM8731_IFACE_IWL_16bits    ;
    WM8731CodecIOCtl(pCodecHandle, WM8731_IFACE,&Command );

    WM8731_SRATEbits.w = 0; // Reset
#ifdef SLAVE_CODEC
	WM8731_SRATEbits.USB_NORMAL = 0; // Normal mode
    WM8731_SRATEbits.BOSR       = 0; // 272 fs oversampling
    WM8731_SRATEbits.SR         = 0x8; // 
#else
    WM8731_SRATEbits.USB_NORMAL = 1; // USB mode
    WM8731_SRATEbits.BOSR       = 1; // 272 fs oversampling
    WM8731_SRATEbits.SR         = 0x8; // fs = 12 MHz / 272 = 44.118 kHz
#endif
    Command = WM8731_SRATEbits.w;
    WM8731CodecIOCtl(pCodecHandle, WM8731_SRATE,&Command );

    WM8731_LINVOLbits.w = 0; // Reset
    WM8731_LINVOLbits.LINVOL = 0x17; // +12 dB
    Command = WM8731_LINVOLbits.w;
    WM8731CodecIOCtl(pCodecHandle, WM8731_LINVOL,&Command);

    WM8731_RINVOLbits.w = 0; // Reset
    WM8731_RINVOLbits.RINVOL = 0x17; // +12 dB
    Command = WM8731_RINVOLbits.w;
    WM8731CodecIOCtl(pCodecHandle, WM8731_RINVOL,&Command);

    WM8731_LOUT1Vbits.w = 0; // Reset
    WM8731_LOUT1Vbits.LHPVOL = 0x79; // + 0 dB
    Command = WM8731_LOUT1Vbits.w;
    WM8731CodecIOCtl(pCodecHandle, WM8731_LOUT1V,&Command);

    WM8731_ROUT1Vbits.w = 0; // Reset
    WM8731_ROUT1Vbits.RHPVOL = 0x79;  // +0 dB
    Command = WM8731_ROUT1Vbits.w;
    WM8731CodecIOCtl(pCodecHandle, WM8731_ROUT1V, &Command);

    Command = 1; // Activate interface
    WM8731CodecIOCtl(pCodecHandle, WM8731_ACTIVE, &Command);

}

void WM8731CodecSetVolume( WM8731_CODEC * pCodecHandle, UINT16 Volume_In_dB )
{
    UINT16 Command;
    _WM8731_LOUT1Vbits WM8731_LOUT1Vbits; // Left  Headphone Out
    _WM8731_ROUT1Vbits WM8731_ROUT1Vbits; // Right Headphone Out

    CMDInit(); // Setup command interface to configure CODEC
    Delay10us(1);

    Volume_In_dB = ( Volume_In_dB > 0x7F ) ? 0x7F : Volume_In_dB; // volume <= 6 dB
    Volume_In_dB = ( Volume_In_dB < 0x30 ) ? 0x30 : Volume_In_dB; // volume >= -74dB
    // -74 dB ==> mute

    WM8731_LOUT1Vbits.w = 0; // Reset
    WM8731_LOUT1Vbits.LHPVOL = Volume_In_dB;
    Command = WM8731_LOUT1Vbits.w;
    WM8731CodecIOCtl(pCodecHandle, WM8731_LOUT1V,&Command);

    WM8731_ROUT1Vbits.w = 0; // Reset
    WM8731_ROUT1Vbits.RHPVOL = Volume_In_dB;  // +0 dB
    Command = WM8731_ROUT1Vbits.w;
    WM8731CodecIOCtl(pCodecHandle, WM8731_ROUT1V,&Command);

  //thisCodec->gain = Volume_In_dB;
    CMDDeinit();
}

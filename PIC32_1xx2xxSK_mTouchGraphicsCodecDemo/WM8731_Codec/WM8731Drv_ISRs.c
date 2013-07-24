/**********************************************************************
* © 2010 Microchip Technology Inc.
*
* FileName:        WM8731Drv.c
* Includes:
*
*     void WM8731Codec_MusicOn( BOOL MusicOn )
*
*     BOOL WM8731_MusicOnOff(void)
*
*     void WM8731Codec_Reset(void)
*
*     void WM8731DRV_WhatsTheNewWavFile( FSFILE * NewWavFile )
*
*     void WM8731DRV_SetBufferCount( UINT32 Nbuffers )
*
*     UINT16 WM8731DRV_GetSongElapsedTime(void)
*
*     void __ISR(_CORE_SOFTWARE_0_VECTOR, IPL3SOFT) FileReadISR(void)
*
*      void __attribute__((vector(WM8731DRV_TX_XFER_DONE_VECTOR),
*                         interrupt(WM8731DRV_TX_XFER_DONE_IPL),
*                         nomips16)) SPI_I2S_TRANSMIT(void)
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
#include "module_dyn_header.h"


// This provides a single tone sine wave to validate music interface to CODEC
// 44 samples per period square wave.
// Sample Rate for Codec = 44,118 Hz, so Freq = 44118/44 = 1002.68 Hz



// From UpdateSliderOrButton
extern volatile UINT16 SliderValue;

/* Audio Codec data */
STEREO_AUDIO_DATA StereoDataIn[SRC_IN_PROC_SIZE]  __attribute__((aligned(4)));
STEREO_AUDIO_DATA DataIn[SRC_IN_PROC_SIZE/2]; // Buffer for MDD file reads

STEREO_AUDIO_DATA *pDataStart    = &StereoDataIn[0];
STEREO_AUDIO_DATA *pDataEnd      = &StereoDataIn[SRC_IN_PROC_SIZE-1];

STEREO_AUDIO_DATA *pBufferStart[2] = { &StereoDataIn[0],
                                       &StereoDataIn[SRC_IN_PROC_SIZE/2] };
STEREO_AUDIO_DATA *pBufferEnd[2]   = { &StereoDataIn[SRC_IN_PROC_SIZE/2 - 1],
                                       &StereoDataIn[SRC_IN_PROC_SIZE-1] };
static STEREO_AUDIO_DATA *pNextData = &StereoDataIn[0];

BOOL volatile BufferEmpty[2] = { TRUE, TRUE };

static volatile BOOL CodecMusicOn = FALSE;
static UINT32 NumBuffersRead = 0;


void WM8731Codec_MusicOn( BOOL MusicOn )
{
    CodecMusicOn = MusicOn;
}


BOOL WM8731_MusicOnOff(void)
{
    return CodecMusicOn;
}


void WM8731Codec_Reset(void)
{
    pNextData = &StereoDataIn[0];
}



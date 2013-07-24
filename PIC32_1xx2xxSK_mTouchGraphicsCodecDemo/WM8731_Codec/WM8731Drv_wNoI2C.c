/*********************************************************************
 *
 *                  Wolfson WM 8731 codec playback and record functions
 *
 *********************************************************************
 * FileName:        audio_wm871.c
 * Dependencies:
 * Processor:       PIC32
 *
 * Complier:        MPLAB Cxx
 *                  MPLAB IDE
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 * Microchip Audio Library – PIC32 Software.
 * Copyright © 2008 Microchip Technology Inc.  All rights reserved.
 *
 * Microchip licenses the Software for your use with Microchip microcontrollers
 * and Microchip digital signal controllers pursuant to the terms of the
 * Non-Exclusive Software License Agreement accompanying this Software.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY
 * OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION,
 * ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS
 * FOR A PARTICULAR PURPOSE.
 * MICROCHIP AND ITS LICENSORS ASSUME NO RESPONSIBILITY FOR THE ACCURACY,
 * RELIABILITY OR APPLICATION OF THE SOFTWARE AND DOCUMENTATION.
 * IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED
 * UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH
 * OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL,
 * SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS
 * OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY,
 * SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED
 * TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *
 ********************************************************************/

#include <stdlib.h>
#include <string.h>

// #include <GenericTypeDefs.h>

#include <plib.h>

#define WM8731_ADDRESS  0x1a

#define WM8731_REG_LLINE_IN         0x0
#define WM8731_REG_RLINE_IN         0x1
#define WM8731_REG_LHPHONE_OUT      0x2
#define WM8731_REG_RHPHONE_OUT      1
#define WM8731_REG_ANALOG_PATH      4
#define WM8731_REG_DIGITAL_PATH     0x5
#define WM8731_REG_PDOWN_CTRL       0x6
#define WM8731_REG_DIGITAL_IF       0x7
#define WM8731_REG_SAMPLING_CTRL    0x8
#define WM8731_REG_ACTIVE_CTRL      0x9
#define WM8731_REG_RESET            0xf

#define Fsck        100000  // standard I2C bus frequency




// direct bit bang access

#define BITBANG_I2C

#define I2C_DATA    BIT_15
#define I2C_CLK     BIT_14

#define I2C_PORT    PORTA
#define I2C_TRISCLR TRISACLR
#define I2C_TRISSET TRISASET
#define I2C_LATSET  LATASET
#define I2C_LATCLR  LATACLR

void i2c_wait()
{
    int i;
    for(i=0; i<50; i++)
    ;
}


void i2c_init()
{
    I2C_LATCLR = I2C_DATA | I2C_CLK;        // clear I2C Data bits
    I2C_TRISSET = I2C_DATA | I2C_CLK;       // float I2C bits
}


void i2c_start()
{
    i2c_wait();             // assume clk and data high
    I2C_TRISCLR = I2C_DATA;
    i2c_wait();
    I2C_TRISCLR = I2C_CLK;
    i2c_wait();
}

void i2c_restart()
{
    i2c_wait();
    I2C_TRISCLR = I2C_DATA;
    i2c_wait();
}

void i2c_stop()             // assume clk is already low
{
    I2C_TRISCLR = I2C_DATA; // make sure data is low
    i2c_wait();
    I2C_TRISSET = I2C_CLK;      // bring clk up
    i2c_wait();
    I2C_TRISSET = I2C_DATA; // now data
    i2c_wait();
}



unsigned long i2c_send(unsigned char c)
{
    unsigned char bit;
    unsigned short ack;

    for (bit=0x80; bit; bit>>=1)
    {
        if (c & bit)
            I2C_TRISSET = I2C_DATA; // bit high
        else
            I2C_TRISCLR = I2C_DATA; // bit low

        i2c_wait();
        I2C_LATSET =  I2C_CLK;
        I2C_LATSET =  I2C_CLK;
        I2C_TRISSET = I2C_CLK;          // clock high
        i2c_wait();
        I2C_LATCLR  = I2C_CLK;
        I2C_TRISCLR = I2C_CLK;          // clock low
        i2c_wait();
    }
    I2C_TRISSET = I2C_DATA;         // ack cycle
    i2c_wait();
    I2C_LATSET =  I2C_CLK;
    I2C_LATSET =  I2C_CLK;
    I2C_TRISSET = I2C_CLK;              // clock high
    i2c_wait();
    ack = I2C_PORT & I2C_DATA;          // get ack bit
    I2C_LATCLR  = I2C_CLK;
    I2C_TRISCLR = I2C_CLK;              // clock low
    i2c_wait();
    return ack?1:0;
        /*if(ack)
        return 0x8000;*/
}


#ifdef AUDIO_SUPPORT_PLAY
static AUDIO_RES Wm8731InitPlay(const AudioEnvDcpt* pEnv, int sampleRate)
{
    int fail=0;

    i2c_init();

    fail|=WmMasterWrite(WM8731_REG_RESET, 0x00);    // reset device

    fail|=WmMasterWrite(WM8731_REG_LLINE_IN, 0x180);    // LRINBOTH|LINMUTE|(-34.5dB); use just the left channel
    fail|=WmMasterWrite(WM8731_REG_LHPHONE_OUT, 0x17f); // LRHPBOTH|!LZCEN|+6dB; use the left headphone commands
    fail|=WmMasterWrite(WM8731_REG_ANALOG_PATH, 0xd0);  // (SIDEATT=-15dB)|!SIDETONE|DACSEL|!BYPASS;

    fail|=WmMasterWrite(WM8731_REG_DIGITAL_PATH, 0x05); // !DACMU|(DEEMP=44.1KHz);

    fail|=WmMasterWrite(WM8731_REG_PDOWN_CTRL, 0x07);   // ADCPD|MICPD|LINEINPD
    fail|=WmMasterWrite(WM8731_REG_DIGITAL_IF, 0x53);   // MS|LRP|DSP_MODE

//  fail|=WmMasterWrite(WM8731_REG_SAMPLING_CTRL, 0x2f);    // SR(1011)|BOSR|USB;
    fail|=WmMasterWrite(WM8731_REG_SAMPLING_CTRL, 0x0d);    // SR(0011)|!BOSR|USB; 8KHz exactly, but no filtering!

    // WmMasterWrite(WM8731_REG_ACTIVE_CTRL, 0x01); // ACTIVE; not right now. turn on when enabling the out amp


    return fail?AUDIO_RES_EXT_WRITE_ERROR:AUDIO_RES_OK;
}

static void Wm8731DisableOutAmp(void)
{
    WmMasterWrite(WM8731_REG_ACTIVE_CTRL, 0x00);    // ACTIVE; turn off
}

static void Wm8731EnableOutAmp(void)
{
    WmMasterWrite(WM8731_REG_ACTIVE_CTRL, 0x01);    // ACTIVE; turn on
}
#endif  // AUDIO_SUPPORT_PLAY


#ifdef  AUDIO_SUPPORT_RECORD
static AUDIO_RES Wm8731InitRecord(const AudioEnvDcpt* pEnv, int sampleRate)
{
    int fail=0;

    i2c_init();

    fail|=WmMasterWrite(WM8731_REG_RESET, 0x00);    // reset device

    fail|=WmMasterWrite(WM8731_REG_LLINE_IN, 0x180);    // LRINBOTH|LINMUTE|(-34.5dB); use just the left channel
    fail|=WmMasterWrite(WM8731_REG_ANALOG_PATH, 0x05);  // INSEL|!MUTEMIC|MICBOOST;
    fail|=WmMasterWrite(WM8731_REG_DIGITAL_PATH, 0x00); // !HPOR|!ADCHPD;

    fail|=WmMasterWrite(WM8731_REG_PDOWN_CTRL, 0x19);   // OUTPD|DACPD|LINEINPD
    fail|=WmMasterWrite(WM8731_REG_DIGITAL_IF, 0x53);   // MS|LRP|DSP_MODE
    // fail|=WmMasterWrite(WM8731_REG_SAMPLING_CTRL, 0x2f); // SR(1011)|BOSR|USB;
    fail|=WmMasterWrite(WM8731_REG_SAMPLING_CTRL, 0x0d);    // SR(0011)|!BOSR|USB; 8KHz but no filter
    //  WmMasterWrite(WM8731_REG_ACTIVE_CTRL, 0x01);    // ACTIVE; not right now. turn on when enabling the in amp

    return fail?AUDIO_RES_EXT_WRITE_ERROR:AUDIO_RES_OK;
}

static void Wm8731DisableInAmp(void)
{
    WmMasterWrite(WM8731_REG_ACTIVE_CTRL, 0x00);    // ACTIVE; turn off
}

static void Wm8731EnableInAmp(void)
{
    WmMasterWrite(WM8731_REG_ACTIVE_CTRL, 0x01);    // ACTIVE; turn on
}
#endif  // AUDIO_SUPPORT_RECORD


// returns 0 for success!
static int WmMasterWrite(unsigned char regAdd, unsigned short cmd)
{
    unsigned char   buff[4];    // store the words here
    unsigned short  wmWord;
    int     fail=0;


    wmWord=((unsigned short)regAdd<<9)|(cmd&0x1ff); // reg address is 7 bits, command is 9 bits
    buff[0]=(WM8731_ADDRESS<<1)|0;  // issue a write command with WM8731 address
    buff[1]=(unsigned char)(wmWord>>8);
    buff[2]=(unsigned char)(wmWord);

    // now transmit
    i2c_start();
    i2c_send(buff[0]);i2c_send(buff[1]);i2c_send(buff[2]);
    i2c_stop();

    return fail;
}


/*static int WmMasterRead(unsigned char* pBuff, int nChars)
{
    int fail=0;

    StartI2C1();        // Send the Start Bit
    IdleI2C1();     // Wait to complete

    // issue a read command
    if(MasterWriteI2C1((WM8731_ADDRESS<<1)|1)!=0)
    {
        fail=1; // failed
    }
    else while(nChars--)
    {
        *pBuff++=MasterReadI2C1();
    }

    StopI2C1(); // Send the Stop condition
    IdleI2C1(); // Wait to complete


    return !fail;
}
*/





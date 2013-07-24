/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  OTM2201A LCD controller driver
 *  to be used with Audio Development Kit
 *****************************************************************************
 * FileName:        OTM2201A.c
 * Dependencies:    Graphics.h
 * Processor:       PIC24, PIC32
 * Compiler:        MPLAB C30, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2008 Microchip Technology Inc.  All rights reserved.
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
 * Adam Folts           11/01/10
 *****************************************************************************/
#include "Graphics.h"
#include "GOL.h"
#include "OTM2201A.h"

// Color
#ifdef COLOR_24BPP
DWORD_VAL    _color;
#else
WORD    _color;
#endif
#define BUFFER_SIZE 0x100000
// Current Page
WORD    _page;
extern volatile DWORD  tick;                                   // tick counter
// Clipping region control
SHORT   _clipRgn;

// Clipping region borders
SHORT   _clipLeft;
SHORT   _clipTop;
SHORT   _clipRight;
SHORT   _clipBottom;

#ifdef COLOR_24BPP
#define BLUE8(color16)   (BYTE) ((color16 & 0x0000FF))
#define GREEN8(color16) (BYTE) ((color16 & 0x00FF00) >> 8)
#define RED8(color16)  (BYTE) ((color16 & 0xFF0000) >> 16)
#else
#define RED8(color16)   (BYTE) ((color16 & 0xF800) >> 8)
#define GREEN8(color16) (BYTE) ((color16 & 0x07E0) >> 3)
#define BLUE8(color16)  (BYTE) ((color16 & 0x001F) << 3)
#endif
/////////////////////// LOCAL FUNCTIONS PROTOTYPES ////////////////////////////
void        SetAddress(DWORD address);
void        SetReg(BYTE index, WORD value);
BYTE        GetReg(WORD index);


void        PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE *bitmap, BYTE stretch);
void        PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE *bitmap, BYTE stretch);
void        PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE *bitmap, BYTE stretch);
void        PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE *bitmap, BYTE stretch);

void        PutImage1BPPExt(SHORT left, SHORT top, void *bitmap, BYTE stretch);
void        PutImage4BPPExt(SHORT left, SHORT top, void *bitmap, BYTE stretch);
void        PutImage8BPPExt(SHORT left, SHORT top, void *bitmap, BYTE stretch);
void        PutImage16BPPExt(SHORT left, SHORT top, void *bitmap, BYTE stretch);

/*********************************************************************
* Macro:  WritePixel(color)
*
* PreCondition: none
*
* Input: color
*
* Output: none
*
* Side Effects: none
*
* Overview: writes pixel at the current address
*
* Note: chip select should be enabled
*
********************************************************************/
#ifdef USE_16BIT_PMP
#define WritePixel(color)   DeviceWrite(color)
#else
#ifdef USE_PALETTE
#define WritePixel(color)   DeviceWrite(color)
#else
#define WritePixel(color)   DeviceWrite(color)
#endif
#endif

/*********************************************************************
* Function:  void SetAddress(DWORD address)
*
* PreCondition: none
*
* Input: address - address
*
* Output: none
*
* Side Effects: none
*
* Overview: sets the address for read/write operations
*
* Note: chip select should be enabled
*
********************************************************************/
void SetAddress(DWORD address)
{

}

/*********************************************************************
* Function:  void  SetReg(WORD index, BYTE value)
*
* PreCondition: none
*
* Input: index - register number
*        value - value to be set
*
* Output: none
*
* Side Effects: none
*
* Overview: sets graphics controller register (byte access)
*
* Note: none
*
********************************************************************/
void SetReg(BYTE index, WORD value)
{

CLR_CS(); // chip selected

CLR_RS(); // set to write to the control registers
SET_RD(); // not reading
SET_WR(); // not writing yet
DeviceWrite(0x00); // write the command to the port
CLR_WR(); // bring write line low
SET_WR(); // clock in the command
DeviceWrite(index);// write the command to the port
CLR_WR(); // bring write line low
SET_WR(); // clock in the command

SET_RS(); // set to write data
SET_RD(); // not reading
SET_WR(); // not writing yet
DeviceWrite(value >> 8); // write the command to the port
CLR_WR(); // bring write line low
SET_WR(); // clock in the command
DeviceWrite(value); // write the command to the port
CLR_WR(); // bring write line low
SET_WR(); // clock in the command

SET_CS(); // unselect chip

}

/*********************************************************************
* Function:  BYTE GetReg(WORD index)
*
* PreCondition: none
*
* Input: index - register number
*
* Output: none
*
* Side Effects: none
*
* Overview: returns graphics controller register value (byte access)
*
* Note: none
*
********************************************************************/
BYTE GetReg(WORD index)
{
	// To be created
	return 0;
}

/*********************************************************************
* Function:  void ResetDevice()
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: resets LCD, initializes PMP
*
* Note: none
*
********************************************************************/
void ResetDevice(void)
{

/////////////////////////////////////////////////////////////////////
// Initialize the device
/////////////////////////////////////////////////////////////////////
DeviceInit();

SET_RESET();
DelayMs(10);
CLR_RESET();
DelayMs(100);
SET_RESET();
DelayMs(500);
SetReg(0x11,0x0018); // Booster circuits not started automatically, controlled through PON0-3
SetReg(0x12,0x0000); // Generate VGH VCI1 x 5 (13.75v) / VGL VCI1 x -3 (-8.25v)
SetReg(0x13,0x0063); // clock cycle of external (RGB) interface (as default, unused)
SetReg(0x14,0x556A); // VCOMG = 0 ( Amplitude of VCOM = |VCOMH-VCOML| ), VCOMH = GVDD x 0.8690
SetReg(0x10,0x0800); // Constant current in op-amp Medium Fast 1

DelayMs(50);
SetReg(0x11,0x0118); // Start booster circuit 1
DelayMs(50);
SetReg(0x11,0x0318); // Booster circuit 1 on, start VGH circuit
DelayMs(50);
SetReg(0x11,0x0718); // Booster circuit 1, VGH on, start VGL circuit
DelayMs(50);
SetReg(0x11,0x0F18); // Booster circuit 1, VGH, VGL on, Start VCL circuit
DelayMs(50);
SetReg(0x11,0x0F38); // Booster circuit 1, VGH, VGL on, Start VCL circuit

DelayMs(50);
SetReg(0x07,0x001A);    // FLM output disabled// Gate output Enabled, Normally black
SetReg(0x01,0x011C);    // Decrement Address counter// 528x220 dots (176xRGBx220)
SetReg(0x03,0x1030);    // RGB Mode Increment automatically the address counter, PMP interface
SetReg(0x07,0x0000);    // FLM output disabled// Gate output disabled
SetReg(0x08,0x0808);    // 8 lines for the front porch// 8 lines for the back porch
SetReg(0x15,0x0020);    // Sn=2, Vcom1=1/2, Vcom2=2/1, RGB=16dotclock

SetReg(0x36,0x00AF);    // Set the horizontal start position of a window for memory access// 175
SetReg(0x37,0x0000);    // Set the horizontal end position of a window for memory access// 0
SetReg(0x38,0x00DB);    // Set the vertical start position for memory access
SetReg(0x39,0x0000);    // Set the vertical end opsition of a window for memory access// 0

SetReg(0x50,0x0001);    // Gamma Control (P34, OTM2201A Datasheet)
SetReg(0x51,0x0208);    // Gamma Control (P34, OTM2201A Datasheet)
SetReg(0x52,0x0805);    // Gamma Control (P34, OTM2201A Datasheet)
SetReg(0x53,0x0404);    // Gamma Control (P34, OTM2201A Datasheet)
SetReg(0x54,0x0c0c);    // Gamma Control (P34, OTM2201A Datasheet)
SetReg(0x55,0x000C);    // Gamma Control (P34, OTM2201A Datasheet)
SetReg(0x56,0x0100);    // Gamma Control (P34, OTM2201A Datasheet)
SetReg(0x57,0x0400);    // Gamma Control (P34, OTM2201A Datasheet)
SetReg(0x58,0x1108);    // Gamma Control (P34, OTM2201A Datasheet)
SetReg(0x59,0x050C);    // Gamma Control (P34, OTM2201A Datasheet)
SetReg(0x0F,0x0F01);    // Freq = 453kHz x 1.29// Oscillator on
SetReg(0x07,0x0012);    // FLM output disabled, Gate output Enabled, Normally black
SetReg(0x07,0x0017);    // FLM output disabled, Gate output Enabled, Normal display, gate on, vcom on, display on
DelayMs(500);

}


/*********************************************************************
* Function: void PutPixel(SHORT x, SHORT y)
*
* PreCondition: none
*
* Input: x,y - pixel coordinates
*
* Output: none
*
* Side Effects: none
*
* Overview: puts pixel
*
* Note: none
*
********************************************************************/
void PutPixel(SHORT x, SHORT y)
{

 WORD address;

    DisplayEnable();

    #if (DISP_ORIENTATION == 90)
   {
     SHORT tempx;

     tempx = x;
     x =  y;
     y = GetMaxX() - tempx;
   }
   #endif

    address = x + y *(256);                         //Starting Address of Pixel

    SetReg(0x20,(address & 0x00FF));                //Give Device Address for Pixel
    SetReg(0x21,(address & 0xFF00) >> 8);           //Give Device Address for Pixel

    SetReg(0x22,_color);                              //Give Device Pixel Color

    DisplayDisable();

}

/*********************************************************************
* Function: WORD GetPixel(SHORT x, SHORT y)
*
* PreCondition: none
*
* Input: x,y - pixel coordinates
*
* Output: pixel color
*
* Side Effects: none
*
* Overview: returns pixel color at x,y position
*
* Note: none
*
********************************************************************/
WORD GetPixel(SHORT x, SHORT y)
{
/* Not Creaetd Yet */
	return 0;
}

#ifdef USE_DRV_LINE

/*********************************************************************
* Function: WORD Line2D(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
*
* PreCondition: none
*
* Input: x1,y1 - starting coordinates, x2,y2 - ending coordinates
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: draws solid line
*
* Note: none
*
********************************************************************/
static WORD Line2D(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
{

/* Not Created Yet */

}

/*********************************************************************
* Function: WORD Line(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
*
* PreCondition: none
*
* Input: x1,y1 - starting coordinates, x2,y2 - ending coordinates
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: draws line
*
* Note: none
*
********************************************************************/
WORD Line(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
{
/* Not Created Yet */
}

#endif
#ifdef USE_DRV_BAR

/*********************************************************************
* Function: WORD Bar(SHORT left, SHORT top, SHORT right, SHORT bottom)
*
* PreCondition: none
*
* Input: left,top - top left corner coordinates,
*        right,bottom - bottom right corner coordinates
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: draws rectangle filled with current color
*
* Note: none
*
********************************************************************/
WORD Bar(SHORT left, SHORT top, SHORT right, SHORT bottom)
{
/* Not Created Yet */
}

#endif

#ifdef USE_DRV_CIRCLE

/***************************************************************************
* Function: WORD Circle(SHORT x, SHORT y, SHORT radius)
*
* Overview: This macro draws a circle with the given center and radius.
*
* Input: x - Center x position.
*        y - Center y position.
*        radius - the radius of the circle.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
********************************************************************/
WORD Circle(SHORT x, SHORT y, SHORT radius)
{

}

 #endif
 #ifdef USE_DRV_CLEARDEVICE

/*********************************************************************
* Function: void ClearDevice(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: clears screen with current color
*
* Note: none
*
********************************************************************/
void ClearDevice(void)
{
  int i;
  WORD address;

    DisplayEnable();

    address = 0;                         //Starting Address of Pixel

    SetReg(0x20,(address & 0x00FF));                //Give Device Address for Pixel
    SetReg(0x21,(address & 0xFF00) >> 8);           //Give Device Address for Pixel

    for(i=0; i<(DISP_HOR_RESOLUTION*DISP_VER_RESOLUTION);i++)
    {
         SetReg(0x22,_color);                            //Give Device Pixel Color
    }

    DisplayDisable();

}

#endif
#ifdef USE_DRV_PUTIMAGE
/*********************************************************************
* Function: WORD PutImage(SHORT left, SHORT top, void* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the image is not yet completely drawn.
*         - Returns 1 when the image is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: outputs image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
WORD PutImage(SHORT left, SHORT top, void *bitmap, BYTE stretch)
{

}

#ifdef USE_BITMAP_FLASH

/*********************************************************************
* Function: void PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE *bitmap, BYTE stretch)
{

}

/*********************************************************************
* Function: void PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs 16 color image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE *bitmap, BYTE stretch)
{

}

/*********************************************************************
* Function: void PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs 256 color image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE *bitmap, BYTE stretch)
{

}

/*********************************************************************
* Function: void PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs hicolor image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE *bitmap, BYTE stretch)
{

}

#endif
#ifdef USE_BITMAP_EXTERNAL

/*********************************************************************
* Function: void PutImage1BPPExt(SHORT left, SHORT top, void* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
void PutImage1BPPExt(SHORT left, SHORT top, void *bitmap, BYTE stretch)
{

}

/*********************************************************************
* Function: void PutImage4BPPExt(SHORT left, SHORT top, void* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
void PutImage4BPPExt(SHORT left, SHORT top, void *bitmap, BYTE stretch)
{

}

/*********************************************************************
* Function: void PutImage8BPPExt(SHORT left, SHORT top, void* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
void PutImage8BPPExt(SHORT left, SHORT top, void *bitmap, BYTE stretch)
{

}

/*********************************************************************
* Function: void PutImage16BPPExt(SHORT left, SHORT top, void* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
void PutImage16BPPExt(SHORT left, SHORT top, void *bitmap, BYTE stretch)
{

}
#endif
#endif
#ifdef USE_PALETTE

/*********************************************************************
* Function: void PaletteInit(void)
*
* Overview: Initializes the CLUT.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: Drawing mode will change to support palettes
*
********************************************************************/
void PaletteInit(void)
{

}

/*********************************************************************
* Function: BYTE SetPaletteBpp(BYTE bpp)
*
* Overview: Sets the CLUT's number of valid entries.
*
* PreCondition: PaletteInit() must be called before.
*
* Input: bpp -> Bits per pixel
*
* Output: Status: Zero -> Success, Non-zero -> Error.
*
* Side Effects: Drawing mode will change to support palettes
*
********************************************************************/
BYTE SetPaletteBpp(BYTE bpp)
{

}

/*********************************************************************
* Function: void EnablePalette(void)
*
* Overview: Enables the Palette mode
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects:
*
********************************************************************/
void EnablePalette(void)
{
 //   SetPaletteBpp(PaletteBpp);
}

/*********************************************************************
* Function: void DisablePalette(void)
*
* Overview: Disables the Palette mode
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects:
*
********************************************************************/
void DisablePalette(void)
{
//    SetPaletteBpp(16);
}

/*********************************************************************
* Function: BYTE IsPaletteEnabled(void)
*
* Overview: Returns if the Palette mode is enabled or not
*
* PreCondition: none
*
* Input: none
*
* Output: Enabled -> 1, Disabled -> 0
*
* Side Effects:
*
********************************************************************/
BYTE IsPaletteEnabled(void)
{
//    return ((PaletteBpp == 16) ? 0 : 1);
}

/*********************************************************************
* Function: void StartVBlankInterrupt(void)
*
* Overview: Sets up the Vertical Blanking Interrupt
*
* PreCondition: Interrupts must be enabled
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void StartVBlankInterrupt(void)
{

    /* We don't use Vertical Blanking Interrupt in SSD1926 */
 /*
    if(pPendingPalette != NULL)
    {
        blPaletteChangeError = SetPalette(pPendingPalette, PendingStartEntry, PendingLength);
        if(!blPaletteChangeError)
        {
            _palette = pPendingPalette;
        }
    }
*/
}

/*********************************************************************
* Function: BYTE SetPaletteFlash(PALETTE_ENTRY *pPaletteEntry, WORD startEntry, WORD length)
*
* Overview: Loads the palettes from the flash immediately.
*
* PreCondition: PaletteInit() must be called before.
*
* Input: pPaletteEntry   - Pointer to the palette table in ROM
*        startEntry      - Start entry to load (inclusive)
*        length          - Number of entries
*
* Output: Status: Zero -> Success, Non-zero -> Error.
*
* Side Effects: There may be a slight flicker when the Palette entries
*               are getting loaded one by one.
*
********************************************************************/
BYTE SetPaletteFlash(PALETTE_ENTRY *pPaletteEntry, WORD startEntry, WORD length)
{

}

#endif

void SetActivePage(BYTE pagenumber)
{
}

void SetVisualPage(BYTE pagenumber)
{
}

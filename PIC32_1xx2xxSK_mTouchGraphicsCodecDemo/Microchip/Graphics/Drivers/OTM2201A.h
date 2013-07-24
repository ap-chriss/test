/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  OTM2201A LCD controllers driver
 *  to be used with Audio Development Kit
 *****************************************************************************
 * FileName:        OTM2201A.h
 * Dependencies:    p24Fxxxx.h or plib.h
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
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
 * Adam Folts           11/01/10
 *****************************************************************************/
#ifndef _OTM2201A_H
    #define _OTM2201A_H

    #if defined(__PIC32MX__)
        #include <plib.h>
    #endif
        #define PMDIN1  PMDIN
    
    #include "GraphicsConfig.h"
    #include "GenericTypeDefs.h"

/*********************************************************************
* Overview: Additional hardware-accelerated functions can be implemented
*           in the driver. These definitions exclude the PutPixel()-based
*           functions in the primitives layer (Primitive.c file) from compilation.
*********************************************************************/
 //  #define COLOR_24BPP
// Define this to implement Font related functions in the driver.
//#define USE_DRV_FONT
// Define this to implement Line function in the driver.
    #ifndef USE_PALETTE
//        #define USE_DRV_LINE
    #endif

// Define this to implement Circle function in the driver.
//   #define USE_DRV_CIRCLE - //Don't use this. The driver support for this is obsolete.

// Define this to implement FillCircle function in the driver.
//#define USE_DRV_FILLCIRCLE
// Define this to implement Bar function in the driver.
//    #define USE_DRV_BAR

// Define this to implement ClearDevice function in the driver.
    #define USE_DRV_CLEARDEVICE

// Define this to implement PutImage function in the driver.
//    #define USE_DRV_PUTIMAGE

/*********************************************************************
* PARAMETERS VALIDATION
*********************************************************************/
    #if COLOR_DEPTH != 16
        #error This driver supports 16 BPP only.
    #endif
    #if (DISP_HOR_RESOLUTION % 8) != 0
   //     #error Horizontal resolution must be divisible by 8.
    #endif
    #if (DISP_ORIENTATION != 0) && (DISP_ORIENTATION != 180) && (DISP_ORIENTATION != 90) && (DISP_ORIENTATION != 270)
        #error The display orientation selected is not supported. It can be only 0,90,180 or 270.
    #endif

/*********************************************************************
* Overview: Clipping region control codes to be used with SetClip(...)
*           function. 
*********************************************************************/
    #define CLIP_DISABLE    0   // Disables clipping.
    #define CLIP_ENABLE     1   // Enables clipping.

/*********************************************************************
* Overview: Some basic colors definitions.
*********************************************************************/
    #ifdef USE_PALETTE
    
        #include "PaletteColorDefines.h"
    
    #else
 
 
	#ifdef COLOR_24BPP
	
        #define BLACK           (DWORD_VAL)RGB888CONVERT(0, 0, 0)
        #define BRIGHTBLUE      (DWORD_VAL)RGB888CONVERT(0, 0, 255)
        #define BRIGHTGREEN     (DWORD_VAL)RGB888CONVERT(0, 255, 0)
        #define BRIGHTCYAN      (DWORD_VAL)RGB888CONVERT(0, 255, 255)
        #define BRIGHTRED       (DWORD_VAL)RGB888CONVERT(255, 0, 0)
        #define BRIGHTMAGENTA   (DWORD_VAL)RGB888CONVERT(255, 0, 255)
        #define BRIGHTYELLOW    (DWORD_VAL)RGB888CONVERT(255, 255, 0)
        #define BLUE            (DWORD_VAL)RGB888CONVERT(0, 0, 128)
        #define GREEN           (DWORD_VAL)RGB888CONVERT(0, 128, 0)
        #define CYAN            (DWORD_VAL)RGB888CONVERT(0, 128, 128)
        #define RED             (DWORD_VAL)RGB888CONVERT(128, 0, 0)
        #define MAGENTA         (DWORD_VAL)RGB888CONVERT(128, 0, 128)
        #define BROWN           (DWORD_VAL)RGB888CONVERT(255, 128, 0)
        #define LIGHTGRAY       (DWORD_VAL)RGB888CONVERT(128, 128, 128)
        #define DARKGRAY        (DWORD_VAL)RGB888CONVERT(64, 64, 64)
        #define LIGHTBLUE       (DWORD_VAL)RGB888CONVERT(128, 128, 255)
        #define LIGHTGREEN      (DWORD_VAL)RGB888CONVERT(128, 255, 128)
        #define LIGHTCYAN       (DWORD_VAL)RGB888CONVERT(128, 255, 255)
        #define LIGHTRED        (DWORD_VAL)RGB888CONVERT(255, 128, 128)
        #define LIGHTMAGENTA    (DWORD_VAL)RGB888CONVERT(255, 128, 255)
        #define YELLOW          (DWORD_VAL)RGB888CONVERT(255, 255, 128)
        #define WHITE           (DWORD_VAL)RGB888CONVERT(255, 255, 255)
        #define GRAY0           (DWORD_VAL)RGB888CONVERT(224, 224, 224)
        #define GRAY1           (DWORD_VAL)RGB888CONVERT(192, 192, 192)
        #define GRAY2           (DWORD_VAL)RGB888CONVERT(160, 160, 160)
        #define GRAY3           (DWORD_VAL)RGB888CONVERT(128, 128, 128)
        #define GRAY4           (DWORD_VAL)RGB888CONVERT(96, 96, 96)
        #define GRAY5           (DWORD_VAL)RGB888CONVERT(64, 64, 64)
        #define GRAY6           (DWORD_VAL)RGB888CONVERT(32, 32, 32)
	
	#else
		
		#define BLACK           RGB565CONVERT(0, 0, 0)
        #define BRIGHTBLUE      RGB565CONVERT(0, 0, 255)
        #define BRIGHTGREEN     RGB565CONVERT(0, 255, 0)
        #define BRIGHTCYAN      RGB565CONVERT(0, 255, 255)
        #define BRIGHTRED       RGB565CONVERT(255, 0, 0)
        #define BRIGHTMAGENTA   RGB565CONVERT(255, 0, 255)
        #define BRIGHTYELLOW    RGB565CONVERT(255, 255, 0)
        #define BLUE            RGB565CONVERT(0, 0, 128)
        #define GREEN           RGB565CONVERT(0, 128, 0)
        #define CYAN            RGB565CONVERT(0, 128, 128)
        #define RED             RGB565CONVERT(128, 0, 0)
        #define MAGENTA         RGB565CONVERT(128, 0, 128)
        #define BROWN           RGB565CONVERT(255, 128, 0)
        #define LIGHTGRAY       RGB565CONVERT(128, 128, 128)
        #define DARKGRAY        RGB565CONVERT(64, 64, 64)
        #define LIGHTBLUE       RGB565CONVERT(128, 128, 255)
        #define LIGHTGREEN      RGB565CONVERT(128, 255, 128)
        #define LIGHTCYAN       RGB565CONVERT(128, 255, 255)
        #define LIGHTRED        RGB565CONVERT(255, 128, 128)
        #define LIGHTMAGENTA    RGB565CONVERT(255, 128, 255)
        #define YELLOW          RGB565CONVERT(255, 255, 128)
        #define WHITE           RGB565CONVERT(255, 255, 255)
        #define GRAY0           RGB565CONVERT(224, 224, 224)
        #define GRAY1           RGB565CONVERT(192, 192, 192)
        #define GRAY2           RGB565CONVERT(160, 160, 160)
        #define GRAY3           RGB565CONVERT(128, 128, 128)
        #define GRAY4           RGB565CONVERT(96, 96, 96)
        #define GRAY5           RGB565CONVERT(64, 64, 64)
        #define GRAY6           RGB565CONVERT(32, 32, 32)
		
	#endif

    #endif

/*********************************************************************
* Overview: Extern for the current color
*********************************************************************/
#ifdef COLOR_24BPP
extern DWORD_VAL     _color;
#else
extern WORD     _color;
#endif

/*********************************************************************
* Overview: Clipping region control and border settings.
*********************************************************************/

// Clipping region enable control
extern SHORT    _clipRgn;

// Left clipping region border
extern SHORT    _clipLeft;

// Top clipping region border
extern SHORT    _clipTop;

// Right clipping region border
extern SHORT    _clipRight;

// Bottom clipping region border
extern SHORT    _clipBottom;

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
BYTE            GetReg(WORD index);

void SetActivePage(BYTE pagenumber);
void SetVisualPage(BYTE pagenumber);
/*********************************************************************
* Function:  void ResetDevice()
*
* Overview: Initializes LCD module.
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
void            ResetDevice(void);

/*********************************************************************
* Function:  void SetBacklightLevel()
*
* Overview: Sets LCD Backlight Level
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
void SetBacklightLevel(int level);


/*********************************************************************
* Macros:  GetMaxX()
*
* Overview: Returns maximum horizontal coordinate.
*
* PreCondition: none
*
* Input: none
*
* Output: Maximum horizontal coordinate.
*
*
* Example:
*   <CODE> 
*
*	// Create a window that will occupy the whole screen.
*	WndCreate(0xFF,				    	// ID
*             0,0,
*			  GetMaxX(),GetMaxY(),		// dimension
*             WND_DRAW,					// will be dislayed after creation
*             (void*)&mchpIcon,         // use icon used
*             pText,	   				// set to text pointed to by pText
*             NULL);					// use default scheme 
*
*  </CODE>
*
* Side Effects: none
*
********************************************************************/
    #if (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 270)
        #define GetMaxX()   (DISP_VER_RESOLUTION - 1)
    #elif (DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180)
        #define GetMaxX()   (DISP_HOR_RESOLUTION - 1)
    #endif

/*********************************************************************
* Macros:  GetMaxY()
*
* Overview: Returns maximum vertical coordinate.
*
* PreCondition: none
*
* Input: none
*
* Output: Maximum vertical coordinate.
*
* Example: (see GetMaxX()) example.
*
* Side Effects: none
*
********************************************************************/
    #if (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 270)
        #define GetMaxY()   (DISP_HOR_RESOLUTION - 1)
    #elif (DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180)
        #define GetMaxY()   (DISP_VER_RESOLUTION - 1)
    #endif

/*********************************************************************
* Macros:  SetColor(color)
*
* Overview: Sets current drawing color.
*
* PreCondition: none
*
* Input: color - Color coded in 5:6:5 RGB format.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define SetColor(color) _color = color

/*********************************************************************
* Macros:  GetColor()
*
* Overview: Returns current drawing color.
*
* PreCondition: none
*
* Input: none
*
* Output: Color coded in 5:6:5 RGB format.
*
* Side Effects: none
*
********************************************************************/
    #define GetColor()  _color


/*********************************************************************
* Function: void PutPixel(SHORT x, SHORT y)
*
* Overview: Puts pixel with the given x,y coordinate position.
*
* PreCondition: none
*
* Input: x - x position of the pixel.
*		 y - y position of the pixel.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void PutPixel(SHORT x, SHORT y);

/*********************************************************************
* Function: WORD GetPixel(SHORT x, SHORT y)
*
* Overview: Returns pixel color at the given x,y coordinate position.
*
* PreCondition: none
*
* Input: x - x position of the pixel.
*		 y - y position of the pixel.
*
* Output: pixel color
*
* Side Effects: none
*
********************************************************************/
WORD    GetPixel(SHORT x, SHORT y);

/*********************************************************************
* Macros: SetClipRgn(left, top, right, bottom)
*
* Overview: Sets clipping region.
*
* PreCondition: none
*
* Input: left - Defines the left clipping region border.
*		 top - Defines the top clipping region border.
*		 right - Defines the right clipping region border.
*	     bottom - Defines the bottom clipping region border.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define SetClipRgn(left, top, right, bottom) \
    _clipLeft = left;                            \
    _clipTop = top;                              \
    _clipRight = right;                          \
    _clipBottom = bottom;

/*********************************************************************
* Macros: GetClipLeft()
*
* Overview: Returns left clipping border.
*
* PreCondition: none
*
* Input: none
*
* Output: Left clipping border.
*
* Side Effects: none
*
********************************************************************/
    #define GetClipLeft()   _clipLeft

/*********************************************************************
* Macros: GetClipRight()
*
* Overview: Returns right clipping border.
*
* PreCondition: none
*
* Input: none
*
* Output: Right clipping border.
*
* Side Effects: none
*
********************************************************************/
    #define GetClipRight()  _clipRight

/*********************************************************************
* Macros: GetClipTop()
*
* Overview: Returns top clipping border.
*
* PreCondition: none
*
* Input: none
*
* Output: Top clipping border.
*
* Side Effects: none
*
********************************************************************/
    #define GetClipTop()    _clipTop

/*********************************************************************
* Macros: GetClipBottom()
*
* Overview: Returns bottom clipping border.
*
* PreCondition: none
*
* Input: none
*
* Output: Bottom clipping border.
*
* Side Effects: none
*
********************************************************************/
    #define GetClipBottom() _clipBottom

/*********************************************************************
* Macros: SetClip(control)
*
* Overview: Enables/disables clipping.
*
* PreCondition: none
*
* Input: control - Enables or disables the clipping.
*			- 0: Disable clipping
*			- 1: Enable clipping
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define SetClip(control)    _clipRgn = control;

/*********************************************************************
* Macros: IsDeviceBusy()
*
* Overview: Returns non-zero if LCD controller is busy 
*           (previous drawing operation is not completed).
*
* PreCondition: none
*
* Input: none
*
* Output: Busy status.
*
* Side Effects: none
*
********************************************************************/
    #define IsDeviceBusy()  (0)	//((GetReg(REG_2D_220) & 0x01) == 0)

/*********************************************************************
* Overview: SSD1926 registers definitions.
*********************************************************************/	
	#define REG00_PROD_CODE        0x00     // Product Code Register [READONLY]
	#define REG02_READBACK         0x02     // Configuration Readback Register [READONLY]
	#define REG04_PLL_DDIVIDER     0x04     // PLL D-Divider Register
	#define REG06_PLL_0            0x06     // PLL Setting Register 0
	#define REG08_PLL_1            0x08     // PLL Setting Register 1
	#define REG0A_PLL_2            0x0A     // PLL Setting Register 2
	#define REG0C_PLL_NDIVIDER     0x0C     // PLL N-Divider Register
	#define REG0E_SS_CONTROL_0     0x0E     // SS Control Register 0
	#define REG10_SS_CONTROL_1     0x10     // SS Control Register 1
	#define REG12_CLK_SRC_SELECT   0x12     // Clock Source Select Register
	#define REG14_LCD_PANEL_TYPE   0x14     // LCD Panel Type Register
	#define REG16_HDISP_WIDTH      0x16     // Horizontal Display Width Register
	#define REG18_HNDP_PERIOD      0x18     // Horizontal Non-Display Period Register
	#define REG1A_VDISP_HEIGHT_0   0x1A     // Vertical Display Height Register 0
	#define REG1C_VDISP_HEIGHT_1   0x1C     // Vertical Display Height Register 1
	#define REG1E_VNDP_PERIOD      0x1E     // Vertical Non-Display Period Register
	#define REG20_PHS_PULSE_WIDTH  0x20     // PHS Pulse Width (HSW) Register
	#define REG22_PHS_PULSE_START  0x22     // PHS Pulse Start Position (HPS) Register
	#define REG24_PVS_PULSE_WIDTH  0x24     // PVS Pulse Width (VSW) Register
	#define REG26_PVS_PULSE_START  0x26     // PVS Pulse Start Position (VPS) Register
	#define REG28_PCLK_POLARITY    0x28     // PCLK Polarity Register
	#define REG2A_DSP_MODE         0x2A     // Display Mode Register
	#define REG2C_PIP1_DSP_SA_0    0x2C     // PIP1 Display Start Address Register 0
	#define REG2E_PIP1_DSP_SA_1    0x2E     // PIP1 Display Start Address Register 1
	#define REG30_PIP1_DSP_SA_2    0x30     // PIP1 Display Start Address Register 2
	#define REG32_PIP1_WIN_X_SP    0x32     // PIP1 Window X Start Position Register
	#define REG34_PIP1_WIN_Y_SP_0  0x34     // PIP1 Window Y Start Position Register 0
	#define REG36_PIP1_WIN_Y_SP_1  0x36     // PIP1 Window Y Start Position Register 1
	#define REG38_PIP1_WIN_X_EP    0x38     // PIP1 Window X End Position Register
	#define REG3A_PIP1_WIN_Y_EP_0  0x3A     // PIP1 Window Y End Position Register 0
	#define REG3C_PIP1_WIN_Y_EP_1  0x3C     // PIP1 Window Y End Position Register 1
	#define REG3E_PIP2_DSP_SA_0    0x3E     // PIP2 Display Start Address Register 0
	#define REG40_PIP2_DSP_SA_1    0x40     // PIP2 Display Start Address Register 1
	#define REG42_PIP2_DSP_SA_2    0x42     // PIP2 Display Start Address Register 2
	#define REG44_PIP2_WIN_X_SP    0x44     // PIP2 Window X Start Position Register
	#define REG46_PIP2_WIN_Y_SP_0  0x46     // PIP2 Window Y Start Position Register 0
	#define REG48_PIP2_WIN_Y_SP_1  0x48     // PIP2 Window Y Start Position Register 1
	#define REG4A_PIP2_WIN_X_EP    0x4A     // PIP2 Window X End Position Register
	#define REG4C_PIP2_WIN_Y_EP_0  0x4C     // PIP2 Window Y End Position Register 0
	#define REG4E_PIP2_WIN_Y_EP_1  0x4E     // PIP2 Window Y End Position Register 1
	#define REG50_DISPLAY_CONTROL  0x50     // Display Control Register [WRITEONLY]
	#define REG52_INPUT_MODE       0x52     // Input Mode Register
	#define REG54_TRANSP_KEY_RED   0x54     // Transparency Key Color Red Register
	#define REG56_TRANSP_KEY_GREEN 0x56     // Transparency Key Color Green Register
	#define REG58_TRANSP_KEY_BLUE  0x58     // Transparency Key Color Blue Register
	#define REG5A_WRITE_WIN_X_SP   0x5A     // Write Window X Start Position Register
	#define REG5C_WRITE_WIN_Y_SP_0 0x5C     // Write Window Start Position Register 0
	#define REG5E_WRITE_WIN_Y_SP_1 0x5E     // Write Window Start Position Register 1
	#define REG60_WRITE_WIN_X_EP   0x60     // Write Window X End Position Register
	#define REG62_WRITE_WIN_Y_EP_0 0x62     // Write Window Y End Position Register 0
	#define REG64_WRITE_WIN_Y_EP_1 0x64     // Write Window Y End Position Register 1
	#define REG66_MEM_DATA_PORT_0  0x66     // Memory Data Port Register 0 [WRITEONLY]
	#define REG67_MEM_DATA_PORT_1  0x67     // Memory Data Port Register 1 [WRITEONLY]
	#define REG68_POWER_SAVE       0x68     // Power Save Register
	#define REG6A_N_DISP_PER_CTRS  0x6A     // Non-Display Period Control/Status Register
	#define REG6C_GPO_0            0x6C     // General Purpose Output Register 0
	#define REG6E_GPO_1            0x6E     // General Purpose Output Register 1
	#define REG70_PWM_CONTROL      0x70     // PWM Control Register
	#define REG72_PWM_HIGH_DC_0    0x72     // PWM High Duty Cycle Register 0
	#define REG74_PWM_HIGH_DC_1    0x74     // PWM High Duty Cycle Register 1
	#define REG76_PWM_HIGH_DC_2    0x76     // PWM High Duty Cycle Register 2
	#define REG78_PWM_HIGH_DC_3    0x78     // PWM High Duty Cycle Register 3
	#define REG7A_PWM_LOW_DC_0     0x7A     // PWM Low Duty Cycle Register 0
	#define REG7C_PWM_LOW_DC_1     0x7C     // PWM Low Duty Cycle Register 1
	#define REG7E_PWM_LOW_DC_2     0x7E     // PWM Low Duty Cycle Register 2
	#define REG80_PWM_LOW_DC_3     0x80     // PWM Low Duty Cycle Register 3
	#define REG82_SDRAM_CONTROL_0  0x82     // SDRAM Control Register 0
	#define REG84_SDRAM_STATUS_0   0x84     // SDRAM Status Register 0 [WRITEONLY]
	#define REG86_SDRAM_STATUS_1   0x86     // SDRAM Status Register 1 [READONLY]
	#define REG88_SDRAM_MRS_VAL_0  0x88     // SDRAM MRS Value Register 0
	#define REG8A_SDRAM_MRS_VAL_1  0x8A     // SDRAM MRS Value Register 1
	#define REG8C_SDRAM_RFS_CNT_0  0x8C     // SDRAM Refresh Counter Register 0
	#define REG8E_SDRAM_RFS_CNT_1  0x8E     // SDRAM Refresh Counter Register 1
	#define REG90_SDRAM_BUF_SIZE   0x90     // SDRAM Write Buffer Memory Size Register
	#define REG92_SDRAM_DEBUG      0x92     // SDRAM Debug Register
	#define REG94_ALP_CONTROL      0x94     // Alpha-Blend Control Register [WRITEONLY]
	#define REG96_ALP_STATUS       0x96     // Alpha-Blend Status Register [READONLY]
	#define REG98_ALP_HR_SIZE      0x98     // Alpha-Blend Horizontal Size Register
	#define REG9A_ALP_VR_SIZE_0    0x9A     // Alpha-Blend Vertical Size Register 0
	#define REG9C_ALP_VR_SIZE_1    0x9C     // Alpha-Blend Vertical Size Register 1
	#define REG9E_ALP_VALUE        0x9E     // Alpha-Blend Value Register
	#define REGA0_ALP_IN_IMG1_SA_0 0xA0     // Alpha-Blend Input Image 1 Start Address Register 0
	#define REGA2_ALP_IN_IMG1_SA_1 0xA2     // Alpha-Blend Input Image 1 Start Address Register 1
	#define REGA4_ALP_IN_IMG1_SA_2 0xA4     // Alpha-Blend Input Image 1 Start Address Register 2
	#define REGA6_ALP_IN_IMG2_SA_0 0xA6     // Alpha-Blend Input Image 2 Start Address Register 0
	#define REGA8_ALP_IN_IMG2_SA_1 0xA8     // Alpha-Blend Input Image 2 Start Address Register 1
	#define REGAA_ALP_IN_IMG2_SA_2 0xAA     // Alpha-Blend Input Image 2 Start Address Register 2
	#define REGAC_ALP_OUT_IMG_SA_0 0xAC     // Alpha-Blend Output Image Start Address Register 0
	#define REGAE_ALP_OUT_IMG_SA_1 0xAE     // Alpha-Blend Output Image Start Address Register 1
	#define REGB0_ALP_OUT_IMG_SA_2 0xB0     // Alpha-Blend Output Image Start Address Register 2
	#define REGB2_INTERRUPT_CTRL   0xB2     // Interrupt Control Register
	#define REGB4_INTERRUPT_STAT   0xB4     // Interrupt Status Register [READONLY]
	#define REGB6_INTERRUPT_CLEAR  0xB6     // Interrupt Clear Register [WRITEONLY]

#endif

/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Main header file for the display driver
 *****************************************************************************
 * FileName:        DisplayDriver.h
 * Dependencies:    p24Fxxxx.h or plib.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
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
 * Anton Alkhimenok     09/04/08
 *****************************************************************************/
#ifndef _DISPLAYDRIVER_H
    #define _DISPLAYDRIVER_H

    #if (DISPLAY_CONTROLLER == CUSTOM_CONTROLLER)
        #include "CustomDisplayDriver.h"

    #elif (DISPLAY_CONTROLLER == MCHP_DA210)
        #include "MicrochipGraphicsModule.h"

    #elif (DISPLAY_CONTROLLER == SSD1906)
        #include "SSD1906.h"

    #elif (DISPLAY_CONTROLLER == SSD1926)
        #include "SSD1926.h"

    #elif (DISPLAY_CONTROLLER == OTM2201A)
        #include "OTM2201A.h"

    #elif (DISPLAY_CONTROLLER == S1D13517)
        #include "S1D13517.h"

    #elif (DISPLAY_CONTROLLER == S6D0129)
        #include "drvTFT001.h"

    #elif (DISPLAY_CONTROLLER == S6D0139)
        #include "drvTFT001.h"

    #elif (DISPLAY_CONTROLLER == LGDP4531)
        #include "drvTFT001.h"

    #elif (DISPLAY_CONTROLLER == R61505)
        #include "drvTFT001.h"

    #elif (DISPLAY_CONTROLLER == SPFD5408)
        #include "drvTFT001.h"

    #elif (DISPLAY_CONTROLLER == SSD1339)
        #include "SSD1339.h"

    #elif (DISPLAY_CONTROLLER == ST7529)
        #include "ST7529.h"

    #elif (DISPLAY_CONTROLLER == SH1101A)
        #include "SH1101A_SSD1303.h"

    #elif (DISPLAY_CONTROLLER == SSD1303)
        #include "SH1101A_SSD1303.h"

    #elif (DISPLAY_CONTROLLER == HIT1270)
        #include "HIT1270.h"

    #elif (DISPLAY_CONTROLLER == UC1610)
        #include "UC1610.h"

    #elif (DISPLAY_CONTROLLER == ILI9320)
        #include "drvTFT001.h"

    #elif (DISPLAY_CONTROLLER == HX8347A) || (DISPLAY_CONTROLLER == HX8347D)
        #include "HX8347.h"

    #elif (DISPLAY_CONTROLLER == SSD1289)
        #include "drvTFT002.h"

    #elif (DISPLAY_CONTROLLER == NO_CONTROLLER_DEFINED)
    #elif (DISPLAY_CONTROLLER == R61580)
        #include "drvTFT001.h"

    #elif (DISPLAY_CONTROLLER == SSD2119)
        #include "drvTFT002.h"

    #else
        #error GRAPHICS CONTROLLER IS NOT SUPPORTED
    #endif
#endif

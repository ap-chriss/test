/*********************************************************************
 *
 *  Hardware specific definitions
 *
 *********************************************************************
 * FileName:        HardwareProfile_xxx.h
 * Dependencies:    None
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:        Microchip C32 v1.00 or higher
 *                  Microchip C30 v3.01 or higher
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright © 2002-2009 Microchip Technology Inc.  All rights
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and
 * distribute:
 * (i)  the Software when embedded on a Microchip microcontroller or
 *      digital signal controller product (“Device”) which is
 *      integrated into Licensee’s product; or
 * (ii) ONLY the Software driver source files ENC28J60.c and
 *      ENC28J60.h ported to a non-Microchip device used in
 *      conjunction with a Microchip ethernet controller for the
 *      sole purpose of interfacing with the ethernet controller.
 *
 * You should refer to the license agreement accompanying this
 * Software for additional information regarding your rights and
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Howard Schlunder     10/03/06    Original, copied from Compiler.h
 * Jayanth Murthy       06/25/09    dsPIC & PIC24H support
 * Pradeep Budagutta    15 Sep 2009 Added PIC24FJ256DA210 Development Board Support
 * PAT                  04/07/10    Removed GRAPHICS_HARDWARE_PLATFORM
 *                                  Added MMB support
 ********************************************************************/
#ifndef __HARDWARE_PROFILE_GFX_ADK_H
    #define __HARDWARE_PROFILE_GFX_ADK_H

/* ################################################################## */
/*********************************************************************
* START OF GRAPHICS RELATED MACROS
********************************************************************/
/* ################################################################## */

/*********************************************************************
* AUTO GENERATED CODE
********************************************************************/

//Auto Generated Code
#define GFX_PICTAIL_V3E
#define EXPLORER_16
#define USE_8BIT_PMP
#define PIC32_GP_SK_DM320001
#define DISPLAY_CONTROLLER OTM2201A
#define DISPLAY_PANEL PH480272T_005_I11Q
#define COLOR_DEPTH 16
//End Auto Generated Code


/*********************************************************************
* END OF AUTO GENERATED CODE
********************************************************************/

#ifdef _GRAPHICS_H

/*********************************************************************
* ERROR CHECKING
*********************************************************************/
#if !defined (GFX_PICTAIL_V3E) && !defined (GFX_PICTAIL_V2) && !defined (PIC24FJ256DA210_DEV_BOARD) && !defined(MULTI_MEDIA_BOARD_DM00123)
    #error "Error: Graphics Hardware Platform is not defined! One of the three (GFX_PICTAIL_V3, GFX_PICTAIL_V2, PIC24FJ256DA210_DEV_BOARD) must be defined."
#endif

#if defined (__PIC24FJ128GA010__) || defined (__PIC24FJ256GA110__) ||   \
    defined (__PIC24FJ256GB110__) || defined (__dsPIC33F__)        ||   \
    defined (__PIC24HJ128GP504__)
    #if defined (USE_16BIT_PMP)
        #error "The device selected does not support 16 bit PMP interface."
    #endif
#endif

#ifndef DISPLAY_CONTROLLER
    #error "Error: DISPLAY_CONTROLLER not defined"
#endif

#if (defined (GFX_PICTAIL_V3E) || defined (PIC24FJ256DA210_DEV_BOARD)) && !defined(DISPLAY_PANEL)
    #error "Error: DISPLAY_PANEL not defined"
#endif

/*********************************************************************
* SETTINGS FOR DISPLAY DRIVER FEATURE for PIC24FJ256DA210  Family of
* display controller
*********************************************************************/

    #ifdef USE_DOUBLE_BUFFERING
        #ifdef GFX_DISPLAY_BUFFER_START_ADDRESS
            #undef GFX_DISPLAY_BUFFER_START_ADDRESS

            volatile extern  DWORD _drawbuffer;
            #define GFX_DISPLAY_BUFFER_START_ADDRESS _drawbuffer
        #endif

        #define GFX_BUFFER1 0x00020000ul
        #define GFX_BUFFER2 0x00050000ul

    #endif

/*********************************************************************
* DISPLAY PARALLEL INTERFACE
*********************************************************************/
    // EPMP is exclusive to GB210 devices and PMP to some devices
    #if defined (__PIC24FJ256DA210__)
        // EPMP is used by graphics controller
    #elif defined (__PIC24FJ256GB210__)
        #define USE_GFX_EPMP
    #else
        #define USE_GFX_PMP
    #endif

/*********************************************************************
* DISPLAY SETTINGS
********************************************************************/

// -----------------------------------
// For SMART DISPLAYS
// -----------------------------------

// Using LGDP4531 Display Controller
    #if (DISPLAY_CONTROLLER == LGDP4531)
        #define DISP_ORIENTATION    90
        #define DISP_HOR_RESOLUTION 240
        #define DISP_VER_RESOLUTION 320

        #define DISPLAY_PANEL -1
    #endif // #if (DISPLAY_CONTROLLER == LGDP4531)

// Using S6D0129 Display Controller
    #if (DISPLAY_CONTROLLER == S6D0129)
        #define DISP_ORIENTATION    90
        #define DISP_HOR_RESOLUTION 240
        #define DISP_VER_RESOLUTION 320
    #endif // #if (DISPLAY_CONTROLLER == S6D0129)

// -----------------------------------
// For SMART RGB GLASS
// -----------------------------------

// check if DISPLAY_PANEL is not defined set it to -1 to skip the rest
// of the display panel settings.
#if !defined (DISPLAY_PANEL)
    #define DISPLAY_PANEL -1
#endif

// Using TFT_G240320LTSW_118W_E, TFT2N0369_E, DT032TFT_TS, DT032TFT  Display Panel.
// These panels uses Solomon Systech SSD1289 as their controller.
    #if (DISPLAY_PANEL == TFT_G240320LTSW_118W_E)   || (DISPLAY_PANEL == TFT2N0369_E) ||        \
        (DISPLAY_PANEL == DT032TFT_TS)              || (DISPLAY_PANEL == DT032TFT)

        /* note: For SSD1289

            tHBP (horizontal back porch)= DISP_HOR_BACK_PORCH + DISP_HOR_PULSE_WIDTH
            tVBP (vertical back porch)  = DISP_VER_BACK_PORCH + DISP_VER_PULSE_WIDTH

            Horizontal Cycle (280) = tHBP + DISP_HOR_FRONT_PORCH + DISP_HOR_RESOLUTION
            Vertical Cycle (326)   = tVBP + DISP_VER_FRONT_PORCH + DISP_VER_RESOLUTION

            In summary, the DISP_HOR_PULSE_WIDTH should not exceed tHBP and
            the DISP_VER_PULSE_WIDTH should not exceed tVBP. See SSD1289 data sheet for
            details.
        */

        #define DISP_ORIENTATION        0
        #define DISP_HOR_RESOLUTION     240
        #define DISP_VER_RESOLUTION     320
        #define DISP_DATA_WIDTH         18
        #define DISP_INV_LSHIFT
        #define DISP_HOR_PULSE_WIDTH    25
        #define DISP_HOR_BACK_PORCH     5
        #define DISP_HOR_FRONT_PORCH    10
        #define DISP_VER_PULSE_WIDTH    4
        #define DISP_VER_BACK_PORCH     0
        #define DISP_VER_FRONT_PORCH    2

        #if (DISPLAY_CONTROLLER == MCHP_DA210)
            #define GFX_LCD_TYPE                        GFX_LCD_TFT
            #define GFX_DISPLAYENABLE_ENABLE
            #define GFX_HSYNC_ENABLE
            #define GFX_VSYNC_ENABLE
            #define GFX_DISPLAYPOWER_ENABLE
            #define GFX_CLOCK_POLARITY                  GFX_ACTIVE_HIGH
            #define GFX_DISPLAYENABLE_POLARITY          GFX_ACTIVE_HIGH
            #define GFX_HSYNC_POLARITY                  GFX_ACTIVE_LOW
            #define GFX_VSYNC_POLARITY                  GFX_ACTIVE_LOW
            #define GFX_DISPLAYPOWER_POLARITY           GFX_ACTIVE_HIGH
        #endif
    #endif // #if (DISPLAY_PANEL == TFT_G240320LTSW_118W_E) || (DISPLAY_PANEL == TFT2N0369_E) ||....

// Using TFT_G320240DTSW_69W_TP_E Display Panel
    #if (DISPLAY_PANEL == TFT_G320240DTSW_69W_TP_E) || (DISPLAY_PANEL == _35QVW0T)
        #define DISP_ORIENTATION        0
        #define DISP_HOR_RESOLUTION     320
        #define DISP_VER_RESOLUTION     240
        #define DISP_DATA_WIDTH         18
        #define DISP_HOR_PULSE_WIDTH    25
        #define DISP_HOR_BACK_PORCH     8
        #define DISP_HOR_FRONT_PORCH    8
        #define DISP_VER_PULSE_WIDTH    8
        #define DISP_VER_BACK_PORCH     7
        #define DISP_VER_FRONT_PORCH    5
    #endif // #if (DISPLAY_PANEL == TFT_G320240DTSW_69W_TP_E) || (DISPLAY_PANEL == _35QVW0T)

// Using PH480272T_005_I06Q Display Panel
    #if (DISPLAY_PANEL == PH480272T_005_I06Q)

        #define DISP_ORIENTATION        0
        #define DISP_HOR_RESOLUTION     480
        #define DISP_VER_RESOLUTION     272
        #define DISP_DATA_WIDTH         24
        #define DISP_HOR_PULSE_WIDTH    42
        #define DISP_HOR_BACK_PORCH     2
        #define DISP_HOR_FRONT_PORCH    2
        #define DISP_VER_PULSE_WIDTH    10
        #define DISP_VER_BACK_PORCH     2
        #define DISP_VER_FRONT_PORCH    2
        #if (DISPLAY_CONTROLLER == MCHP_DA210)
            #define GFX_LCD_TYPE                        GFX_LCD_TFT
            #define GFX_DISPLAYENABLE_ENABLE
            #define GFX_HSYNC_ENABLE
            #define GFX_VSYNC_ENABLE
            #define GFX_DISPLAYPOWER_ENABLE
            #define GFX_CLOCK_POLARITY                  GFX_ACTIVE_LOW
            #define GFX_DISPLAYENABLE_POLARITY          GFX_ACTIVE_HIGH
            #define GFX_HSYNC_POLARITY                  GFX_ACTIVE_LOW
            #define GFX_VSYNC_POLARITY                  GFX_ACTIVE_LOW
            #define GFX_DISPLAYPOWER_POLARITY           GFX_ACTIVE_HIGH
        #endif
    #endif // #if (DISPLAY_PANEL == PH480272T_005_I06Q)

// Using PH480272T_005_I11Q Display Panel
    #if (DISPLAY_PANEL == PH480272T_005_I11Q)
        #define DISP_ORIENTATION        90
        #define DISP_HOR_RESOLUTION     176
        #define DISP_VER_RESOLUTION     220
    #endif // #if (DISPLAY_PANEL == PH480272T_005_I11Q)

/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */

/*********************************************************************
* IOS FOR THE DISPLAY CONTROLLER
*********************************************************************/
#ifdef __PIC32MX__
    #if   defined( __32MX220F032D__)
        // Definitions for TRIS pins
        #define DISPLAY_RESET_ADDR_BIT 3 // PMA3 -> RC5
        #if    defined( DISPLAY_RESET_ADDR_BIT )
            #define DisplayResetConfig()    PMAENSET=1<<DISPLAY_RESET_ADDR_BIT;
        #else
            #define DisplayResetConfig()    TRISCbits.TRISC5 = 0;
        #endif//defined( DISPLAY_RESET_ADDR_BIT )

        #define DisplayWriteConfig()        TRISBbits.TRISB3 = 0;
        #define DisplayReadConfig()         TRISBbits.TRISB13 = 0;
        #define DisplayCSConfig()           1;  //Always Grounded
        #define DisplayRSConfig()           TRISBbits.TRISB5 = 0;

        #define DisplayResetEnable()        LATCbits.LATC5 = 1;
        #define DisplayResetDisable()       LATCbits.LATC5 = 0;

        //Chip Select
        #define CLR_CS()                    1; // Always Grounded
        #define SET_CS()                    1; // Always Grounded

        //Data/Command Select
        #define CLR_RS()                    LATBbits.LATB5 = 0;
        #define SET_RS()                    LATBbits.LATB5 = 1;

        //Read Line
        #define SET_RD()                    PORTBbits.RB13 = 0;
        #define CLR_RD()                    PORTBbits.RB13 = 1;

        //Write Line
        #define SET_WR()                    LATBbits.LATB3 = 0;
        #define CLR_WR()                    LATBbits.LATB3 = 1;

        //RESET Line
        #if    defined( DISPLAY_RESET_ADDR_BIT )
            #define SET_RESET()             PMADDRSET=1<<DISPLAY_RESET_ADDR_BIT;
            #define CLR_RESET()             PMADDRCLR=1<<DISPLAY_RESET_ADDR_BIT;
        #else
            #define SET_RESET()             LATCbits.LATC5 = 1;
            #define CLR_RESET()             LATCbits.LATC5 = 0;
        #endif//defined( DISPLAY_RESET_ADDR_BIT )

        // Definitions for CS pin
        #define DisplayEnable()             LATBbits.LATB5 = 0;
        #define DisplayDisable()            LATBbits.LATB5 = 1;

    #else
       // Definitions for TRIS pins
        #define DisplayResetConfig()        TRISCCLR = _TRISC_TRISC1_MASK    //ADK Checked
        #define DisplayWriteConfig()        TRISDCLR = _TRISD_TRISD2_MASK
        #define DisplayReadConfig()         TRISDCLR = _TRISD_TRISD5_MASK    //ADK Checked
        #define DisplayCSConfig()           TRISDCLR = _TRISD_TRISD11_MASK   //ADK Checked
        #define DisplayRSConfig()           TRISBCLR = _TRISB_TRISB15_MASK   //ADK Checked


        #define DisplayResetEnable()        LATCCLR =  _LATC_LATC1_MASK       //ADK Checked
        #define DisplayResetDisable()       LATCSET =  _LATC_LATC1_MASK       //ADK Checked

        //Chip Select
        #define CLR_CS()                    LATDCLR = _LATD_LATD11_MASK       //ADK Checked
        #define SET_CS()                    LATDSET = _LATD_LATD11_MASK       //ADK Checked
        //Data/Command Select
        #define CLR_RS()                    LATBCLR = _LATB_LATB15_MASK       //ADK Checked
        #define SET_RS()                    LATBSET = _LATB_LATB15_MASK       //ADK Checked
        //Read Line
        #define SET_RD()                    LATDSET = _LATD_LATD5_MASK       //ADK Checked
        #define CLR_RD()                    LATDCLR = _LATD_LATD5_MASK       //ADK Checked
        //Write Line
        #define SET_WR()                    LATDSET = _LATD_LATD4_MASK       //ADK Checked
        #define CLR_WR()                    LATDCLR = _LATD_LATD4_MASK       //ADK Checked
        //RESET Line
        #define SET_RESET()                 LATCSET = _LATC_LATC1_MASK       //ADK Checked
        #define CLR_RESET()                 LATCCLR = _LATC_LATC1_MASK       //ADK Checked

        // Definitions for CS pin
        #define DisplayEnable()             LATDCLR = _LATD_LATD11_MASK     //ADK Checked
        #define DisplayDisable()            LATDSET = _LATD_LATD11_MASK     //ADK Checked
    #endif//defined( __32MX220F032D__)

#else     //PIC 24 Support

                    // Definitions for TRIS pins
            #define DisplayResetConfig()        TRISCbits.TRISC5 = 0;
            #define DisplayWriteConfig()        TRISBbits.TRISB3 = 0;
            #define DisplayReadConfig()         TRISBbits.TRISB13 = 0;
            #define DisplayCSConfig()           1;  //Always Grounded
            #define DisplayRSConfig()           TRISBbits.TRISB5 = 0;

            #define DisplayResetEnable()        LATCbits.LATC5 = 1;
            #define DisplayResetDisable()       LATCbits.LATC5 = 0;

            //Chip Select
            #define CLR_CS()                    1; // Always Grounded
            #define SET_CS()                    1; // Always Grounded
            //Data/Command Select
            #define CLR_RS()                    LATBbits.LATB5 = 0;
            #define SET_RS()                    LATBbits.LATB5 = 1;
            //Read Line
            #define SET_RD()                    PORTBbits.RB13 = 0;
            #define CLR_RD()                    PORTBbits.RB13 = 1;
            //Write Line
            #define SET_WR()                    LATBbits.LATB3 = 0;
            #define CLR_WR()                    LATBbits.LATB3 = 1;
            //RESET Line
            #define SET_RESET()                 LATCbits.LATC5 = 1;
            #define CLR_RESET()                 LATCbits.LATC5 = 0;

            // Definitions for CS pin
            #define DisplayEnable()             LATBbits.LATB5 = 0;
            #define DisplayDisable()            LATBbits.LATB5 = 1;

#endif
/*********************************************************************
* IO FOR THE BEEPER
*********************************************************************/
#if defined (GFX_PICTAIL_V2) || defined (GFX_PICTAIL_V1)
    #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
        #define BEEP_TRIS_BIT   TRISBbits.TRISB4
        #define BEEP_LAT_BIT    LATBbits.LATB4
    #else
        #define BEEP_TRIS_BIT   TRISDbits.TRISD0
        #define BEEP_LAT_BIT    LATDbits.LATD0
    #endif
#elif defined (GFX_PICTAIL_V3E) || defined (PIC24FJ256DA210_DEV_BOARD)
    #define BEEP_TRIS_BIT   TRISDbits.TRISD0
    #define BEEP_LAT_BIT    LATDbits.LATD0
#endif // #if defined (GFX_PICTAIL_V2)

/*********************************************************************
* IOS FOR THE FLASH/EEPROM SPI
*********************************************************************/
#if defined (GFX_PICTAIL_V2) || defined (GFX_PICTAIL_V1)
    #define USE_SST25_SPI2
    #if  defined(__PIC24F__) || defined(__PIC24H__) || defined(__dsPIC33F__)
        #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
            #define EEPROM_CS_TRIS  TRISAbits.TRISA0
            #define EEPROM_CS_LAT   LATAbits.LATA0
        #elif defined(__PIC24FJ256GB110__)
            // This PIM has RD12 rerouted to RG0
            #define EEPROM_CS_TRIS  TRISGbits.TRISG0
            #define EEPROM_CS_LAT   LATGbits.LATG0
        #else
            #define EEPROM_CS_TRIS  TRISDbits.TRISD12
            #define EEPROM_CS_LAT   LATDbits.LATD12
        #endif
    #elif defined(__PIC32MX__)
        #define EEPROM_CS_TRIS  TRISDbits.TRISD12
        #define EEPROM_CS_LAT   LATDbits.LATD12
    #endif
    #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
        #define EEPROM_SCK_TRIS TRISCbits.TRISC2
        #define EEPROM_SDO_TRIS TRISCbits.TRISC0
        #define EEPROM_SDI_TRIS TRISCbits.TRISC1
    #else
        #define EEPROM_SCK_TRIS TRISGbits.TRISG6
        #define EEPROM_SDO_TRIS TRISGbits.TRISG8
        #define EEPROM_SDI_TRIS TRISGbits.TRISG7
    #endif
#elif defined (GFX_PICTAIL_V3E)
    #define USE_SST25_SPI2
    #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
        #define SST25_CS_TRIS   TRISAbits.TRISA8
        #define SST25_CS_LAT    LATAbits.LATA8
        #define SST25_SCK_TRIS  TRISCbits.TRISC2
        #define SST25_SDO_TRIS  TRISCbits.TRISC0
        #define SST25_SDI_TRIS  TRISCbits.TRISC1
    #else
        #define SST25_CS_TRIS   TRISDbits.TRISD1
        #define SST25_CS_LAT    LATDbits.LATD1
        #define SST25_SCK_TRIS  TRISGbits.TRISG6
        #define SST25_SDO_TRIS  TRISGbits.TRISG8
        #define SST25_SDI_TRIS  TRISGbits.TRISG7
        #define SST25_SDI_ANS   ANSGbits.ANSG7
    #endif
#elif defined (PIC24FJ256DA210_DEV_BOARD)
    #define USE_SST25_SPI2
    #define SST25_CS_TRIS   TRISAbits.TRISA14
    #define SST25_CS_LAT    LATAbits.LATA14
    #define SST25_SCK_TRIS  TRISDbits.TRISD8
    #define SST25_SDO_TRIS  TRISBbits.TRISB1
    #define SST25_SDI_TRIS  TRISBbits.TRISB0
    #define SST25_SDI_ANS   ANSBbits.ANSB0
    #define SST25_SDO_ANS   ANSBbits.ANSB1
#elif defined (MULTI_MEDIA_BOARD_DM00123)

    #if defined (PIC32_GP_SK_DM320001) || defined (PIC32_USB_SK_DM320003_1)
        #define USE_SST25_SPI2
    #elif defined (PIC32_USB_SK_DM320003_2)
        #define USE_SST25_SPI2A
    #elif defined (PIC32_ENET_SK_DM320004)
        #define USE_SST25_SPI3A
    #else
        #error "Please define the starter kit that you are using"
    #endif

    // define the CPLD SPI selection and chip select
    #ifdef USE_SST25_SPI2
        #define SST25_CS_TRIS   TRISGbits.TRISG9
        #define SST25_CS_LAT    LATGbits.LATG9
        #define SPI_FLASH_CHANNEL   CPLD_SPI2
    #elif defined (USE_SST25_SPI2A)
        #define SST25_CS_TRIS   TRISGbits.TRISG9
        #define SST25_CS_LAT    LATGbits.LATG9
        #define SPI_FLASH_CHANNEL   CPLD_SPI2A
    #elif defined (USE_SST25_SPI3A)
        #define SST25_CS_TRIS   TRISFbits.TRISF12
        #define SST25_CS_LAT    LATFbits.LATF12
        #define SPI_FLASH_CHANNEL   CPLD_SPI3A
    #else
        #error "SPI Channel can't be used for SPI Flash"
    #endif

#endif



/*********************************************************************
* IOS FOR THE GRAPHICS PICTAIL PARALLEL FLASH MEMORY
*********************************************************************/
#if defined (GFX_PICTAIL_V1)
    #define SST39_CS_TRIS    TRISDbits.TRISD9
    #define SST39_CS_LAT     LATDbits.LATD9
    #define SST39_A18_TRIS   TRISBbits.TRISB11
    #define SST39_A18_LAT    LATBbits.LATB11
    #define SST39_A17_TRIS   TRISBbits.TRISB10
    #define SST39_A17_LAT    LATBbits.LATB10
    #define SST39_A16_TRIS   TRISDbits.TRISD3
    #define SST39_A16_LAT    LATDbits.LATD3
#elif defined(GFX_PICTAIL_V2)
    #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
        #define SST39_CS_TRIS    TRISAbits.TRISA8
        #define SST39_CS_LAT     LATAbits.LATA8
        #define SST39_A18_TRIS   TRISAbits.TRISA1
        #define SST39_A18_LAT    LATAbits.LATA1
        #define SST39_A17_TRIS   TRISCbits.TRISC5
        #define SST39_A17_LAT    LATCbits.LATC5
        #define SST39_A16_TRIS   TRISCbits.TRISC4
        #define SST39_A16_LAT    LATCbits.LATC4
    #else
        #define SST39_CS_TRIS    TRISDbits.TRISD1
        #define SST39_CS_LAT     LATDbits.LATD1
        #define SST39_A18_TRIS   TRISCbits.TRISC2
        #define SST39_A18_LAT    LATCbits.LATC2
        #define SST39_A17_TRIS   TRISGbits.TRISG15
        #define SST39_A17_LAT    LATGbits.LATG15
        #define SST39_A16_TRIS   TRISGbits.TRISG14
        #define SST39_A16_LAT    LATGbits.LATG14
    #endif
#endif

/*********************************************************************
* IOS FOR THE TOUCH SCREEN
*********************************************************************/

// ADC Status
#define TOUCH_ADC_DONE          AD1CON1bits.DONE

// ADC channel constants
#if defined (PIC24FJ256DA210_DEV_BOARD)
    #define ADC_TEMP    4
    #define ADC_POT     5
#elif defined (MULTI_MEDIA_BOARD_DM00123)
    /* The Multi-Media Development Board does not have a
     * analog pot or temperature sensor (see BMA150 for temperature
     * sensor data
     */
#else
    #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
        #define ADC_TEMP        4
        #define ADC_POT         0
        #define ADC_POT_TRIS    TRISAbits.TRISA0
        #define ADC_POT_PCFG    AD1PCFGLbits.PCFG0
    #elif defined(__PIC32MX__)
        #define ADC_TEMP    ADC_CH0_POS_SAMPLEA_AN4
        #define ADC_POT     ADC_CH0_POS_SAMPLEA_AN5
    #else
        #define ADC_TEMP    4
        #define ADC_POT     5
    #endif
#endif

#if defined (GFX_PICTAIL_V1)
    #ifdef __PIC32MX__
        #define ADC_XPOS    ADC_CH0_POS_SAMPLEA_AN13
        #define ADC_YPOS    ADC_CH0_POS_SAMPLEA_AN12
    #else
        #define ADC_XPOS    13
        #define ADC_YPOS    12
    #endif

    // Y port definitions
    #define ADPCFG_XPOS AD1PCFGbits.PCFG13
    #define LAT_XPOS    LATBbits.LATB13
    #define LAT_XNEG    LATBbits.LATB11
    #define TRIS_XPOS   TRISBbits.TRISB13
    #define TRIS_XNEG   TRISBbits.TRISB11

    // X port definitions
    #define ADPCFG_YPOS AD1PCFGbits.PCFG12
    #define LAT_YPOS    LATBbits.LATB12
    #define LAT_YNEG    LATBbits.LATB10
    #define TRIS_YPOS   TRISBbits.TRISB12
    #define TRIS_YNEG   TRISBbits.TRISB10

#elif defined (GFX_PICTAIL_V2)
    #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
        #define ADC_XPOS    5
        #define ADC_YPOS    4
    #elif defined(__PIC32MX__)
        #define ADC_XPOS    ADC_CH0_POS_SAMPLEA_AN11
        #define ADC_YPOS    ADC_CH0_POS_SAMPLEA_AN10
    #else
        #define ADC_XPOS    11
        #define ADC_YPOS    10
    #endif
    #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)

        // Y port definitions
        #define ADPCFG_XPOS AD1PCFGLbits.PCFG5
        #define LAT_XPOS    LATBbits.LATB3
        #define LAT_XNEG    LATCbits.LATC9
        #define TRIS_XPOS   TRISBbits.TRISB3
        #define TRIS_XNEG   TRISCbits.TRISC9

        // X port definitions
        #define ADPCFG_YPOS AD1PCFGLbits.PCFG4
        #define LAT_YPOS    LATBbits.LATB2
        #define LAT_YNEG    LATCbits.LATC8
        #define TRIS_YPOS   TRISBbits.TRISB2
        #define TRIS_YNEG   TRISCbits.TRISC8

    #else

        // Y port definitions
        #define ADPCFG_XPOS AD1PCFGbits.PCFG11
        #define LAT_XPOS    LATBbits.LATB11
        #define LAT_XNEG    LATGbits.LATG13
        #define TRIS_XPOS   TRISBbits.TRISB11
        #define TRIS_XNEG   TRISGbits.TRISG13

        // X port definitions
        #define ADPCFG_YPOS AD1PCFGbits.PCFG10
        #define LAT_YPOS    LATBbits.LATB10
        #define LAT_YNEG    LATGbits.LATG12
        #define TRIS_YPOS   TRISBbits.TRISB10
        #define TRIS_YNEG   TRISGbits.TRISG12
    #endif
#elif defined (GFX_PICTAIL_V3E)
    #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
        #define ADC_XPOS    5
        #define ADC_YPOS    4
    #elif defined(__PIC32MX__)
        #define ADC_XPOS    ADC_CH0_POS_SAMPLEA_AN11
        #define ADC_YPOS    ADC_CH0_POS_SAMPLEA_AN10
    #else
        #define ADC_XPOS    11
        #define ADC_YPOS    10
    #endif
    #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)

        // Y port definitions
        #define ADPCFG_XPOS AD1PCFGLbits.PCFG5
        #define LAT_XPOS    LATBbits.LATB3
        #define LAT_XNEG    LATCbits.LATC9
        #define TRIS_XPOS   TRISBbits.TRISB3
        #define TRIS_XNEG   TRISCbits.TRISC9

        // X port definitions
        #define ADPCFG_YPOS AD1PCFGLbits.PCFG4
        #define LAT_YPOS    LATBbits.LATB2
        #define LAT_YNEG    LATCbits.LATC8
        #define TRIS_YPOS   TRISBbits.TRISB2
        #define TRIS_YNEG   TRISCbits.TRISC8

    #else

        // Y port definitions
        #define ADPCFG_XPOS AD1PCFGbits.PCFG11
        #define LAT_XPOS    LATBbits.LATB11
        #define TRIS_XPOS   TRISBbits.TRISB11

        #if defined(__32MX460F512L__) && !defined (PIC32_USB_SK_DM320003_1)
            #define LAT_XNEG    LATGbits.LATG15
            #define TRIS_XNEG   TRISGbits.TRISG15
        #else
            #define LAT_XNEG    LATDbits.LATD9
            #define TRIS_XNEG   TRISDbits.TRISD9
        #endif

        // X port definitions
        #define ADPCFG_YPOS AD1PCFGbits.PCFG10
        #define LAT_YPOS    LATBbits.LATB10
        #define LAT_YNEG    LATDbits.LATD8
        #define TRIS_YPOS   TRISBbits.TRISB10
        #define TRIS_YNEG   TRISDbits.TRISD8
    #endif
#elif defined (PIC24FJ256DA210_DEV_BOARD)
    #if defined(__PIC24FJ256DA210__)
        #define ADC_XPOS    16
        #define ADC_YPOS    18

        // Y port definitions
        #define ADPCFG_XPOS ANSCbits.ANSC4
        #define LAT_XPOS    LATCbits.LATC4
        #define TRIS_XPOS   TRISCbits.TRISC4
        #define LAT_XNEG    LATAbits.LATA2
        #define TRIS_XNEG   TRISAbits.TRISA2

        // X port definitions
        #define ADPCFG_YPOS ANSGbits.ANSG7
        #define LAT_YPOS    LATGbits.LATG7
        #define TRIS_YPOS   TRISGbits.TRISG7
        #define LAT_YNEG    LATAbits.LATA1
        #define TRIS_YNEG   TRISAbits.TRISA1
    #endif
#elif defined (MULTI_MEDIA_BOARD_DM00123)
    #define ADC_XPOS    ADC_CH0_POS_SAMPLEA_AN11
    #define ADC_YPOS    ADC_CH0_POS_SAMPLEA_AN14

    // X port definitions
    #define ADPCFG_XPOS AD1PCFGbits.PCFG11
    #define LAT_XPOS    LATBbits.LATB11
    #define TRIS_XPOS   TRISBbits.TRISB11
    #define LAT_XNEG    LATBbits.LATB13
    #define TRIS_XNEG   AD1PCFGbits.PCFG13 = 1, TRISBbits.TRISB13

    // Y port definitions
    #define ADPCFG_YPOS AD1PCFGbits.PCFG14
    #define LAT_YPOS    LATBbits.LATB14
    #define TRIS_YPOS   TRISBbits.TRISB14
    #define LAT_YNEG    LATBbits.LATB12
    #define TRIS_YNEG   AD1PCFGbits.PCFG12 = 1, TRISBbits.TRISB12
#endif

/*********************************************************************
* IOS FOR THE SWITCHES (SIDE BUTTONS)
*********************************************************************/

#if defined (PIC24FJ256DA210_DEV_BOARD)
    #if defined(__PIC24FJ256DA210__)
        #define BTN_S1  _RG8
        #define BTN_S2  _RE9
        #define BTN_S3  _RB5
      #endif

#elif defined (MULTI_MEDIA_BOARD_DM00123)
      #define BTN_S3  1
      #define BTN_S4  1
      #define BTN_S5  1
      #define BTN_S6  1
#else
    #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
        #define BTN_S3  _RA9
        #define BTN_S4  1
        #define BTN_S5  1
        #define BTN_S6  1
      #elif defined (PIC32_USB_SK_DM320003_1) || defined (PIC32_USB_SK_DM320003_2) || defined (PIC32_GP_SK_DM320001)
        #define BTN_S3  1
        #define BTN_S4  1
        #define BTN_S5  1
        #define BTN_S6  1
      #else
        #define BTN_S3  _RD6
        #define BTN_S4  _RD13
        #define BTN_S5  _RA7
        #define BTN_S6  _RD7
    #endif
#endif

/*********************************************************************
* IOS FOR THE TCON
*********************************************************************/
#if defined (PIC24FJ256DA210_DEV_BOARD)
    #if (DISPLAY_PANEL == TFT_G240320LTSW_118W_E)
        #define CS_PORT     _RA0
        #define CS_TRIS     _TRISA0
        #define CS_DIG()

        #define SCL_PORT    _RD8
        #define SCL_TRIS    _TRISD8
        #define SCL_DIG()

        #define SDO_PORT    _RB1
        #define SDO_TRIS    _TRISB1
        #define SDO_DIG()   _ANSB1 = 0;

        #define DC_PORT     _RB0
        #define DC_TRIS     _TRISB0
        #define DC_DIG()    _ANSB0 = 0;

    #endif // #if (DISPLAY_PANEL == TFT_G240320LTSW_118W_E)
#endif // #if defined (DA210_DEV_BOARD)...

#endif //ifdef _GRAPHICS_H

/*********************************************************************
* IOS FOR THE UART
*********************************************************************/
#if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
    #define TX_TRIS TRISCbits.TRISC1
    #define RX_TRIS TRISCbits.TRISC0
#elif defined(__PIC24FJ256DA210__)
    #define TX_TRIS TRISFbits.TRISF3
    #define RX_TRIS TRISDbits.TRISD0
#else
    #define TX_TRIS TRISFbits.TRISF5
    #define RX_TRIS TRISFbits.TRISF4
#endif

/*********************************************************************
* RTCC DEFAULT INITIALIZATION (these are values to initialize the RTCC
*********************************************************************/
#define RTCC_DEFAULT_DAY        29      // 29
#define RTCC_DEFAULT_MONTH      4       // April
#define RTCC_DEFAULT_YEAR       10      // 2010
#define RTCC_DEFAULT_WEEKDAY    04      // Thursday
#define RTCC_DEFAULT_HOUR       10      // 10:10:01
#define RTCC_DEFAULT_MINUTE     10
#define RTCC_DEFAULT_SECOND     01

#if defined (MULTI_MEDIA_BOARD_DM00123)

#include <plib.h>

/*********************************************************************
* Configuration for the CPLD
*********************************************************************/
#ifdef USE_16BIT_PMP
#define GRAPHICS_HW_CONFIG     CPLD_GFX_CONFIG_16BIT
#else
#define GRAPHICS_HW_CONFIG     CPLD_GFX_CONFIG_8BIT
#endif

/*********************************************************************
* MMB LEDs
*********************************************************************/
typedef enum
{
    LED_2,
    LED_3,
    LED_4,
    LED_5,
    LED_10
}MMB_LED;

extern inline void __attribute__((always_inline)) SetLEDDirection(void)
{
   PORTSetPinsDigitalOut(IOPORT_D, (BIT_1 | BIT_2 | BIT_3));
    PORTSetPinsDigitalOut(IOPORT_C, (BIT_1 | BIT_2));
}

extern inline void __attribute__((always_inline)) TurnLEDOn(MMB_LED led)
{
    if(led == LED_2)
      PORTSetBits(IOPORT_D, BIT_1);

    if(led == LED_3)
      PORTSetBits(IOPORT_D, BIT_2);

    if(led == LED_4)
      PORTSetBits(IOPORT_D, BIT_3);

    if(led == LED_5)
      PORTSetBits(IOPORT_C, BIT_1);

    if(led == LED_10)
      PORTSetBits(IOPORT_C, BIT_2);

}

extern inline void __attribute__((always_inline)) TurnLEDOff(MMB_LED led)
{
    if(led == LED_2)
      PORTClearBits(IOPORT_D, BIT_1);

    if(led == LED_3)
      PORTClearBits(IOPORT_D, BIT_2);

    if(led == LED_4)
      PORTClearBits(IOPORT_D, BIT_3);

    if(led == LED_5)
      PORTClearBits(IOPORT_C, BIT_1);

    if(led == LED_10)
      PORTClearBits(IOPORT_C, BIT_2);

}

extern inline void __attribute__((always_inline)) ToggleLED(MMB_LED led)
{
    if(led == LED_2)
      PORTToggleBits(IOPORT_D, BIT_1);

    if(led == LED_3)
      PORTToggleBits(IOPORT_D, BIT_2);

    if(led == LED_4)
      PORTToggleBits(IOPORT_D, BIT_3);

    if(led == LED_5)
      PORTToggleBits(IOPORT_C, BIT_1);

    if(led == LED_10)
      PORTToggleBits(IOPORT_C, BIT_2);

}

extern inline void __attribute__((always_inline)) TurnLEDAllOn(void)
{
      PORTSetBits(IOPORT_D, BIT_1);
      PORTSetBits(IOPORT_D, BIT_2);
      PORTSetBits(IOPORT_D, BIT_3);
      PORTSetBits(IOPORT_C, BIT_1);
      PORTSetBits(IOPORT_C, BIT_2);

}

extern inline void __attribute__((always_inline)) TurnLEDAllOff(void)
{
      PORTClearBits(IOPORT_D, BIT_1);
      PORTClearBits(IOPORT_D, BIT_2);
      PORTClearBits(IOPORT_D, BIT_3);
      PORTClearBits(IOPORT_C, BIT_1);
      PORTClearBits(IOPORT_C, BIT_2);

}

#endif // #ifdef (MULTI_MEDIA_BOARD_DM00123)

#endif//ndef__HARDWARE_PROFILE_GFX_ADK_H


/*********************************************************************
 *
 *                  Dynamic Module Header definitions
 *
 *********************************************************************
 * FileName:        module_dyn_header.h
 * Dependencies:
 * Processor:       PIC32
 *
 * Complier:        MPLAB C32
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


#ifndef _MODULE_DYN_HEADER_H_
#define _MODULE_DYN_HEADER_H_


typedef void*    (*export_proc)(void);       // generic exported procedure
typedef void*    (*import_proc)(void);       // generic imported procedure


typedef struct
{
    export_proc     proc_f;                 // procedure itself
    const char*     proc_name;              // name of the exported procedure
}export_entry;


typedef struct
{
    int             nexports;               // number of exported procedures
    export_entry    export_entries[];       // optional procedure entries
}export_dcpt;   // export descriptor    


typedef struct
{
    const char*     module_name;            // module to import from
    const char*     iproc_name;             // name of procedures to be imported
    import_proc*    iproc;                  // RAM containing the pointer to this procedure
                                            // loader will update the table
}import_entry;

typedef struct
{
    int             nimports;               // number of imported modules
    import_entry    imports[];              // import descriptors
}import_dcpt;   // import descriptor
    

#define _MCHP_PROCESSOR_NAME_SIZE_      16

typedef struct
{
    unsigned long   module_flags;       // reserved for now
    char            processor_name[_MCHP_PROCESSOR_NAME_SIZE_];   // processor the module was build for
    unsigned short  processor_variant;  // processor variant
    unsigned short  module_version;     // library version    
    void*           ram_start;
    void*           ram_end;            // ram area
    void*           flash_start;
    void*           flash_end;          // flash area
    void*           reserved[0];        // future expansion        
}module_info_dcpt;   // module info descriptor


// default module info
// 

// linker symbols for ram/flash areas
extern char _text_begin[];            // the start of the text segment
extern char _text_end[];              // the end of the text segment
extern char _data_begin[];            // the start of the data segment
extern char _data_end[];              // the end of the data segment

#define    _DefaultModuleInfo(vers)  { \
    0,                         \
    {"pic32mx"},               \
    __PIC32_FEATURE_SET__,     \
    vers,                      \
    (void*)_data_begin,        \
    (void*)_data_end,          \
    (void*)_text_begin,        \
    (void*)_text_end,          \
                                }



typedef struct
{
    export_proc     proc_init;              // init procedure; could be 0
    export_proc     proc_cleanup;           // clean-up procedure; could be 0
}module_init_dcpt;  // module initialization descriptor
    


typedef struct
{
    int             module_error;          // last error condition, per module
    // some other fields may be added here
    //
}module_data_dcpt;   // private module data descriptor     








#define _MCHP_DYN_HDR_SIGNATURE_SIZE_   20
#define _MCHP_MODULE_NAME_SIZE_         20

#define _MCHP_DYN_HDR_SIGNATURE_        "@MCHP_MODULE_HEADER"

typedef struct
{
    const char              module_sign[_MCHP_DYN_HDR_SIGNATURE_SIZE_];         // module signature
    //
    const char              module_name[_MCHP_MODULE_NAME_SIZE_];               // name of the module

    // initialization
    const module_init_dcpt* module_init;                                        // pointer to module init              

    // exports
    const export_dcpt*      module_export_tbl;                                  // pointer to the export descriptor

    // imports
    const import_dcpt*      module_import_tbl;                                  // pointer to the import descriptor

    // private data
    module_data_dcpt*       module_data;                                        // pointer to module data   

    // module info
    const module_info_dcpt  module_info;                                        // module info
}module_dyn_hdr;



// prologue/epilogue for exported procedures

extern volatile char    _gp[];
register unsigned int   _export_gp_reg  __asm__("gp");

#define export_prologue(save_gp)  do{ save_gp = _export_gp_reg; \
                                      _export_gp_reg = (unsigned int)_gp; \
                                    }while(0)

   
#define export_epilogue(save_gp)  do{ _export_gp_reg = save_gp; \
                                    }while(0)
    



#endif	// _MODULE_DYN_HEADER_H_


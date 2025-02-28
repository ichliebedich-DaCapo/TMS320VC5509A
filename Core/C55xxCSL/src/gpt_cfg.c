/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... GPT
* FILENAME...... gpt_cfg.c
* DATE CREATED.. Sun 02/10/2002 
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  02/10/2002 
*   MODIFIED : 03/12/2003 changed structure member names
*------------------------------------------------------------------------------
* DESCRIPTION:  (body file for the 5502 General Purpose 64 bit timer)
*
*
*
\******************************************************************************/
#define _GPT_MOD_

/****************************************\
* include files
\****************************************/    

#include <csl_gpt.h>
   
 
#if (_GPT_SUPPORT)
/******************************************************************************\
*                         L O C A L   S E C T I O N
\******************************************************************************/

/****************************************\
* GPT static macro declarations
\****************************************/
/* See gpt.h Romability */
 
/****************************************\
* GPT static typedef declarations
\****************************************/

/****************************************\
* GPT static function declarations
\****************************************/

/****************************************\
* GPT Variable definitions : ROMability 
\****************************************/

/* Definition Romability See GPT.h */


             
/****************************************\
* TIMER static function definitions
\****************************************/

/*----------------------------------------------------------------------------*/

/******************************************************************************\
*                        G L O B A L   S E C T I O N
\******************************************************************************/

/****************************************\
* TIMER global variable definitions
\****************************************/
/* Definition See GPT.h */
/*----------------------------------------------------------------------------*/
/* DSP/BIOS symbols that CSL needs to check to see what if any timer devices  */
/* are in use by BIOS. These symbols will e defined in the BIOS linker        */
/* command file generated by gconf.                                           */
/* See C6xx */
             
/****************************************\
* TIMER global function definitions
\****************************************/

#pragma CODE_SECTION(GPT_config,".text:GPT_config")

/*----------------------------------------------------------------------------*/
void GPT_config(GPT_Handle hGpt, GPT_Config *Config) {
  int oldgie;
  ioport GPT_RegObj *regPtr = ((GPT_PrivateObj *)hGpt)->regs;

  GPT_ASSERT_HANDLE(hGpt,return);

    oldgie = IRQ_globalDisable();  
 
   (regPtr->gptgctl1) = 0x0000u;
   (regPtr->gptgpint) = Config->gptgpint;
   (regPtr->gptgpen) = Config->gptgpen;
   (regPtr->gptgpdir) = Config->gptgpdir;
   (regPtr->gptgpdat) = Config->gptgpdat;
   (regPtr->gptprd1) = Config->gptprd1;
   (regPtr->gptprd2) = Config->gptprd2;
   (regPtr->gptprd3) = Config->gptprd3;
   (regPtr->gptprd4) = Config->gptprd4;
   (regPtr->gptctl1) = Config->gptctl1;
   (regPtr->gptctl2) = Config->gptctl2;
   (regPtr->gptgctl1) = Config->gptgctl1;  
  
  IRQ_globalRestore(oldgie); 
 
}
/*----------------------------------------------------------------------------*/

#endif /* TIMER_SUPPORT */
/******************************************************************************\
* End of gpt_cfg.c
\******************************************************************************/


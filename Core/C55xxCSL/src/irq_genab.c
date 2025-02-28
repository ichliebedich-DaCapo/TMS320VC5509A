/******************************************************************************\
+*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... IRQ
* FILENAME...... IRQ_genab.c
* DATE CREATED.. Thu 03/22/2001 
* PROJECT....... Chip Support Library
* COMPONENT..... CSL service layer
* PREREQUISITS.. csl_std.h 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  03/22/2001
*   MODIFIED: 06/29/2001 added pragma for section per function
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the IRQ module)
*
*
*
\******************************************************************************/
#define _IRQ_MOD_

#include <csl_irq.h>

#if (_IRQ_SUPPORT)

#pragma CODE_SECTION(IRQ_globalEnable,".text:IRQ_globalEnable")

/*--------------------------------------------------------------------*/
 int IRQ_globalEnable() {
   int old_intm = CHIP_FGET(ST1_55,INTM);
   asm("\tNOP                      ;====> CODE AUTO-GENERATED by CSL");
#ifdef ALGEBRAIC
   asm("\tBIT(ST1,#ST1_INTM)=#0    ;====> CODE AUTO-GENERATED by CSL");
#else
   asm("\tBCLR INTM                ;====> CODE AUTO-GENERATED by CSL");
#endif
   return old_intm;    
 }
#endif /* IRQ_SUPPORT */
/******************************************************************************\
* End of IRQ_genab.c
\******************************************************************************/


/** 
 * FILE: IntcInterrupts.c
 *
 * DESCRIPTION:  Contains an implementations of generic interrupt      
 *    controller handling routines for the MPC56xx and PX MCU families.
 *
 * For more on Qorivva/PX MCUs INTC module and initializations please
 * consult the Qorivva cookbook AN2865.  
 *
 *
 * VERSION: 1.2     
*/

#include "ngv_dmu.h"
#include "basetimer.h"
#include "serialdebug.h"
#include "myuart.h"
#include "youruart.h"
#include "charlcd.h"
#include "freerunadc.h"
#include "freerunusonic.h"
#include "rotaryencoder.h"


/*---------------------------------------------------------------------------*/
/* By default we do not use nested interrupts, interrupts are handled in C.  */
/* In case you want to use nested interrupts, set INTC_NESTED_INTERRUPT.     */
/* In this case make sure all the needed registers are saved in the prolog   */
/* and epilog of asm void INTC_INTCInterruptHandler(void)                    */ 
/*---------------------------------------------------------------------------*/

#ifndef INTC_NESTED_INTERRUPT
#if NGV_DMU_PROJECT == 0
#define INTC_NESTED_INTERRUPT 0
#else
#define INTC_NESTED_INTERRUPT 1
#endif
#endif

/*---------------------------------------------------------------------------*/
/* Includes                                                                  */
/*---------------------------------------------------------------------------*/

#include "MPC5606B.h"         /* MCU platform development header       */

#include "IntcInterrupts.h"     /* Implement functions from this file */

/*---------------------------------------------------------------------------*/
/* Inline Assembler Defines                                                  */
/*---------------------------------------------------------------------------*/

/** This macro allows to use C defined address with the inline assembler */
#define MAKE_HLI_ADDRESS(hli_name, c_expr) /*lint -e753 */enum { hli_name=/*lint -e30*/((int)(c_expr)) /*lint -esym(749, hli_name) */ };

/** Address of the IACKR Interrupt Controller register. */
MAKE_HLI_ADDRESS(INTC_IACKR, &INTC.IACKR.R)
/** Address of the EOIR End-of-Interrupt Register register. */
MAKE_HLI_ADDRESS(INTC_EOIR, &INTC.EOIR.R)

/** Address of the MCR -- used for e200z0h initialization */
MAKE_HLI_ADDRESS(INTC_MCR, &INTC.MCR.R)

/*---------------------------------------------------------------------------*/
/* Function Implementations                                                  */
/*---------------------------------------------------------------------------*/

#if NGV_DMU_PROJECT == 1
void interrupt_handler (void);
#endif

/** This is the Interrupt Service Routine Branch Table placed in memory */
#define INTC_INTERRUPTS_REQUEST_VECTOR_TABLE_SIZE (233)

#pragma push /* Save the current state */

#if NGV_DMU_PROJECT == 0
#pragma section data_type ".__initialized_intc_handlertable" ".__uninitialized_intc_handlertable"
/* The INTC vector table will be placed in RAM. 
   We will use the ".__uninitialized_intc_handlertable" name to do the 
   placement in the Linker Command File (.lcf) to avoid the initialization at
   startup time.  This will decrease the code size, but the table won't be 
   initialized to zero.
*/ 
INTCInterruptFn INTCInterruptsHandlerTable[INTC_INTERRUPTS_REQUEST_VECTOR_TABLE_SIZE];

#else
#pragma section data_type ".vects" ".uvects"

INTCInterruptFn INTCInterruptsHandlerTable[INTC_INTERRUPTS_REQUEST_VECTOR_TABLE_SIZE] = {
	interrupt_handler,    /* Vector - 0    interrupt_handler*/
	interrupt_handler,    /* Vector - 1    interrupt_handler*/
	interrupt_handler,    /* Vector - 2    interrupt_handler*/
	interrupt_handler,    /* Vector - 3    interrupt_handler*/
	interrupt_handler,    /* Vector - 4    interrupt_handler*/
	interrupt_handler,    /* Vector - 5    interrupt_handler*/
	interrupt_handler,    /* Vector - 6    interrupt_handler*/
	interrupt_handler,    /* Vector - 7    interrupt_handler*/
	interrupt_handler,    /* Vector - 8    Reserved */
	interrupt_handler,    /* Vector - 9    interrupt_handler*/
	interrupt_handler,    /* Vector - 10    interrupt_handler*/
	interrupt_handler,    /* Vector - 11    interrupt_handler*/
	interrupt_handler,    /* Vector - 12    interrupt_handler*/
	interrupt_handler,    /* Vector - 13    interrupt_handler*/
	interrupt_handler,    /* Vector - 14    interrupt_handler*/
	interrupt_handler,    /* Vector - 15    interrupt_handler*/
	interrupt_handler,    /* Vector - 16    interrupt_handler*/
	interrupt_handler,    /* Vector - 17    interrupt_handler*/
	interrupt_handler,    /* Vector - 18    interrupt_handler*/
	interrupt_handler,    /* Vector - 19    interrupt_handler*/
	interrupt_handler,    /* Vector - 20    interrupt_handler*/
	interrupt_handler,    /* Vector - 21    interrupt_handler*/
	interrupt_handler,    /* Vector - 22    interrupt_handler*/
	interrupt_handler,    /* Vector - 23    interrupt_handler*/
	interrupt_handler,    /* Vector - 24    interrupt_handler*/
	interrupt_handler,    /* Vector - 25    interrupt_handler*/
	interrupt_handler,    /* Vector - 26    interrupt_handler*/
	interrupt_handler,    /* Vector - 27    Reserved */
	interrupt_handler,    /* Vector - 28    interrupt_handler*/
	interrupt_handler,    /* Vector - 29    Reserved */
	interrupt_handler,    /* Vector - 30    interrupt_handler*/
	interrupt_handler,    /* Vector - 31    interrupt_handler*/
	interrupt_handler,    /* Vector - 32    interrupt_handler*/
	interrupt_handler,    /* Vector - 33    interrupt_handler*/
	interrupt_handler,    /* Vector - 34    Reserved */
	interrupt_handler,    /* Vector - 35    interrupt_handler*/
	interrupt_handler,    /* Vector - 36    interrupt_handler*/
	interrupt_handler,    /* Vector - 37    Reserved */
	interrupt_handler,    /* Vector - 38    interrupt_handler*/
	interrupt_handler,    /* Vector - 39    interrupt_handler*/
	interrupt_handler,    /* Vector - 40    Reserved */
	interrupt_handler,    /* Vector - 41    interrupt_handler*/
	interrupt_handler,    /* Vector - 42    interrupt_handler*/
	RENC_IRQ16_23_ISR,    /* Vector - 43    RENC_IRQ16_23_ISR*/
	interrupt_handler,    /* Vector - 44    Reserved */
	interrupt_handler,    /* Vector - 45    Reserved */
	interrupt_handler,    /* Vector - 46    interrupt_handler*/
	interrupt_handler,    /* Vector - 47    interrupt_handler*/
	interrupt_handler,    /* Vector - 48    interrupt_handler*/
	interrupt_handler,    /* Vector - 49    interrupt_handler*/
	interrupt_handler,    /* Vector - 50    Reserved */
	interrupt_handler,    /* Vector - 51    interrupt_handler*/
	interrupt_handler,    /* Vector - 52    interrupt_handler*/
	interrupt_handler,    /* Vector - 53    interrupt_handler*/
	interrupt_handler,    /* Vector - 54    interrupt_handler*/
	interrupt_handler,    /* Vector - 55    Reserved */
	interrupt_handler,    /* Vector - 56    interrupt_handler*/
	interrupt_handler,    /* Vector - 57    interrupt_handler*/
	interrupt_handler,    /* Vector - 58    Reserved */
	BTMR_PIT0_ISR,        /* Vector - 59    BTMR_PIT0_ISR*/
	CLCD_PIT1_ISR,        /* Vector - 60    CLCD_PIT1_ISR*/
	interrupt_handler,    /* Vector - 61    interrupt_handler*/
	FADC_ADC0_EOC_ISR,     /* Vector - 62    FADC_ADC0_EOC_ISR*/
	interrupt_handler,    /* Vector - 63    Reserved */
	interrupt_handler,    /* Vector - 65    interrupt_handler*/
	interrupt_handler,    /* Vector - 65    interrupt_handler*/
	interrupt_handler,    /* Vector - 66    interrupt_handler*/
	interrupt_handler,    /* Vector - 67    Reserved */
	interrupt_handler,    /* Vector - 68    interrupt_handler*/
	interrupt_handler,    /* Vector - 69    interrupt_handler*/
	interrupt_handler,    /* Vector - 70    interrupt_handler*/
	interrupt_handler,    /* Vector - 71    interrupt_handler*/
	interrupt_handler,    /* Vector - 72    interrupt_handler*/
	interrupt_handler,    /* Vector - 73    interrupt_handler*/
	interrupt_handler,    /* Vector - 74    interrupt_handler*/
	interrupt_handler,    /* Vector - 75    interrupt_handler*/
	interrupt_handler,    /* Vector - 76    interrupt_handler*/
	interrupt_handler,    /* Vector - 77    interrupt_handler*/
	interrupt_handler,    /* Vector - 78    interrupt_handler*/
	SDBG_RX0_ISR,         /* Vector - 79    SDBG_RX0_ISR*/
	SDBG_TX0_ISR,         /* Vector - 80    SDBG_TX0_ISR*/
	interrupt_handler,    /* Vector - 81    interrupt_handler*/
	interrupt_handler,    /* Vector - 82    interrupt_handler*/
	interrupt_handler,    /* Vector - 83    Reserved */
	interrupt_handler,    /* Vector - 84    interrupt_handler*/
	interrupt_handler,    /* Vector - 85    interrupt_handler*/
	interrupt_handler,    /* Vector - 86    interrupt_handler*/
	interrupt_handler,    /* Vector - 87    Reserved */
	interrupt_handler,    /* Vector - 88    interrupt_handler*/
	interrupt_handler,    /* Vector - 89    interrupt_handler*/
	interrupt_handler,    /* Vector - 90    interrupt_handler*/
	interrupt_handler,    /* Vector - 91    interrupt_handler*/
	interrupt_handler,    /* Vector - 92    interrupt_handler*/
	interrupt_handler,    /* Vector - 93    interrupt_handler*/
	interrupt_handler,    /* Vector - 94    interrupt_handler*/
	interrupt_handler,    /* Vector - 95    interrupt_handler*/
	interrupt_handler,    /* Vector - 96    interrupt_handler*/
	interrupt_handler,    /* Vector - 97    interrupt_handler*/
	interrupt_handler,    /* Vector - 98    interrupt_handler*/
    MUART_RX1_ISR,        /* Vector - 99     MUART_RX1_ISR*/
    MUART_TX1_ISR,        /* Vector - 100    MUART_TX1_ISR*/
	interrupt_handler,    /* Vector - 101    interrupt_handler*/
	interrupt_handler,    /* Vector - 102    Reserved */
	interrupt_handler,    /* Vector - 103    Reserved */
	interrupt_handler,    /* Vector - 104    Reserved */
	interrupt_handler,    /* Vector - 105    interrupt_handler*/
	interrupt_handler,    /* Vector - 106    interrupt_handler*/
	interrupt_handler,    /* Vector - 107    Reserved */
	interrupt_handler,    /* Vector - 108    interrupt_handler*/
	interrupt_handler,    /* Vector - 109    interrupt_handler*/
	interrupt_handler,    /* Vector - 110    interrupt_handler*/
	interrupt_handler,    /* Vector - 111    interrupt_handler*/
	interrupt_handler,    /* Vector - 112    interrupt_handler*/
	interrupt_handler,    /* Vector - 113    interrupt_handler*/
	interrupt_handler,    /* Vector - 114    interrupt_handler*/
	interrupt_handler,    /* Vector - 115    interrupt_handler*/
	interrupt_handler,    /* Vector - 116    interrupt_handler*/
	interrupt_handler,    /* Vector - 117    interrupt_handler*/
	interrupt_handler,    /* Vector - 118    interrupt_handler*/
    YUART_RX2_ISR,      /* Vector - 119    YUART_RX2_ISR*/
    YUART_TX2_ISR,      /* Vector - 120    YUART_TX2_ISR*/
	interrupt_handler,    /* Vector - 121    interrupt_handler*/
	interrupt_handler,    /* Vector - 122    interrupt_handler*/
	interrupt_handler,    /* Vector - 123    interrupt_handler*/
	interrupt_handler,    /* Vector - 124    interrupt_handler*/
	interrupt_handler,    /* Vector - 125    interrupt_handler*/
	interrupt_handler,    /* Vector - 126    Reserved */
	FUSS_PIT3_ISR,        /* Vector - 127    FUSS_PIT3_ISR*/
	interrupt_handler,    /* Vector - 128    interrupt_handler*/
	interrupt_handler,    /* Vector - 129    interrupt_handler*/
	interrupt_handler,    /* Vector - 130    interrupt_handler*/
	interrupt_handler,    /* Vector - 131    interrupt_handler*/
	interrupt_handler,    /* Vector - 132    Reserved */
	interrupt_handler,    /* Vector - 133    Reserved */
	interrupt_handler,    /* Vector - 134    Reserved */
	interrupt_handler,    /* Vector - 135    Reserved */
	interrupt_handler,    /* Vector - 136    Reserved */
	interrupt_handler,    /* Vector - 137    Reserved */
	interrupt_handler,    /* Vector - 138    Reserved */
	interrupt_handler,    /* Vector - 139    Reserved */
	interrupt_handler,    /* Vector - 140    Reserved */
	interrupt_handler,    /* Vector - 141    interrupt_handler*/
	interrupt_handler,    /* Vector - 142    interrupt_handler*/
	interrupt_handler,    /* Vector - 143    interrupt_handler*/
	interrupt_handler,    /* Vector - 144    interrupt_handler*/
	interrupt_handler,    /* Vector - 145    interrupt_handler*/
	interrupt_handler,    /* Vector - 146    interrupt_handler*/
	interrupt_handler,    /* Vector - 147    interrupt_handler*/
	interrupt_handler,    /* Vector - 148    interrupt_handler*/
	interrupt_handler,    /* Vector - 149    interrupt_handler*/
	interrupt_handler,    /* Vector - 150    interrupt_handler*/
	interrupt_handler,    /* Vector - 151    interrupt_handler*/
	interrupt_handler,    /* Vector - 152    interrupt_handler*/
	interrupt_handler,    /* Vector - 153    interrupt_handler*/
	interrupt_handler,    /* Vector - 154    interrupt_handler*/
	interrupt_handler,    /* Vector - 155    interrupt_handler*/
	interrupt_handler,    /* Vector - 156    interrupt_handler*/
	interrupt_handler,    /* Vector - 157    interrupt_handler*/
	interrupt_handler,    /* Vector - 158    interrupt_handler*/
	interrupt_handler,    /* Vector - 159    interrupt_handler*/
	interrupt_handler,    /* Vector - 160    interrupt_handler*/
	interrupt_handler,    /* Vector - 161    interrupt_handler*/
    FUSS_EMIOS1_F10_F11_ISR,    /* Vector - 162    FUSS_EMIOS1_F10_F11_ISR*/
    FUSS_EMIOS1_F12_F13_ISR,    /* Vector - 163    FUSS_EMIOS1_F12_F13_ISR*/
    FUSS_EMIOS1_F14_F15_ISR,    /* Vector - 164    FUSS_EMIOS1_F14_F15_ISR*/
	interrupt_handler,    /* Vector - 165    interrupt_handler*/
	interrupt_handler,    /* Vector - 166    interrupt_handler*/
	interrupt_handler,    /* Vector - 167    interrupt_handler*/
	interrupt_handler,    /* Vector - 168    interrupt_handler*/
	interrupt_handler,    /* Vector - 169    interrupt_handler*/
	interrupt_handler,    /* Vector - 170    interrupt_handler*/
	interrupt_handler,    /* Vector - 171    interrupt_handler*/
	interrupt_handler,    /* Vector - 172    interrupt_handler*/
	interrupt_handler,    /* Vector - 173    interrupt_handler*/
	interrupt_handler,    /* Vector - 174    interrupt_handler*/
	interrupt_handler,    /* Vector - 175    Reserved */
	interrupt_handler,    /* Vector - 176    interrupt_handler*/
	interrupt_handler,    /* Vector - 177    interrupt_handler*/
	interrupt_handler,    /* Vector - 178    interrupt_handler*/
	interrupt_handler,    /* Vector - 179    interrupt_handler*/
	interrupt_handler,    /* Vector - 180    interrupt_handler*/
	interrupt_handler,    /* Vector - 181    interrupt_handler*/
	interrupt_handler,    /* Vector - 182    interrupt_handler*/
	interrupt_handler,    /* Vector - 183    interrupt_handler*/
	interrupt_handler,    /* Vector - 184    interrupt_handler*/
	interrupt_handler,    /* Vector - 185    interrupt_handler*/
	interrupt_handler,    /* Vector - 186    interrupt_handler*/
	interrupt_handler,    /* Vector - 187    interrupt_handler*/
	interrupt_handler,    /* Vector - 188    interrupt_handler*/
	interrupt_handler,    /* Vector - 189    interrupt_handler*/
	interrupt_handler,    /* Vector - 190    interrupt_handler*/
	interrupt_handler,    /* Vector - 191    interrupt_handler*/
	interrupt_handler,    /* Vector - 192    Reserved */
	interrupt_handler,    /* Vector - 193    interrupt_handler*/
	interrupt_handler,    /* Vector - 194    interrupt_handler*/
	interrupt_handler,    /* Vector - 195    interrupt_handler*/
	interrupt_handler,    /* Vector - 196    interrupt_handler*/
	interrupt_handler,    /* Vector - 197    interrupt_handler*/
	interrupt_handler,    /* Vector - 198    interrupt_handler*/
	interrupt_handler,    /* Vector - 199    interrupt_handler*/
	interrupt_handler,    /* Vector - 200    interrupt_handler*/
	interrupt_handler,    /* Vector - 201    interrupt_handler*/
	interrupt_handler,    /* Vector - 202    interrupt_handler*/
	interrupt_handler,    /* Vector - 203    interrupt_handler*/
	interrupt_handler,    /* Vector - 204    Reserved */
	interrupt_handler,    /* Vector - 205    interrupt_handler*/
	interrupt_handler,    /* Vector - 206    interrupt_handler*/
	interrupt_handler,    /* Vector - 207    interrupt_handler*/
	interrupt_handler,    /* Vector - 208    interrupt_handler*/
	interrupt_handler,    /* Vector - 209    interrupt_handler*/
	interrupt_handler,    /* Vector - 210    interrupt_handler*/
	interrupt_handler,    /* Vector - 211    Reserved */
	interrupt_handler,    /* Vector - 212    interrupt_handler*/
	interrupt_handler,    /* Vector - 213    interrupt_handler*/
	interrupt_handler,    /* Vector - 214    interrupt_handler*/
	interrupt_handler,    /* Vector - 215    interrupt_handler*/
	interrupt_handler,    /* Vector - 216    Reserved */
	interrupt_handler,    /* Vector - 217    Reserved */
	interrupt_handler,    /* Vector - 218    Reserved */
	interrupt_handler,    /* Vector - 219    Reserved */
	interrupt_handler,    /* Vector - 220    Reserved */
	interrupt_handler,    /* Vector - 221    Reserved */
	interrupt_handler,    /* Vector - 222    Reserved */
	interrupt_handler,    /* Vector - 223    Reserved */
	interrupt_handler,    /* Vector - 224    Reserved */
	interrupt_handler,    /* Vector - 225    Reserved */
	interrupt_handler,    /* Vector - 226    Reserved */
	interrupt_handler,    /* Vector - 227    Reserved */
	interrupt_handler,    /* Vector - 228    Reserved */
	interrupt_handler,    /* Vector - 229    Reserved */
	interrupt_handler,    /* Vector - 230    Reserved */
	interrupt_handler,    /* Vector - 231    Reserved */
	interrupt_handler,    /* Vector - 232    Reserved */
};

#endif

#pragma pop

#pragma push /* save the current state */
#pragma force_active on
#pragma function_align 16 /* We use 16 bytes alignment for Exception handlers */
/** Handle the interrupt source by jumping to the ISR branch table (IACKR) */

#if INTC_NESTED_INTERRUPT == 0

__declspec(interrupt)
__declspec(section ".__exception_handlers")
void INTC_INTCInterruptHandler(void)
{
    INTCInterruptFn *handlerFn = (INTCInterruptFn*)(*(unsigned int*)&INTC.IACKR.R);

    (**handlerFn)();

    INTC.EOIR.R = 0;
}

#else

__declspec(interrupt)
__declspec(section ".__exception_handlers")
__asm void INTC_INTCInterruptHandler(void)
{
nofralloc
prolog:
    stwu    r1, -0x50 (r1)    /* Create stack frame */
    stw r0,  0x24 (r1)        /* Store r0 working register  */

    /* Save SRR0 and SRR1 */
    mfsrr1  r0                /* Store SRR1 (must be done before enabling EE) */
    stw     r0,  0x10 (r1)
    mfsrr0  r0                /* Store SRR0 (must be done before enabling EE) */
    stw     r0,  0x0C (r1)

    /* Clear request to processor; r3 contains the address of the ISR */
    stw     r3,  0x28 (r1)    /* Store r3 */
    lis     r3, INTC_IACKR@h  /* Read pointer into ISR Vector Table & store in r3     */
    ori     r3, r3, INTC_IACKR@l
    lwz     r3, 0x0(r3)       /* Load INTC_IACKR, which clears request to processor   */
    lwz     r3, 0x0(r3)       /* Read ISR address from ISR Vector Table using pointer */

    /* Enable processor recognition of interrupts */
    wrteei  1                   /* Set MSR[EE]=1  */

    /* Save rest of context required by EABI */
    stw     r12, 0x4C (r1)      /* Store r12 */
    stw     r11, 0x48 (r1)      /* Store r11 */
    stw     r10, 0x44 (r1)      /* Store r10 */
    stw     r9,  0x40 (r1)      /* Store r9 */
    stw     r8,  0x3C (r1)      /* Store r8 */
    stw     r7,  0x38 (r1)      /* Store r7 */
    stw     r6,  0x34 (r1)      /* Store r6 */
    stw     r5,  0x30 (r1)      /* Store r5 */
    stw     r4,  0x2C (r1)      /* Store r4 */
    mfcr    r0                  /* Store CR */
    stw     r0,  0x20 (r1)
    mfxer   r0                  /* Store XER */
    stw     r0,  0x1C (r1)
    mfctr   r0                  /* Store CTR */
    stw     r0,  0x18 (r1)
    mflr    r0                  /* Store LR */
    stw     r0,  0x14 (r1)

    /* Branch to ISR handler address from SW vector table */
    mtlr    r3                  /* Store ISR address to LR to use for branching later */
    blrl                        /* Branch to ISR, but return here */

epilog:
    /* Restore context required by EABI (except working registers) */
    lwz     r0,  0x14 (r1)      /* Restore LR */
    mtlr    r0
    lwz     r0,  0x18 (r1)      /* Restore CTR */
    mtctr   r0
    lwz     r0,  0x1C (r1)      /* Restore XER */
    mtxer   r0
    lwz     r0,  0x20 (r1)      /* Restore CR */
    mtcrf   0xff, r0
    lwz     r5,  0x30 (r1)      /* Restore r5 */
    lwz     r6,  0x34 (r1)      /* Restore r6 */
    lwz     r7,  0x38 (r1)      /* Restore r7 */
    lwz     r8,  0x3C (r1)      /* Restore r8 */
    lwz     r9,  0x40 (r1)      /* Restore r9 */
    lwz     r10, 0x44 (r1)      /* Restore r10 */
    lwz     r11, 0x48 (r1)      /* Restore r11 */
    lwz     r12, 0x4C (r1)      /* Restore r12 */

    /* Disable processor recognition of interrupts */
    wrteei  0

    /* Ensure interrupt flag has finished clearing */
    mbar    0

    /* Write 0 to INTC_EOIR, informing INTC to lower priority */
    li      r3, 0
    lis     r4, INTC_EOIR@h     /* Load upper half of INTC_EOIR address to r4 */
    ori     r4, r4, INTC_EOIR@l
    stw     r3, 0(r4)           /* Write 0 to INTC_EOIR */

    /* Restore Working Registers */
    lwz     r3,  0x28 (r1)      /* Restore r3 */
    lwz     r4,  0x2C (r1)      /* Restore r4 */

    /* Retrieve SRR0 and SRR1 */
    lwz     r0,  0x0C (r1)      /* Restore SRR0 */
    mtsrr0  r0
    lwz     r0,  0x10 (r1)      /* Restore SRR1 */
    mtsrr1  r0

    /* Restore Other Working Registers */
    lwz     r0,  0x24 (r1)      /* Restore r0 */

    /* Restore space on stack */
    addi    r1, r1, 0x50

    /* End of Interrupt */
    rfi
}

#endif

#pragma force_active off
#pragma pop

/**
 * This function can be used to install an interrupt handler for a given
 * interrupt vector. It will also set the Priority Status Register for the
 * source to the one given
 */
void INTC_InstallINTCInterruptHandler(INTCInterruptFn handlerFn, unsigned short vectorNum,
                                      unsigned char psrPriority)
{
    /* Set the function pointer in the ISR Handler table */
    INTCInterruptsHandlerTable[vectorNum] = handlerFn;
    /* Set the PSR Priority */
    INTC.PSR[vectorNum].B.PRI = psrPriority; 
}

/**
 * This function will setup the PowerPC Zen core to jump to an Interrupt Service Routine
 * handler. It can be used from user_init() (no stack frame, no memory access).
 * The INTC module will be configured for Software mode and vector table entry size of 4 bytes. 
*/
__asm void INTC_InitINTCInterrupts( void )
{
    nofralloc

    /* Set the location of the ISR Handler Table in INTC IACKR Register */
    lis r0, INTCInterruptsHandlerTable@h
    ori r0, r0, INTCInterruptsHandlerTable@l
    lis r3, INTC_IACKR@ha
    stw r0, INTC_IACKR@l(r3)

    /* Set INTC.MCR.B.HVEN = 0 for SW vector mode and
	INTC.MCR.B.VTES = 0 for 4-byte branch table entries.
     */
    lis r0, 0
    lis r3,INTC_MCR@ha 
    stw r0, INTC_MCR@l(r3)

#if NGV_DMU_PROJECT == 0    
    /* Enable external interrupts by setting MSR[EE]=1. */
    wrteei  1
#endif

	blr
}

#if NGV_DMU_PROJECT == 1
/********************** DUMMY INTERRUPT HANDLER **********************/
void interrupt_handler (void)
{

}
#endif



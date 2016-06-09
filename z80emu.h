/* z80emu.h
 * Main header of z80emu. Don't modify this file directly. Use z80config.h and
 * z80user.h to customize the emulator to your need. 
 *
 * Copyright (c) 2012-2016 Lin Ke-Fong
 *
 * This program is free, do whatever you want with it.
 */

#ifndef __Z80EMU_INCLUDED__
#define __Z80EMU_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#include "z80config.h"

/* Flags for Z80_STATE's status member. If the emulation is interrupted, status
 * can indicate why. You may add additionnal flags for your own use as needed.
 */

#define Z80_STATUS_FLAG_HALT            (1 << 0)
#define Z80_STATUS_FLAG_DI              (1 << 1)
#define Z80_STATUS_FLAG_EI              (1 << 2)
#define Z80_STATUS_FLAG_RETI            (1 << 3)
#define Z80_STATUS_FLAG_RETN            (1 << 4)
#define Z80_STATUS_FLAG_ED_UNDEFINED    (1 << 5)
 
/* The main registers are stored inside Z80_STATE as an union of arrays named 
 * registers. They are referenced using indexes. Words are stored in the 
 * endianness of the host processor. The alternate set of word registers AF', 
 * BC', DE', and HL' is stored in the alternates member of Z80_STATE, as an 
 * array using the same ordering.
 */

#ifdef Z80_BIG_ENDIAN

#       define Z80_B            0
#       define Z80_C            1
#       define Z80_D            2
#       define Z80_E            3
#       define Z80_H            4
#       define Z80_L            5
#       define Z80_A            6
#       define Z80_F            7

#       define Z80_IXH          8
#       define Z80_IXL          9
#       define Z80_IYH          10
#       define Z80_IYL          11

#else

#       define Z80_B            1
#       define Z80_C            0
#       define Z80_D            3
#       define Z80_E            2
#       define Z80_H            5
#       define Z80_L            4
#       define Z80_A            7
#       define Z80_F            6

#       define Z80_IXH          9
#       define Z80_IXL          8
#       define Z80_IYH          11
#       define Z80_IYL          10
                                
#endif

#define Z80_BC                  0
#define Z80_DE                  1
#define Z80_HL                  2
#define Z80_AF                  3

#define Z80_IX                  4
#define Z80_IY                  5 
#define Z80_SP                  6

/* Z80's flags. */

#define Z80_S_FLAG_SHIFT        7       
#define Z80_Z_FLAG_SHIFT        6
#define Z80_Y_FLAG_SHIFT        5
#define Z80_H_FLAG_SHIFT        4
#define Z80_X_FLAG_SHIFT        3
#define Z80_PV_FLAG_SHIFT       2
#define Z80_N_FLAG_SHIFT        1
#define Z80_C_FLAG_SHIFT        0

#define Z80_S_FLAG              (1 << Z80_S_FLAG_SHIFT)
#define Z80_Z_FLAG              (1 << Z80_Z_FLAG_SHIFT)
#define Z80_Y_FLAG              (1 << Z80_Y_FLAG_SHIFT)
#define Z80_H_FLAG              (1 << Z80_H_FLAG_SHIFT)
#define Z80_X_FLAG              (1 << Z80_X_FLAG_SHIFT)
#define Z80_PV_FLAG             (1 << Z80_PV_FLAG_SHIFT)
#define Z80_N_FLAG              (1 << Z80_N_FLAG_SHIFT)
#define Z80_C_FLAG              (1 << Z80_C_FLAG_SHIFT)

#define Z80_P_FLAG_SHIFT        Z80_PV_FLAG_SHIFT
#define Z80_V_FLAG_SHIFT        Z80_PV_FLAG_SHIFT
#define Z80_P_FLAG              Z80_PV_FLAG
#define Z80_V_FLAG              Z80_PV_FLAG

/* Z80's three interrupt modes. */

enum {

        Z80_INTERRUPT_MODE_0,
        Z80_INTERRUPT_MODE_1,
        Z80_INTERRUPT_MODE_2

};

/* Z80 processor's state. You may add your own members if needed. However, it
 * is rather recommended to use the context pointer passed to the emulation 
 * functions. See z80user.h.
 */ 

typedef struct Z80_STATE {

        int             status;

        union {

                unsigned char   byte[14];
                unsigned short  word[7];

        } registers;

        unsigned short  alternates[4];

        int             i, r, pc, iff1, iff2, im;
        
        /* Register decoding table. */

        void            *register_table[16], 
                        *dd_register_table[16], 
                        *fd_register_table[16];        

} Z80_STATE;

/* Initialize processor's state to power-on default and reset status. */

extern void     Z80Reset (Z80_STATE *state);

/* Trigger an interrupt according to the current interrupt mode and return the
 * number of cycles required to accept it. If maskable interrupts are disabled,
 * this will return zero. Z80_STATE's status is updated. In interrupt mode 0,
 * data_on_bus must be a single byte opcode.
 */

extern int      Z80Interrupt (Z80_STATE *state, 
			int data_on_bus, 
			void *context);

/* Trigger a non maskable interrupt, return the number of cycles needed to
 * accept it. Z80_STATE's status is reset.
 */

extern int      Z80NonMaskableInterrupt (Z80_STATE *state, void *context);

/* Emulate a Z80 processor for number_cycles cycles, which must be greater or
 * equal to 4. Return the number of emulated cycles, this number will be equal
 * to number_cycles or be slightly greater (most probable case). It will be
 * less if the emulation has been interrupted. Z80_STATE's status member is
 * reset at the start of the emulation. If the emulation is interrupted, it may
 * indicate the reason why.
 */

extern int      Z80Emulate (Z80_STATE *state, 
			int number_cycles, 
			void *context);

#ifdef __cplusplus
}
#endif

#endif

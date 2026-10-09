#ifndef GAME_KEYPAD_H
#define GAME_KEYPAD_H

#include "types.h"

/* Key bits: KEYINPUT (0x04000130) bits 0-9, with X and Y placed at bits 10 and 11. */
#define PAD_BUTTON_A      0x0001
#define PAD_BUTTON_B      0x0002
#define PAD_BUTTON_SELECT 0x0004
#define PAD_BUTTON_START  0x0008
#define PAD_KEY_RIGHT     0x0010
#define PAD_KEY_LEFT      0x0020
#define PAD_KEY_UP        0x0040
#define PAD_KEY_DOWN      0x0080
#define PAD_BUTTON_R      0x0100
#define PAD_BUTTON_L      0x0200
#define PAD_BUTTON_X      0x0400
#define PAD_BUTTON_Y      0x0800

/* Key state of the current frame, reached through gKeypad. */
typedef struct Keypad {
    u16 unk_00;
    u16 repeat;                        /* menus move the cursor on these keys; probably newly pressed plus auto-repeat */
    u16 pressed;                       /* menus confirm and cancel on these keys; probably newly pressed only */
    u8 pad_06[6];
    u16 unk_0c;
} Keypad;

extern Keypad *gKeypad;

#endif

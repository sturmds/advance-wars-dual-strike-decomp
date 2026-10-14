#ifndef GAME_KEYPAD_H
#define GAME_KEYPAD_H

#include "types.h"

/* Key bits: KEYINPUT (0x04000130) bits 0-9, with X and Y placed at bits 10 and 11. */
#define PAD_BUTTON_A 0x0001
#define PAD_BUTTON_B 0x0002
#define PAD_BUTTON_SELECT 0x0004
#define PAD_BUTTON_START 0x0008
#define PAD_KEY_RIGHT 0x0010
#define PAD_KEY_LEFT 0x0020
#define PAD_KEY_UP 0x0040
#define PAD_KEY_DOWN 0x0080
#define PAD_BUTTON_R 0x0100
#define PAD_BUTTON_L 0x0200
#define PAD_BUTTON_X 0x0400
#define PAD_BUTTON_Y 0x0800

/* Entries of a table indexed by the 4 d-pad bits (data_02169454). */
#define DPAD_PATTERN_COUNT 16

/* Key state of the current frame, reached through gKeypad. */
typedef struct Keypad {
  u16 held;   /* keys down this frame */
  u16 repeat; /* newly pressed, plus held keys each time they auto-repeat; menus move the cursor on these */
  u16 pressed; /* newly pressed this frame; menus confirm and cancel on these */
  u16 prevHeld; /* held in the frame before */
  u16 nextHeld; /* Keypad_UpdateState works in these four, then copies them to the four above */
  u16 nextRepeat;
  u16 nextPressed;
  u16 nextPrevHeld;
  u16 repeatCountdown; /* frames until the held keys repeat */
  u16 repeatDelay;     /* frames before the first repeat */
  u16 repeatInterval;  /* frames between repeats */
} Keypad;

extern Keypad* gKeypad;

#endif

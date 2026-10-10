#ifndef GAME_OV003_INPUT_FRAME_H
#define GAME_OV003_INPUT_FRAME_H

#include "types.h"

/* Input of one frame in overlay 3 (data_ov003_02365b04), 8 bytes. When
 * Unk_02160ac0.unk_7c is set, func_0207b4b0 copies it into the 8-byte link
 * send buffer data_0229ca48. */
typedef struct Ov003InputFrame {
    u8 touchPhase;                     /* TouchState.phase */
    u8 touchActive;                    /* low byte of TouchState.active */
    u8 touchX;
    u8 touchY;
    u16 keys;                          /* Keypad.unk_00 */
    u8 pad_06[2];
} Ov003InputFrame;

extern Ov003InputFrame data_ov003_02365b04;

#endif

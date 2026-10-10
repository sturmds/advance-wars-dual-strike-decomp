#ifndef GAME_UNK_OV005_02350E2C_H
#define GAME_UNK_OV005_02350E2C_H

#include "types.h"

struct Task;
struct TextPrinter;

/* Work block (Ov005Task.work) of the overlay 5 screen that
 * Ov005_UpdateSlideInTextScreen updates: it scrolls two BG layers in, waits for a
 * text and an A press or the button, then fades out. */
typedef struct Unk_ov005_02350e2c {
    int frame;                         /* scroll-in progress, 0 to frames */
    int frames;
    BOOL buttonPressed;                /* cleared when button 253 is added */
    u8 pad_0c[0x14 - 0x0c];
    struct TextPrinter *printer;
    struct Task *fadeTask;             /* from the func_020fcd78 family */
    struct Task *fadeTask2;
    u8 pad_20[0x24 - 0x20];
    struct Task *childTask;            /* runs data_ov005_02368048 */
    u32 unk_28;
} Unk_ov005_02350e2c;

#endif

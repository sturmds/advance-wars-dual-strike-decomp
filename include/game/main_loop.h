#ifndef GAME_MAIN_LOOP_H
#define GAME_MAIN_LOOP_H

#include "types.h"

/* gMainLoop (DTCM): state of the main loop step func_01ff9c7c, which
 * runs once per frame and ends with VBlankIntrWait. */
typedef struct MainLoop {
    void (*screenCallbacks[2])(void);  /* called once per frame on each screen when set */
    void (*vblankCallback)(void);      /* called by func_01ff9bd0 when set */
    u8 pad_0c[0x18 - 0x0c];
    u32 frameCount;                    /* func_01ff9c7c adds 1 every frame */
    u8 pad_1c[0x24 - 0x1c];
    int unk_24;                        /* 0, 1 or 2: set by func_01ff9610, func_01ff95fc and func_01ff94fc */
    u16 prevKeys;                      /* keys seen last frame, for the reset combo */
} MainLoop;

#endif

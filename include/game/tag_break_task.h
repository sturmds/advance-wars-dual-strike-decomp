#ifndef GAME_TAG_BREAK_TASK_H
#define GAME_TAG_BREAK_TASK_H

#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

/* Script task of the tag power cut-in, as its later steps use it. The
 * first 0x4c bytes are the Task. */
typedef struct TagBreakTask {
    u8 pad_00[0x4c];
    s16 frame;                         /* frame counter of the running step */
    u8 pad_4e[0x58 - 0x4e];
    int frameScroll1;                  /* BG1 (frame) scroll; screen 0 (data_027c0288.unk_00) adds 192 */
    int frameScroll2;                  /* BG2 (frame) scroll, same */
} TagBreakTask;

/* Sets the Y scroll of the BG that `layerBg` names on `screen` to `value`,
 * then restores the current screen. */
#define TAG_BREAK_SET_SCROLL_Y(screen, layerBg, value)                         \
    {                                                                          \
        Unk_027c0280 *saved0280 = data_027c0280;                               \
        Unk_02080440 *saved027c;                                               \
        Screen_SetCurrentGfx(screen);                                          \
        saved027c = data_027c027c;                                             \
        Screen_SetCurrent(screen);                                             \
        data_027c0280->bgScrollY[data_027c0280->layerBg] = (value);            \
        Screen_SetCurrent(saved027c);                                          \
        Screen_SetCurrentGfx(saved0280);                                       \
    }

#endif

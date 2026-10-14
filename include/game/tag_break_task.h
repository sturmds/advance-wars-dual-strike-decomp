#ifndef GAME_TAG_BREAK_TASK_H
#define GAME_TAG_BREAK_TASK_H

#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

/* Parts of the res_tagbreak and res_tagchange files that TagBreak_SetUpScreens
 * and TagPowerScreen_SetUp copy from the main screen to the sub screen: tile
 * data of the BG3 backdrop (res_tagbreak_black, _union, _mix), of the frame
 * on BG2 and BG1 (res_tagbreak) and of the BG0 layer (res_tagchange), which
 * also has three palettes, for BG palettes 3-5. The CO pictures fill object
 * VRAM from TAG_BREAK_OBJ_TILES_START. */
#define TAG_BREAK_BACKDROP_TILES_SIZE 0x6000
#define TAG_BREAK_BG2_TILES_SIZE 0x4000
#define TAG_BREAK_BG1_TILES_SIZE 0x100
#define TAG_CHANGE_TILES_SIZE 0x6000
#define TAG_CHANGE_PALETTE_COUNT 3
#define TAG_BREAK_OBJ_TILES_START 0x200
#define TAG_BREAK_OBJ_TILES_SIZE 0xb400

/* Script task of the tag power cut-in, as its later steps use it. The
 * first 0x4c bytes are the Task. */
typedef struct TagBreakTask {
  u8 pad_00[0x4c];
  s16 frame; /* frame counter of the running step */
  u8 pad_4e[0x58 - 0x4e];
  int frameScroll1; /* BG1 (frame) scroll; screen 0 (data_027c0288.unk_00) adds 192 */
  int frameScroll2; /* BG2 (frame) scroll, same */
} TagBreakTask;

/* Sets the Y scroll of the BG that `layerBg` names on `screen` to `value`,
 * then restores the current screen. */
#define TAG_BREAK_SET_SCROLL_Y(screen, layerBg, value)          \
  {                                                             \
    Unk_027c0280* saved0280 = data_027c0280;                    \
    Unk_02080440* saved027c;                                    \
    Screen_SetCurrentGfx(screen);                               \
    saved027c = data_027c027c;                                  \
    Screen_SetCurrent(screen);                                  \
    data_027c0280->bgScrollY[data_027c0280->layerBg] = (value); \
    Screen_SetCurrent(saved027c);                               \
    Screen_SetCurrentGfx(saved0280);                            \
  }

void TagBreak_DrawSidePieceFacing(int x, int y, int side);

/* Redraws the two side pieces (TagBreak_DrawSidePieceFacing) and scrolls the
 * frame layers on both screens, BG1 by 24 and BG2 by 12 pixels a frame.
 * Screen 0 shows the frame 192 pixels further on. */
static inline void TagBreakTask_ScrollFrame(TagBreakTask* task) {
  TagBreak_DrawSidePieceFacing(64, 224, 0);
  TagBreak_DrawSidePieceFacing(192, 224, 1);
  task->frameScroll1 += 24;
  task->frameScroll2 += 12;
  TAG_BREAK_SET_SCROLL_Y(data_027c0288.unk_00, layer1Bg,
                         task->frameScroll1 + 192);
  TAG_BREAK_SET_SCROLL_Y(data_027c0288.unk_04, layer1Bg, task->frameScroll1);
  TAG_BREAK_SET_SCROLL_Y(data_027c0288.unk_00, layer2Bg,
                         task->frameScroll2 + 192);
  TAG_BREAK_SET_SCROLL_Y(data_027c0288.unk_04, layer2Bg, task->frameScroll2);
}

#endif

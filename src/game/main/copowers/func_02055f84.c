#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/tag_break_task.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrentBoth(void* state);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void TagBreak_DrawSidePiece(int x, int y, int side);
s16 BreakText_GetScreenX(u8 slot);
void BreakText_QueueLetter(u8 slot, u16 x, int y, int attr, int arg);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Palette_BlendWithColor(u16* palette, u16 color, u8 palWeight,
                            u8 colorWeight);
void Palette_Flush(void);
void Task_ResumeScript(Task* task);

extern u8
    gTagChangePalette[]; /* palette rows 3-5 as loaded, from res_tagchange */

#define FRAME(task) (*(s16*)&(task)->unk_4c)
/* Slide offset kept in the task's free bytes at 0x2c. */
#define SLIDE(task) (*(int*)(task)->pad_2c)

#define SLIDE_FRAMES 26
#define TAG_CHANGE_FIRST_PALETTE 3 /* gTagChangePalette holds BG palettes 3-5 */

#define SET_BLEND()                                          \
  data_027c0280->blendMode = BLEND_MODE_ALPHA;               \
  data_027c0280->blendEva = 0;                               \
  data_027c0280->blendEvb = 16;                              \
  data_027c0280->blendEvy = (u16)data_027c0280->blendEvy;    \
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1;    \
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;    \
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;    \
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;    \
  data_027c0280->blendTopObj = 0;                            \
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0; \
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 1; \
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1; \
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1; \
  data_027c0280->blendBottomObj = 1;                         \
  data_027c0280->dispCnt |= 0x100;                           \
  data_027c0280->dispCnt |= 0x200;                           \
  data_027c0280->dispCnt |= 0x400;                           \
  data_027c0280->dispCnt |= 0x800;                           \
  data_027c0280->dispCnt |= 0x1000

#define WHITEN_PALETTES(palWeight, colorWeight)                   \
  Mem_Copy32(palette,                                             \
             data_027c0280->paletteBuffer +                       \
                 TAG_CHANGE_FIRST_PALETTE * PALETTE_16_SIZE,      \
             TAG_CHANGE_PALETTE_COUNT * PALETTE_16_SIZE);         \
  for (i = 0; i < TAG_CHANGE_PALETTE_COUNT; i++) {                \
    Palette_BlendWithColor(                                       \
        (u16*)(data_027c0280->paletteBuffer +                     \
               (i + TAG_CHANGE_FIRST_PALETTE) * PALETTE_16_SIZE), \
        0x7fff, palWeight, colorWeight);                          \
  }                                                               \
  Palette_Flush()

/* Script task step, close to func_020593c8. Frame 0 sets both screens to
 * alpha-blend BG0 over BG1-BG3 and OBJ. Every frame the two pieces drawn by
 * TagBreak_DrawSidePiece slide apart (eased over SLIDE_FRAMES frames) and task->unk_58
 * text sprites are redrawn at y 80. Frames 1-16 raise eva with the frame,
 * fading BG0 in. Frames 17-25 blend palette rows 3-5 (saved in
 * gTagChangePalette) toward white on both screens; frame 26 makes them fully
 * white and resumes the script.
 * Needed to match: gTagChangePalette goes through a local set after the first
 * Screen_SetCurrentBoth (found by tools/permuter.py). */
void func_02055f84(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  u8* palette; /* gTagChangePalette, read once for both screens */
  int i;
  int level;

  if (FRAME(task) == 0) {
    saved0280 = data_027c0280;
    saved027c = data_027c027c;
    Screen_SetCurrentBoth(data_027c0288.unk_00);
    SET_BLEND();
    Screen_SetCurrentBoth(data_027c0288.unk_04);
    SET_BLEND();
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
  FRAME(task)++;
  SLIDE(task) = Math_Ease(1, 0, 128, FRAME(task), SLIDE_FRAMES);
  TagBreak_DrawSidePiece(72 - SLIDE(task), 224, 0);
  TagBreak_DrawSidePiece(SLIDE(task) + 184, 224, 1);
  for (i = 0; i < task->unk_58; i++) {
    BreakText_QueueLetter(i, BreakText_GetScreenX(i) & 0x1ff, 80, 0, 0);
  }
  if (FRAME(task) <= 16) {
    saved0280 = data_027c0280;
    saved027c = data_027c027c;
    Screen_SetCurrentBoth(data_027c0288.unk_00);
    data_027c0280->blendEva = FRAME(task);
    Screen_SetCurrentBoth(data_027c0288.unk_04);
    data_027c0280->blendEva = FRAME(task);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    return;
  }
  switch (FRAME(task)) {
    case 17:
      level = 1;
      break;
    case 18:
      level = 2;
      break;
    case 19:
      level = 3;
      break;
    case 20:
      level = 4;
      break;
    case 21:
      level = 5;
      break;
    case 22:
      level = 7;
      break;
    case 23:
      level = 9;
      break;
    case 24:
      level = 11;
      break;
    case 25:
      level = 13;
      break;
    default:
      FRAME(task) = 0;
      level = 16;
      Task_ResumeScript(task);
      break;
  }
  saved0280 = data_027c0280;
  saved027c = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  palette = gTagChangePalette;
  WHITEN_PALETTES(16 - level, level);
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  WHITEN_PALETTES(16 - level, level);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}

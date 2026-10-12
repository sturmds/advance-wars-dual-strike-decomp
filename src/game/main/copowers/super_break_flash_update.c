#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

void Sound_PlaySe(u32 soundId);
void Sprite_QueueCoPortrait(int x, int y, int arg2, int arg3, int list,
                            BOOL flip);
s16 BreakText_GetCentredX(u8 slot);
void BreakText_QueueLetterSized(u8 slot, u16 x, int y, int attr, int arg);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Palette_BlendWithColor(u16* palette, u16 color, u8 palWeight,
                            u8 colorWeight);
void Palette_Flush(void);
void Task_ResumeScript(Task* task);

extern u8 gMenuState[]; /* SuperBreak_SetUpScreen saved BG palettes 1-3 here */

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

#define FADE_IN_FRAMES 16
#define WHITE 0x7fff
#define PALETTE_SIZE 32

/* Step of the Super CO Power cut-in script, after SuperBreak_BlendUpdate. Draws the
 * power name letters, raises layer 0's blend EVA to 16 over 16 frames, then
 * fades BG palettes 1-3 (from their saved copy) to white over 9 frames,
 * faster towards the end. Leaves them white and resumes the script. */
void SuperBreak_FlashUpdate(Task* task) {
  int i;
  int white;

  if (FRAME(task) == 0) {
    Sound_PlaySe(188);
  }
  FRAME(task)++;
  Sprite_QueueCoPortrait(184, 160, 0x800, 16, 2, 0);
  for (i = 0; i < task->unk_58; i++) {
    BreakText_QueueLetterSized(i, BreakText_GetCentredX(i) & 0x1ff, 80, 0, 1);
  }

  if (FRAME(task) <= FADE_IN_FRAMES) {
    data_027c0280->blendEva = FRAME(task);
    return;
  }

  switch (FRAME(task)) {
    case 17:
      white = 1;
      break;
    case 18:
      white = 2;
      break;
    case 19:
      white = 3;
      break;
    case 20:
      white = 4;
      break;
    case 21:
      white = 5;
      break;
    case 22:
      white = 7;
      break;
    case 23:
      white = 9;
      break;
    case 24:
      white = 11;
      break;
    case 25:
      white = 13;
      break;
    default:
      FRAME(task) = 0;
      white = 16;
      Task_ResumeScript(task);
      break;
  }

  Mem_Copy32(gMenuState, data_027c0280->paletteBuffer + 1 * PALETTE_SIZE,
             3 * PALETTE_SIZE);
  for (i = 0; i < 3; i++) {
    Palette_BlendWithColor(
        (u16*)(data_027c0280->paletteBuffer + (i + 1) * PALETTE_SIZE), WHITE,
        16 - white, white);
  }
  Palette_Flush();
}

#include "types.h"
#include "game/force_rank_screen.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/oam_cells.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Sprite_QueueDefault(int x, int y, int attr, int list);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void func_0200270c(int x, int y, int attr, int list, int hFlip);
int MainLoop_GetFrameCount(void);
void Palette_Load(const void* src, u32 offset, u32 size);
void ForceRankScreen_DrawRank(int x, int y, u8 rank);
void ForceRankScreen_DrawColumnHeads(void);
void ForceRankScreen_MarkChosenSkills(void);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

/* Queues the Force Rank screen's sprites each frame. Top screen: the rank
 * (ForceRankScreen_DrawRank), ForceRankScreen_DrawColumnHeads's sprites, the marks on chosen
 * skills (ForceRankScreen_MarkChosenSkills), the cursor on the selected slot
 * with its color changing every 4 frames, and the icon of each chosen skill
 * in the slots from x 160. Bottom screen: one sprite at (72, 160). Skill icons start at skill id 32. */
void ForceRankScreen_Draw(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  int i;
  int slotCount;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Sprite_QueueDefault(32, 9, 0x1000, 1);
  ForceRankScreen_DrawRank(0x51, 8, gForceRankScreen->rank);
  ForceRankScreen_DrawColumnHeads();
  ForceRankScreen_MarkChosenSkills();
  Sprite_QueueAt(2, (gForceRankScreen->selectedSlot * 16 + 160) & 0x1ff, 8,
                 &data_027c0040, 0x205a, 0);
  Palette_Load(&gForceRankScreen
                    ->cycleColors[((MainLoop_GetFrameCount() >> 2) & 15) * 2],
               600, 2);
  slotCount = gForceRankScreen->rank;
  if (slotCount > 4) {
    slotCount = 4;
  }
  for (i = 0; i < slotCount; i++) {
    if (gForceRankScreen->chosenSkills[i] != 0) {
      Sprite_QueueAt(1, (160 + i * 16) & 0x1ff, 8, &data_027c0040, 0,
                     (gForceRankScreen->chosenSkills[i] - 32) * 4 + 0x400);
    }
  }
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  func_0200270c(72, 160, 0x1800, 1, 1);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}

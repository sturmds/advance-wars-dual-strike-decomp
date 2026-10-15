#include "types.h"
#include "game/task.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7a9c.h"
#include "game/screen_other_map.h"

void ResultStats_DrawStat(int x, int y, int stat, Task* task);
void func_0203bdf0(int x, int y, int coSlot);
void func_0203be58(int x, int y, int coSlot);
void ResultStats_QueuePlayerSprite(int x, int y, int index);

/* Draws the sub screen sprites of the stats page of player task->unk_66,
 * moved right by `offset` pixels: the CO (and tag partner if any) and five
 * stat values. */
void ResultStats_DrawSubScreen(int offset, Task* task) {
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* saved;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  ResultStats_QueuePlayerSprite(offset + 24, 24, task->unk_66 - 1);
  func_0203be58(offset + 24, 40, (task->unk_66 - 1) * 2);
  func_0203bdf0(offset + 74, 41, (task->unk_66 - 1) * 2);
  if (data_022a7aba[task->unk_66].unk_00 & PLAYER_CO_ID_MASK) {
    func_0203be58(offset + 132, 40, (task->unk_66 - 1) * 2 + 1);
    func_0203bdf0(offset + 182, 41, (task->unk_66 - 1) * 2 + 1);
  }
  ResultStats_DrawStat(offset + 40, 102, 1, task);
  ResultStats_DrawStat(offset + 108, 102, 2, task);
  ResultStats_DrawStat(offset + 176, 102, 3, task);
  ResultStats_DrawStat(offset + 40, 136, 0, task);
  ResultStats_DrawStat(offset + 108, 136, 4, task);
  Screen_SetCurrent(saved);
  Screen_SetCurrentGfx(savedGfx);
}

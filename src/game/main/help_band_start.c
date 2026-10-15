#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/task.h"
#include "game/screen_other_map.h"

extern TaskScriptCmd data_021590ec[];

/* Starts the task (HelpBand_Update) that draws the help band of the lower
 * screen: a row of 16 sprite cells using the tiles from `tile`. A nonzero
 * `scrolls` makes it move layer 0 left by one pixel per frame. */
void HelpBand_Start(u16 tile, u16 scrolls) {
  Task* task;
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  task = Task_Create(data_021590ec, TASK_LIST(3));
  task->unk_52 = tile;
  task->unk_64 = scrolls;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}

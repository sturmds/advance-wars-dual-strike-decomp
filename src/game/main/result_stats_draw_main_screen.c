#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void ResultStats_QueuePlayerSprite(int x, int y, int index);
void ResultStats_DrawUnitGrid(int offset, Task* task);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

/* Draws the main screen sprites of the stats page of player task->unk_66:
 * the player sprite and the unit grid. */
void ResultStats_DrawMainScreen(int offset, Task* task) {
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* saved;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  ResultStats_QueuePlayerSprite(offset + 24, 16, task->unk_66 - 1);
  ResultStats_DrawUnitGrid(offset, task);
  Screen_SetCurrent(saved);
  Screen_SetCurrentGfx(savedGfx);
}

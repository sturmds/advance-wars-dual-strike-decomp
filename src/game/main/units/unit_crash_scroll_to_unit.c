#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/task.h"

void func_020f5888(int x, int y);

/* Script step of data_02168f48 (the crash task DailyFuel_BurnStep starts):
 * scrolls the map view to the crashing unit, except in the Design Room. */
void UnitCrash_ScrollToUnit(Task* task) {
  MapUnit* unit = task->unk_4c;

  if (data_027c027c->unk_48[data_027c0284]->gameMode == GAME_MODE_DESIGN_ROOM) {
    return;
  }
  func_020f5888(unit->x, unit->y);
}

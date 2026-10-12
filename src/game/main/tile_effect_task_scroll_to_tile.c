#include "types.h"
#include "game/Unk_02080440.h"
#include "game/tile_effect_task.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
#define STATE (data_027c027c->unk_48[data_027c0284])

void func_020f5888(int x, int y);

/* Script step of a tile effect task: scrolls the map view to its tile, except
 * in the Design Room. */
void TileEffectTask_ScrollToTile(TileEffectTask* task) {
  if (STATE->gameMode == GAME_MODE_DESIGN_ROOM) {
    return;
  }
  func_020f5888((s16)task->x, (s16)task->y);
}

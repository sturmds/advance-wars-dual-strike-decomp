#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

MapHeader* MapHeader_Get(int mapId);

/* TRUE in game mode 2, otherwise TRUE unless the current map header has bit
 * 0 of MapHeader.unk_42 set (campaign maps 0xe0-0x100 and four others do).
 * AiTurn_Start copies the result into bit 2 of gAiTurnState->flags. */
BOOL func_0209dec0(void) {
  Unk_02080440_48* state = data_027c027c->unk_48[data_027c0284];

  if (state->gameMode == 2) {
    return TRUE;
  }
  return (MapHeader_Get(state->unk_1c4->mapId)->unk_42 & 1) == 0;
}

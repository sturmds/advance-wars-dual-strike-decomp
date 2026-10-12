#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

int HwMod32(int a, int b);
int Rng_Next(void);
u8 Unit_GetUsableMovement(MapUnit* unit);

/* Puts unit fromSlot back on its tile and makes toSlot the acting unit:
 * lifts it off its tile, records its start position and rolls a new
 * gAiTurnState->roll. */
void Ai_SwitchActingUnit(int fromSlot, int toSlot) {
  MapUnit* from = &GAME->units[fromSlot];
  MapUnit* to = &GAME->units[toSlot];

  from->flags &= ~MAP_UNIT_LOCKED;
  GAME->mapTiles->unitAt[from->x + GAME->mapTiles->rowStart[from->y]] =
      fromSlot;
  GAME->actingUnit = to;
  GAME->actingUnitIndex = toSlot;
  to->flags |= MAP_UNIT_LOCKED;
  GAME->mapTiles->unitAt[to->x + GAME->mapTiles->rowStart[to->y]] = 0;
  GAME->actingUnitStartX = to->x;
  GAME->actingUnitStartY = to->y;
  gAiTurnState->roll = HwMod32(Rng_Next(), 100);
  gAiTurnState->unk_05 = Unit_GetUsableMovement(GAME->actingUnit);
}

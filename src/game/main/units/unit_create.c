#include "types.h"
#include "game/Unk_02080440.h"

MapUnit* Unit_FindFreeSlot(
    int player); /* first free slot (type 0) of the player's units, or NULL */
void Unit_Init(MapUnit* unit, u8 type);
void Player_CountUnitBuilt(int player, int type);

/* Puts a new unit of the given type at (x, y) for the current player, or
 * returns NULL when all 50 slots of the player are taken. */
MapUnit* Unit_Create(int x, int y, int type) {
  MapUnit* unit = Unit_FindFreeSlot(MAP_STATE->currentPlayer);

  if (unit == NULL) {
    return NULL;
  }
  Unit_Init(unit, type);
  unit->x = x;
  unit->y = y;
  Player_CountUnitBuilt(MAP_STATE->currentPlayer, type);
  return unit;
}

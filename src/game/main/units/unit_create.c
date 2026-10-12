#include "types.h"
#include "game/Unk_02080440.h"

MapUnit* Unit_FindFreeSlot(
    int player); /* first free slot (type 0) of the player's units, or NULL */
void Unit_Init(MapUnit* unit, u8 type);
void Player_CountUnitBuilt(int player, int type);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Puts a new unit of the given type at (x, y) for the current player, or
 * returns NULL when all 50 slots of the player are taken. */
MapUnit* Unit_Create(int x, int y, int type) {
  MapUnit* unit = Unit_FindFreeSlot(STATE->currentPlayer);

  if (unit == NULL) {
    return NULL;
  }
  Unit_Init(unit, type);
  unit->x = x;
  unit->y = y;
  Player_CountUnitBuilt(STATE->currentPlayer, type);
  return unit;
}

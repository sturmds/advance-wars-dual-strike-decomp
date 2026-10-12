#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int Unit_GetValuePerHp(int playerIndex, int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Each player owns MAP_UNITS_PER_PLAYER (64) unit slots, so slot >> 6 is the
 * owner's index minus one. Written as a shift to match. */
#define SLOT_OWNER_SHIFT 6

/* Score of one unit hit by a Black Bomb explosion: the value of the hp it
 * would lose (at most 50 hp, never the last one), negative when the unit
 * is on the team of the acting Black Bomb. */
int AiBlackBomb_ScoreUnit(MapUnit* unit) {
  int value;
  u16 loss;
  int unitOwner;
  int bombOwner;

  value = Unit_GetValuePerHp(STATE->currentPlayer, unit->type);
  loss = unit->hp;
  if (loss > 50) {
    loss = 50;
  } else {
    loss--;
  }
  unitOwner = (unit - STATE->units) >> SLOT_OWNER_SHIFT;
  bombOwner = STATE->actingUnitIndex >> SLOT_OWNER_SHIFT;
  value *= loss;
  if (STATE->players[bombOwner + 1].team ==
      (&STATE->players[unitOwner])[1].team) {
    return -value;
  }
  return value;
}

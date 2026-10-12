#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void Unit_CountDestroyed(
    MapUnit*
        unit); /* counts the unit and the units it carries per type for human players */
void func_020edb84(MapUnit* unit);

#define GAME (data_027c027c->unk_48[data_027c0284])

void func_020dd89c(void) {
  MapUnit* unit =
      &GAME->units[GAME->mapTiles->unitAt
                       [GAME->actingUnitDestX +
                        GAME->mapTiles->rowStart[GAME->actingUnitDestY]]];

  if (GAME->players[GAME->currentPlayer].controller ==
      PLAYER_CONTROLLER_HUMAN) {
    Unit_CountDestroyed(unit);
  }
  func_020edb84(unit);
}

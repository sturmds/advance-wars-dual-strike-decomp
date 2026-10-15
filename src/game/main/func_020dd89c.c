#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

void Unit_CountDestroyed(
    MapUnit*
        unit); /* counts the unit and the units it carries per type for human players */
void func_020edb84(MapUnit* unit);

void func_020dd89c(void) {
  MapUnit* unit =
      &MAP_STATE
           ->units[MAP_STATE->mapTiles
                       ->unitAt[MAP_STATE->actingUnitDestX +
                                MAP_STATE->mapTiles
                                    ->rowStart[MAP_STATE->actingUnitDestY]]];

  if (MAP_STATE->players[MAP_STATE->currentPlayer].controller ==
      PLAYER_CONTROLLER_HUMAN) {
    Unit_CountDestroyed(unit);
  }
  func_020edb84(unit);
}

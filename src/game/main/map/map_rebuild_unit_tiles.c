#include "types.h"
#include "game/map_unit.h"
#include "game/player.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"
#include "game/vision.h"

BOOL UnitSlot_IsVisible(
    u16 unitSlot); /* guess: the unit is not hidden from the current player */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])
#define HOOKS (data_027c027c->unk_50[data_027c0284])

/* Rebuilds the per-tile unit slots after units changed: derives the
 * unk_1c4 size values (the 1e and 20 fields are signed here), runs the
 * screen's callback, then clears MapTiles.unitAt and unk_508 and fills them
 * from the unit list. unk_508 gets every unit; a unit marked SELECTED but
 * not LOCKED is skipped, and a tile holds one unit unless the unit already
 * on it is LOCKED. unitAt only gets units that are not SELECTED, and for a
 * human player only those on a visible tile and not hidden. */
void Map_RebuildUnitTiles(void) {
  int i;
  int x;
  int y;

  MAP_STATE->unk_1c4->unk_26 = (s16)MAP_STATE->unk_1c4->scrollX / 16;
  MAP_STATE->unk_1c4->unk_28 = (s16)MAP_STATE->unk_1c4->scrollY / 16;
  MAP_STATE->unk_1c4->unk_22 = *(s16*)&MAP_STATE->unk_1c4->scrollX;
  MAP_STATE->unk_1c4->unk_24 = *(s16*)&MAP_STATE->unk_1c4->scrollY;
  if (HOOKS->callback != NULL) {
    HOOKS->callback();
  }
  Map_RebuildVision();

  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)] = 0;
      MAP_STATE->mapTiles->unk_508[MAP_TILE_INDEX(x, y)] = 0;
    }
  }

  for (i = 1; i < MAP_UNIT_COUNT; i++) {
    if (MAP_STATE->units[i].type != UNIT_NONE &&
        (!(MAP_STATE->units[i].flags & MAP_UNIT_SELECTED) ||
         (MAP_STATE->units[i].flags & MAP_UNIT_LOCKED))) {
      if (MAP_STATE->mapTiles->unk_508[MAP_TILE_INDEX(
              MAP_STATE->units[i].x, MAP_STATE->units[i].y)] == 0 ||
          (MAP_STATE
               ->units[MAP_STATE->mapTiles->unk_508[MAP_TILE_INDEX(
                   MAP_STATE->units[i].x, MAP_STATE->units[i].y)]]
               .flags &
           MAP_UNIT_LOCKED)) {
        MAP_STATE->mapTiles->unk_508[MAP_TILE_INDEX(MAP_STATE->units[i].x,
                                                    MAP_STATE->units[i].y)] = i;
        if (!(MAP_STATE->units[i].flags & MAP_UNIT_SELECTED)) {
          if (MAP_STATE->players[MAP_STATE->currentPlayer].controller ==
                  PLAYER_CONTROLLER_AI ||
              MAP_STATE->mapTiles->inSight[MAP_TILE_INDEX(
                  MAP_STATE->units[i].x, MAP_STATE->units[i].y)] != 0) {
            MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(
                MAP_STATE->units[i].x, MAP_STATE->units[i].y)] = i;
          }
        }
      }
    }
  }

  for (i = 1; i < MAP_UNIT_COUNT; i++) {
    if (MAP_STATE->units[i].type != UNIT_NONE &&
        !(MAP_STATE->units[i].flags & MAP_UNIT_SELECTED) &&
        MAP_STATE->players[MAP_STATE->currentPlayer].controller !=
            PLAYER_CONTROLLER_AI) {
      if (!UnitSlot_IsVisible(i) ||
          MAP_STATE->mapTiles->inSight[MAP_TILE_INDEX(
              MAP_STATE->units[i].x, MAP_STATE->units[i].y)] == 0) {
        MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(MAP_STATE->units[i].x,
                                                   MAP_STATE->units[i].y)] = 0;
      }
    }
  }
}

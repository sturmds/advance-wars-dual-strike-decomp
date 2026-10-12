#include "types.h"
#include "game/map_unit.h"
#include "game/player.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void Map_RebuildVision(void);
BOOL UnitSlot_IsVisible(
    u16 unitSlot); /* guess: the unit is not hidden from the current player */

#define GAME (data_027c027c->unk_48[data_027c0284])
#define HOOKS (data_027c027c->unk_50[data_027c0284])

/* Index into a per-tile array of MapTiles. */
#define TILE_INDEX(x, y) ((x) + GAME->mapTiles->rowStart[y])

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

  GAME->unk_1c4->unk_26 = (s16)GAME->unk_1c4->scrollX / 16;
  GAME->unk_1c4->unk_28 = (s16)GAME->unk_1c4->scrollY / 16;
  GAME->unk_1c4->unk_22 = *(s16*)&GAME->unk_1c4->scrollX;
  GAME->unk_1c4->unk_24 = *(s16*)&GAME->unk_1c4->scrollY;
  if (HOOKS->callback != NULL) {
    HOOKS->callback();
  }
  Map_RebuildVision();

  for (y = 0; y < GAME->unk_1c4->mapHeight; y++) {
    for (x = 0; x < GAME->unk_1c4->mapWidth; x++) {
      GAME->mapTiles->unitAt[TILE_INDEX(x, y)] = 0;
      GAME->mapTiles->unk_508[TILE_INDEX(x, y)] = 0;
    }
  }

  for (i = 1; i < 256; i++) {
    if (GAME->units[i].type != UNIT_NONE &&
        (!(GAME->units[i].flags & MAP_UNIT_SELECTED) ||
         (GAME->units[i].flags & MAP_UNIT_LOCKED))) {
      if (GAME->mapTiles
                  ->unk_508[TILE_INDEX(GAME->units[i].x, GAME->units[i].y)] ==
              0 ||
          (GAME->units[GAME->mapTiles->unk_508[TILE_INDEX(GAME->units[i].x,
                                                          GAME->units[i].y)]]
               .flags &
           MAP_UNIT_LOCKED)) {
        GAME->mapTiles
            ->unk_508[TILE_INDEX(GAME->units[i].x, GAME->units[i].y)] = i;
        if (!(GAME->units[i].flags & MAP_UNIT_SELECTED)) {
          if (GAME->players[GAME->currentPlayer].controller ==
                  PLAYER_CONTROLLER_AI ||
              GAME->mapTiles->inSight[TILE_INDEX(GAME->units[i].x,
                                                 GAME->units[i].y)] != 0) {
            GAME->mapTiles
                ->unitAt[TILE_INDEX(GAME->units[i].x, GAME->units[i].y)] = i;
          }
        }
      }
    }
  }

  for (i = 1; i < 256; i++) {
    if (GAME->units[i].type != UNIT_NONE &&
        !(GAME->units[i].flags & MAP_UNIT_SELECTED) &&
        GAME->players[GAME->currentPlayer].controller != PLAYER_CONTROLLER_AI) {
      if (!UnitSlot_IsVisible(i) ||
          GAME->mapTiles
                  ->inSight[TILE_INDEX(GAME->units[i].x, GAME->units[i].y)] ==
              0) {
        GAME->mapTiles->unitAt[TILE_INDEX(GAME->units[i].x, GAME->units[i].y)] =
            0;
      }
    }
  }
}

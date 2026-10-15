#include "types.h"
#include "game/map_unit.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/vision.h"

extern u16 Ov000_gPlayerTileOwnerBits
    []; /* per player slot: slot << 5, the owner bits of MapTiles.tiles */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

/* Adds what one player sees to the vision grids its Player.visionGrids bits pick:
 * the area around each of its units (only the first 51 of its 64 slots are
 * looked at) and every tile it owns. */
void Player_AddVision(int player) {
  int i;
  int x;
  int y;

  if (MAP_STATE->players[player].controller == PLAYER_CONTROLLER_NONE) {
    return;
  }
  if (MAP_STATE->players[player].visionGrids == 0) {
    return;
  }
  for (i = 0; i < MAP_UNITS_MAX + 1; i++) {
    Unit_AddVision(&MAP_STATE->units[Ov000_gPlayerFirstUnitSlot[player] + i],
                   1);
  }
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (Ov000_gPlayerTileOwnerBits[player] ==
          (MAP_STATE->mapTiles->tiles[x + MAP_STATE->mapTiles->rowStart[y]] &
           0xe0)) {
        Vision_AddArea(x, y, 0, MAP_STATE->players[player].visionGrids, 1, 0);
      }
    }
  }
}

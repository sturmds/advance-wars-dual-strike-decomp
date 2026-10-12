#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Unit_IsCurrentPlayers(s16 unitSlot);
int UnitType_GetAttackKind(int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

#define NO_TILE 9999
#define UNLIMITED_MOVE 110

/* Picks the tile where the acting sea transport can take aboard the unit
 * at (x, y): one of the four next to it, a shoal or port, with a
 * rangeMap value from 0 to UNLIMITED_MOVE (it takes the lowest, the later tile
 * on ties). A tile with a unit on it counts only when the unit is the
 * current player's, does not move on the ground and is not indirect.
 * Writes the tile to dest[0], dest[1]; FALSE when there is none. */
BOOL AiSeaTransport_FindPickupTile(int x, int y, s16* dest) {
  /* declaration order needed to match (register allocation) */
  int maxY;
  int maxX;
  int ty;
  int minX;
  int dist;
  int slot;
  int tx;
  u8 type;
  MapTiles* map;
  int minY;

  dest[0] = NO_TILE;
  minX = x - 1;
  if (minX < 0) {
    minX = 0;
  }
  minY = y - 1;
  if (minY < 0) {
    minY = 0;
  }
  maxX = STATE->unk_1c4->mapWidth - 1;
  if (x + 1 < maxX) {
    maxX = x + 1;
  }
  maxY = STATE->unk_1c4->mapHeight - 1;
  if (y + 1 < maxY) {
    maxY = y + 1;
  }
  for (ty = minY; ty <= maxY; ty++) {
    for (tx = minX; tx <= maxX; tx++) {
      dist = 0;
      if (x - tx > 0) {
        dist += x - tx;
      } else {
        dist += tx - x;
      }
      if (y - ty > 0) {
        dist += y - ty;
      } else {
        dist += ty - y;
      }
      if (dist < 1 || dist > 1) {
        continue;
      }
      if (STATE->rangeMap[ty][tx] < 0 ||
          STATE->rangeMap[ty][tx] > UNLIMITED_MOVE) {
        continue;
      }
      map = STATE->mapTiles;
      if (MAP_TILE_TERRAIN(map->tiles[TILE(tx, ty)]) != TERRAIN_SHOAL &&
          MAP_TILE_TERRAIN(map->tiles[TILE(tx, ty)]) != TERRAIN_PORT) {
        continue;
      }
      slot = map->unitAt[TILE(tx, ty)];
      if (slot != 0) {
        if (!Unit_IsCurrentPlayers(slot)) {
          continue;
        }
        type = STATE->units[slot].type;
        if (Ov000_gUnitStats[type].moveGroup &
            (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)) {
          continue;
        }
        if (UnitType_GetAttackKind(type) == ATTACK_KIND_INDIRECT) {
          continue;
        }
      }
      if (dest[0] == NO_TILE ||
          STATE->rangeMap[ty][tx] <= STATE->rangeMap[dest[1]][dest[0]]) {
        dest[0] = tx;
        dest[1] = ty;
      }
    }
  }
  return dest[0] != NO_TILE;
}

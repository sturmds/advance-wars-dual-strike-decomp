#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"
#include "game/co_data.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Transport_HasRoomFor(
    MapUnit* transport,
    int unitType); /* TRUE when transport has room for a unit of unitType */

#define STATE (data_027c027c->unk_48[data_027c0284])
/* Needed to match: the tile reads index the state array first. */
#define TILE_STATE STATE_IDX_FIRST
#define TILE_INDEX(x, y) ((x) + TILE_STATE->mapTiles->rowStart[y])
/* Default movement cost chart of the first CO record; -1 marks a terrain the move type cannot enter. */
#define CHART \
  ((const s8(*)[32])gCoTable[0].blocks[0].movementCost[MOVE_CHART_DEFAULT])

/* TRUE when tile (x, y) holds a unit of the acting unit's player that has
 * room for the acting unit, and both units' move types can enter the tile's
 * terrain. */
BOOL Tile_HasTransportToBoard(int x, int y) {
  u8 slot;
  MapUnit* transport;

  slot = TILE_STATE->mapTiles->unitAt[TILE_INDEX(x, y)];
  transport = &STATE->units[slot];
  if (slot == 0) {
    return FALSE;
  }
  /* slot & 0xc0 is the player's block of 64 unit slots */
  if ((slot & 0xc0) != (STATE->actingUnitIndex & 0xc0)) {
    return FALSE;
  }
  if (!Transport_HasRoomFor(transport, STATE->actingUnit->type)) {
    return FALSE;
  }
  /* needed to match: x is reused for the terrain id */
  x = MAP_TILE_TERRAIN(TILE_STATE->mapTiles->tiles[TILE_INDEX(x, y)]);
  if (CHART[Ov000_gUnitStats[transport->type].moveType][x] == -1) {
    return FALSE;
  }
  return CHART[Ov000_gUnitStats[STATE->actingUnit->type].moveType][x] != -1;
}

#include "types.h"
#include "game/task.h"
#include "game/terrain.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u16 Ov000_gPlayerTileOwnerBits
    [5]; /* per player slot: slot << 5, the owner bits of MapTiles.tiles */
extern s8 Ov000_gUrbanTerrain
    [32]; /* nonzero for urban terrain, indexed by terrain id */

s16 MapCell_SetOwner(int x, int y, int tile);
s16 func_020f6488(int x, int y, int tile);
void func_020c4018(void);
void func_020c4448(void);
void Map_CountPlayerProperties(void);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define HQ (STATE->players[task->unk_64])
#define SITE (STATE->unk_608c[i])
#define TILE(x, y) (STATE->mapTiles->tiles[(x) + STATE->mapTiles->rowStart[y]])

/* Step of the task LinkPlayerDefeatTask_Create starts: hands the defeated
 * player's (unk_64) urban properties to a new owner. For reason code 2
 * (unk_66) the new owner is whoever holds the player's HQ tile, which first
 * goes back to the defeated player; for other codes, neutral. The new owner's
 * team stops capturing those tiles, and HQs become cities. Sets unk_68 to 1
 * when done. */
void LinkPlayerDefeatTask_TransferProperties(Task* task) {
  u16 newOwner;
  u16 i;
  int cellTile;
  u8 old;
  u8 unitSlot;
  int terrain;
  u16 ownerBits;

  if (task->unk_66 == 2) {
    old = TILE(HQ.hqX & 0x7f, HQ.hqY & 0x7f);
    TILE(HQ.hqX & 0x7f, HQ.hqY & 0x7f) =
        Ov000_gPlayerTileOwnerBits[task->unk_64] | TERRAIN_HQ;
    newOwner = MAP_TILE_OWNER(old);
    MapCell_SetOwner((s16)(HQ.hqX & 0x7f), (s16)(HQ.hqY & 0x7f),
                     Ov000_gPlayerTileOwnerBits[task->unk_64] & 0xff);
    func_020c4448();
  } else {
    newOwner = 0;
  }
  /* needed to match: a local, kept on the stack */
  cellTile = 0;
  for (i = 0; i < 92; i++) {
    if (SITE.tile == 0xff) {
      break;
    }
    if (Ov000_gUrbanTerrain[MAP_TILE_TERRAIN(SITE.tile)] == 0) {
      continue;
    }
    if ((SITE.tile & 0xe0) != Ov000_gPlayerTileOwnerBits[task->unk_64]) {
      continue;
    }
    if (newOwner != 0) {
      unitSlot =
          STATE->mapTiles->unitAt[SITE.x + STATE->mapTiles->rowStart[SITE.y]];
      if (unitSlot != 0 && STATE->players[(unitSlot >> 6) + 1].team ==
                               STATE->players[newOwner].team) {
        STATE->units[unitSlot].captureProgress = 0;
      }
    }
    terrain = MAP_TILE_TERRAIN(SITE.tile);
    if (terrain == TERRAIN_HQ) {
      TILE(SITE.x, SITE.y) =
          Ov000_gPlayerTileOwnerBits[newOwner] | TERRAIN_CITY;
      func_020f6488(SITE.x, SITE.y, cellTile);
      MapCell_SetOwner(SITE.x, SITE.y,
                       Ov000_gPlayerTileOwnerBits[newOwner] & 0xff);
    } else {
      ownerBits = Ov000_gPlayerTileOwnerBits[newOwner];
      TILE(SITE.x, SITE.y) = ownerBits | terrain;
      MapCell_SetOwner(SITE.x, SITE.y, ownerBits & 0xff);
    }
  }
  func_020c4018();
  func_020c4448();
  Map_CountPlayerProperties();
  task->unk_68 = 1;
}

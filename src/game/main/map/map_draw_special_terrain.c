#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"
#include "game/map_object.h"
#include "game/terrain.h"

extern u16 data_ov000_022dbd50[];
extern u16 data_ov000_022dbd52[];

/* The map header's u16 at +0x28 and +0x2a for a map id; the header fields are
 * u8 unk_28[8] for now. */
#define HEADER_U16(symbol, mapId) \
  (*(u16*)((u8*)(symbol) + (mapId) * sizeof(MapHeader)))

MapObject* Map_FindObjectCovering(int x, int y);
void MapObjects_ClearHead(void);
void Map_AddDeathrayObject(int x, int y, int w, int h, int a, int b);
void Map_AddBlackObeliskObject(int x, int y);
void Map_AddMinicannonObject(int x, int y, int a, int b, int owner);
void Map_AddBlackCannonObject(int x, int y, int w, int h, int a, int b,
                              int owner);
void Map_AddVolcanoObject(int x, int y, int w, int h, int a, int b, int c);
void Map_AddBlockedObject(int x, int y, int w, int h);
void Map_AddGrandBoltWeakPointObject(int x, int y, int owner);
void Map_AddBlackCrystalObject(int x, int y);

void Map_DrawSpecialTerrain(void) {
  int mapA = HEADER_U16(data_ov000_022dbd50, MAP_STATE->unk_1c4->mapId);
  int mapB = HEADER_U16(data_ov000_022dbd52, MAP_STATE->unk_1c4->mapId);
  int x;
  int y;

  MapObjects_ClearHead();
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      u8 tile;

      if (Map_FindObjectCovering(x, y) != NULL) {
        continue;
      }
      tile = MAP_STATE->mapTiles->tiles[x + MAP_STATE->mapTiles->rowStart[y]];
      switch (MAP_TILE_TERRAIN(tile)) {
        case TERRAIN_BLACK_CRYSTAL:
          Map_AddBlackCrystalObject(x, y);
          break;
        case TERRAIN_MINICANNON:
          Map_AddMinicannonObject(x, y, mapA, mapB, MAP_TILE_OWNER(tile));
          break;
        case TERRAIN_VOLCANO:
          switch (MAP_TILE_OWNER(tile)) {
            case 0:
              Map_AddVolcanoObject(x, y, VOLCANO_SIZE, VOLCANO_SIZE, 0, 2, 0);
              break;
            case 1:
              Map_AddVolcanoObject(x, y, VOLCANO_SIZE, VOLCANO_SIZE, 2, 1, 1);
              break;
            case 2:
              Map_AddVolcanoObject(x, y, VOLCANO_SIZE, VOLCANO_SIZE, 2, 1, 2);
              break;
          }
          break;
        case TERRAIN_BLACK_CANNON:
          Map_AddBlackCannonObject(x, y, BLACK_CANNON_SIZE, BLACK_CANNON_SIZE,
                                   mapA, mapB, MAP_TILE_OWNER(tile));
          break;
        case TERRAIN_BLOCKED:
          Map_AddBlockedObject(x, y, BLOCKED_SIZE, BLOCKED_SIZE);
          break;
        case TERRAIN_GRAND_BOLT_WEAK_POINT:
          Map_AddGrandBoltWeakPointObject(x, y, MAP_TILE_OWNER(tile));
          break;
        case TERRAIN_DEATHRAY:
          Map_AddDeathrayObject(x, y, DEATHRAY_SIZE, DEATHRAY_SIZE, 7, 7);
          break;
        case TERRAIN_BLACK_OBELISK:
          Map_AddBlackObeliskObject(x, y);
          break;
      }
    }
  }
}

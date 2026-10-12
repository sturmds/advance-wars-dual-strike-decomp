#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int Ov006_IsPlainRiver(int x, int y);  /* guess: tile x, y is a river tile */
int func_ov006_023624b4(int x, int y); /* river variant for tile x, y */
int Ov006_IsSpecialCell(int x,
                        int y); /* tile x, y holds 0xfc, 0xfd, 0x11c or 0x11d */
int Ov006_GetLandPieceIfNotSea(int x, int y);
void Ov006_SetTerrain(int x, int y,
                      int terrain); /* stores the terrain id of tile x, y */
void Ov006_StoreMapCell(int x, int y,
                        int tile); /* guess: stores the tile graphic of x, y */

/* After a map edit at tile x, y, fixes its four neighbours (above, below,
 * left, right): a river tile gets its river variant picked again; one of
 * the four special tiles of Ov006_IsSpecialCell (likely river mouths) that
 * Ov006_GetLandPieceIfNotSea rejects turns back into sea, tile 42. */
void Ov006_RefreshNeighborRivers(int x, int y) {
  int tile;

  if (y > 0) {
    if (Ov006_IsPlainRiver(x, y - 1) != 0) {
      tile = func_ov006_023624b4(x, y - 1);
      if (tile > 0) {
        Ov006_SetTerrain(x, y - 1, TERRAIN_RIVER);
        Ov006_StoreMapCell(x, y - 1, tile);
      }
    } else if (Ov006_IsSpecialCell(x, y - 1) != 0 &&
               Ov006_GetLandPieceIfNotSea(x, y - 1) == 0) {
      Ov006_SetTerrain(x, y - 1, TERRAIN_SEA);
      Ov006_StoreMapCell(x, y - 1, 42);
    }
  }
  if (y < data_027c027c->unk_48[data_027c0284]->unk_1c4->mapHeight - 1) {
    if (Ov006_IsPlainRiver(x, y + 1) != 0) {
      tile = func_ov006_023624b4(x, y + 1);
      if (tile > 0) {
        Ov006_SetTerrain(x, y + 1, TERRAIN_RIVER);
        Ov006_StoreMapCell(x, y + 1, tile);
      }
    } else if (Ov006_IsSpecialCell(x, y + 1) != 0 &&
               Ov006_GetLandPieceIfNotSea(x, y + 1) == 0) {
      Ov006_SetTerrain(x, y + 1, TERRAIN_SEA);
      Ov006_StoreMapCell(x, y + 1, 42);
    }
  }
  if (x > 0) {
    if (Ov006_IsPlainRiver(x - 1, y) != 0) {
      tile = func_ov006_023624b4(x - 1, y);
      if (tile > 0) {
        Ov006_SetTerrain(x - 1, y, TERRAIN_RIVER);
        Ov006_StoreMapCell(x - 1, y, tile);
      }
    } else if (Ov006_IsSpecialCell(x - 1, y) != 0 &&
               Ov006_GetLandPieceIfNotSea(x - 1, y) == 0) {
      Ov006_SetTerrain(x - 1, y, TERRAIN_SEA);
      Ov006_StoreMapCell(x - 1, y, 42);
    }
  }
  if (x < data_027c027c->unk_48[data_027c0284]->unk_1c4->mapWidth - 1) {
    if (Ov006_IsPlainRiver(x + 1, y) != 0) {
      tile = func_ov006_023624b4(x + 1, y);
      if (tile > 0) {
        Ov006_SetTerrain(x + 1, y, TERRAIN_RIVER);
        Ov006_StoreMapCell(x + 1, y, tile);
      }
    } else if (Ov006_IsSpecialCell(x + 1, y) != 0 &&
               Ov006_GetLandPieceIfNotSea(x + 1, y) == 0) {
      Ov006_SetTerrain(x + 1, y, TERRAIN_SEA);
      Ov006_StoreMapCell(x + 1, y, 42);
    }
  }
}

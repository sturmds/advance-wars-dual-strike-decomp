#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"
#include "game/design_map_edit.h"

int Ov006_IsSpecialCell(int x, int y);
int Ov006_GetLandPieceIfNotSea(int x, int y);
int Ov006_CountRiverNeighbors(int x, int y);
int Ov006_MouthHasNoOutlet(int x, int y);
void Ov006_SetTerrain(int x, int y, int terrain);
void Ov006_StoreMapCell(int x, int y, int cell);
void Ov006_RefreshNeighborRivers(int x, int y);

#define FIX_NEIGHBOR(nx, ny, scoreTooLow)                 \
  if (Ov006_IsSpecialCell(nx, ny)) {                      \
    if (Ov006_GetLandPieceIfNotSea(nx, ny) scoreTooLow || \
        !Ov006_CountRiverNeighbors(nx, ny) ||             \
        !Ov006_MouthHasNoOutlet(nx, ny)) {                \
      Ov006_SetTerrain(nx, ny, TERRAIN_SEA);              \
      Ov006_StoreMapCell(nx, ny, MAP_CELL_SEA);           \
      Ov006_RefreshNeighborRivers(nx, ny);                \
    }                                                     \
  }

/* Checks the eight tiles around x, y. A neighbour with one of the cells
 * Ov006_IsSpecialCell accepts (codes 252, 253, 284, 285) that no longer
 * passes Ov006_GetLandPieceIfNotSea, Ov006_CountRiverNeighbors and Ov006_MouthHasNoOutlet
 * turns back into plain sea; Ov006_RefreshNeighborRivers then updates the tiles
 * around it. Corner tiles fail at a score of 0, side tiles only below 0. */
void Ov006_ResetNeighborsToSea(int x, int y) {
  if (y > 0) {
    FIX_NEIGHBOR(x, y - 1, < 0);
    if (x > 0) {
      FIX_NEIGHBOR(x - 1, y - 1, <= 0);
    }
    if (x < MAP_STATE->unk_1c4->mapWidth - 1) {
      FIX_NEIGHBOR(x + 1, y - 1, <= 0);
    }
  }
  if (y < MAP_STATE->unk_1c4->mapHeight - 1) {
    FIX_NEIGHBOR(x, y + 1, < 0);
    if (x > 0) {
      FIX_NEIGHBOR(x - 1, y + 1, <= 0);
    }
    if (x < MAP_STATE->unk_1c4->mapWidth - 1) {
      FIX_NEIGHBOR(x + 1, y + 1, <= 0);
    }
  }
  if (x > 0) {
    FIX_NEIGHBOR(x - 1, y, < 0);
  }
  if (x < MAP_STATE->unk_1c4->mapWidth - 1) {
    FIX_NEIGHBOR(x + 1, y, < 0);
  }
}

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

int Ov006_IsBareShoal(int x, int y); /* tile x, y is shoal */
int Ov006_GetShoalGraphic(
    int x, int y); /* shoal cell code for tile x, y, -1 if none fits */
void Ov006_SetTerrain(int x, int y,
                      int terrain); /* stores the terrain id of tile x, y */
void Ov006_StoreMapCell(
    int x, int y, int cell); /* guess: stores the cell code of tile x, y */

/* After a map edit at tile x, y, picks the cell code again for each
 * neighbouring shoal (above, left, right, below). A shoal with no fitting
 * cell code turns into sea, cell 42. */
void Ov006_RefreshNeighborShoals(int x, int y) {
  int cell;

  if (y > 0 && Ov006_IsBareShoal(x, y - 1) != 0) {
    cell = Ov006_GetShoalGraphic(x, y - 1);
    if (cell < 0) {
      Ov006_SetTerrain(x, y - 1, TERRAIN_SEA);
      Ov006_StoreMapCell(x, y - 1, 42);
    } else {
      Ov006_StoreMapCell(x, y - 1, cell);
    }
  }
  if (x > 0 && Ov006_IsBareShoal(x - 1, y) != 0) {
    cell = Ov006_GetShoalGraphic(x - 1, y);
    if (cell < 0) {
      Ov006_SetTerrain(x - 1, y, TERRAIN_SEA);
      Ov006_StoreMapCell(x - 1, y, 42);
    } else {
      Ov006_StoreMapCell(x - 1, y, cell);
    }
  }
  if (x < data_027c027c->unk_48[data_027c0284]->unk_1c4->mapWidth - 1 &&
      Ov006_IsBareShoal(x + 1, y) != 0) {
    cell = Ov006_GetShoalGraphic(x + 1, y);
    if (cell < 0) {
      Ov006_SetTerrain(x + 1, y, TERRAIN_SEA);
      Ov006_StoreMapCell(x + 1, y, 42);
    } else {
      Ov006_StoreMapCell(x + 1, y, cell);
    }
  }
  if (y < data_027c027c->unk_48[data_027c0284]->unk_1c4->mapHeight - 1 &&
      Ov006_IsBareShoal(x, y + 1) != 0) {
    cell = Ov006_GetShoalGraphic(x, y + 1);
    if (cell < 0) {
      Ov006_SetTerrain(x, y + 1, TERRAIN_SEA);
      Ov006_StoreMapCell(x, y + 1, 42);
    } else {
      Ov006_StoreMapCell(x, y + 1, cell);
    }
  }
}

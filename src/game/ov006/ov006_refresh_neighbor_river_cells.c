#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int Ov006_IsPlainRiver(int x, int y);
int Ov006_GetRiverPiece(
    int x, int y); /* variant for tile x, y from its 3x3 block, -1 if none */
void func_ov006_0235da1c(int x, int y, int arg);
void Ov006_StoreMapCell(int x, int y, int cell);
void Ov006_AddRiverMouths(int x, int y);

/* After a map edit at tile x, y: for each of the four neighbours (above,
 * below, left, right) that Ov006_IsPlainRiver accepts, picks its variant
 * again. A negative variant goes to func_ov006_0235da1c; a positive one is
 * stored and followed by Ov006_AddRiverMouths. 0 leaves the tile as it is. */
void Ov006_RefreshNeighborRiverCells(int x, int y) {
  int tile;

  if (y > 0 && Ov006_IsPlainRiver(x, y - 1) != 0) {
    tile = Ov006_GetRiverPiece(x, y - 1);
    if (tile < 0) {
      func_ov006_0235da1c(x, y - 1, 1);
    } else if (tile > 0) {
      Ov006_StoreMapCell(x, y - 1, tile);
      Ov006_AddRiverMouths(x, y - 1);
    }
  }
  if (y < data_027c027c->unk_48[data_027c0284]->unk_1c4->mapHeight - 1 &&
      Ov006_IsPlainRiver(x, y + 1) != 0) {
    tile = Ov006_GetRiverPiece(x, y + 1);
    if (tile < 0) {
      func_ov006_0235da1c(x, y + 1, 1);
    } else if (tile > 0) {
      Ov006_StoreMapCell(x, y + 1, tile);
      Ov006_AddRiverMouths(x, y + 1);
    }
  }
  if (x > 0 && Ov006_IsPlainRiver(x - 1, y) != 0) {
    tile = Ov006_GetRiverPiece(x - 1, y);
    if (tile < 0) {
      func_ov006_0235da1c(x - 1, y, 1);
    } else if (tile > 0) {
      Ov006_StoreMapCell(x - 1, y, tile);
      Ov006_AddRiverMouths(x - 1, y);
    }
  }
  if (x < data_027c027c->unk_48[data_027c0284]->unk_1c4->mapWidth - 1 &&
      Ov006_IsPlainRiver(x + 1, y) != 0) {
    tile = Ov006_GetRiverPiece(x + 1, y);
    if (tile < 0) {
      func_ov006_0235da1c(x + 1, y, 1);
    } else if (tile > 0) {
      Ov006_StoreMapCell(x + 1, y, tile);
      Ov006_AddRiverMouths(x + 1, y);
    }
  }
}

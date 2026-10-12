#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int Ov006_GetCellMatchedToLeft(int x, int y);
void Ov006_StoreMapCell(int x, int y, int value);

/* Rewrites the cell at x, y and the one to its right with the tile that
 * Ov006_GetCellMatchedToLeft returns for them. */
void func_ov006_0235dd24(int x, int y) {
  Ov006_StoreMapCell(x, y, Ov006_GetCellMatchedToLeft(x, y));
  if (x < data_027c027c->unk_48[data_027c0284]->unk_1c4->mapWidth - 2) {
    Ov006_StoreMapCell(x + 1, y, Ov006_GetCellMatchedToLeft(x + 1, y));
  }
}

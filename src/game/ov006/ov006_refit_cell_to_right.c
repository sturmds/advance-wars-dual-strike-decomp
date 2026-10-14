#include "types.h"
#include "game/Unk_02080440.h"

void Ov006_StoreMapCell(int x, int y,
                        int cell); /* guess: sets the cell code of tile x, y */
int Ov006_GetCellMatchedToLeft(int x, int y);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Writes the stored cell code of the tile right of (x, y) back through the
 * setter (as Ov006_SetCellAndFit does for one tile). Does nothing at the
 * map's right edge. */
void Ov006_RefitCellToRight(int x, int y) {
  if (x >= STATE->unk_1c4->mapWidth - 1) {
    return;
  }
  Ov006_StoreMapCell(x + 1, y, Ov006_GetCellMatchedToLeft(x + 1, y));
}

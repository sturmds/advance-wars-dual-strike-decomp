#include "types.h"

void Ov006_StoreMapCell(int x, int y,
                        int cell); /* guess: sets the cell code of tile x, y */
int Ov006_GetCellMatchedToLeft(int x, int y);

/* Writes cell at tile x, y, then writes the cell code now stored there back
 * through the same setter (likely so the setter's side effects run on the
 * stored value). The name is a guess from this shape. */
void Ov006_SetCellAndFit(int x, int y, int cell) {
  Ov006_StoreMapCell(x, y, cell);
  Ov006_StoreMapCell(x, y, Ov006_GetCellMatchedToLeft(x, y));
}

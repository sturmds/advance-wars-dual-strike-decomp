#include "types.h"

u16* Bg_GetMapEntry(int layer, int x,
                    int y); /* entry (x, y) of the layer's tile map */

/* Writes one tile map entry at (x, y) of layer 0-3. */
void Bg_PutTile(int layer, int x, int y, u16 entry) {
  *Bg_GetMapEntry(layer, x, y) = entry;
}

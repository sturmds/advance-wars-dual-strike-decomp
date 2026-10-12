#include "types.h"
#include "game/Unk_02080440.h"

void Ov006_RefitTile(int x, int y, int refitRivers);

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Refits the eight tiles around x, y that are on the map, after the tile at
 * x, y changed. Only the four orthogonal neighbours refit their rivers. */
void Ov006_RefitAround(int x, int y) {
  if (y > 0) {
    if (x > 0) {
      Ov006_RefitTile(x - 1, y - 1, FALSE);
    }
    Ov006_RefitTile(x, y - 1, TRUE);
    if (x < STATE->unk_1c4->mapWidth - 1) {
      Ov006_RefitTile(x + 1, y - 1, FALSE);
    }
  }
  if (x > 0) {
    Ov006_RefitTile(x - 1, y, TRUE);
  }
  if (x < STATE->unk_1c4->mapWidth - 1) {
    Ov006_RefitTile(x + 1, y, TRUE);
  }
  if (y < STATE->unk_1c4->mapHeight - 1) {
    if (x > 0) {
      Ov006_RefitTile(x - 1, y + 1, FALSE);
    }
    Ov006_RefitTile(x, y + 1, TRUE);
    if (x < STATE->unk_1c4->mapWidth - 1) {
      Ov006_RefitTile(x + 1, y + 1, FALSE);
    }
  }
}

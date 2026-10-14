#include "types.h"
#include "game/Unk_02080440.h"

void Ov006_ReefToSeaIfNotOpenWater(int x, int y);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Runs the reef check on the eight tiles around x, y that are on the map,
 * after the tile at x, y changed: a reef that no longer fits turns to sea. */
void Ov006_CheckReefsAround(int x, int y) {
  if (y > 0) {
    if (x > 0) {
      Ov006_ReefToSeaIfNotOpenWater(x - 1, y - 1);
    }
    Ov006_ReefToSeaIfNotOpenWater(x, y - 1);
    if (x < STATE->unk_1c4->mapWidth - 1) {
      Ov006_ReefToSeaIfNotOpenWater(x + 1, y - 1);
    }
  }
  if (x > 0) {
    Ov006_ReefToSeaIfNotOpenWater(x - 1, y);
  }
  if (x < STATE->unk_1c4->mapWidth - 1) {
    Ov006_ReefToSeaIfNotOpenWater(x + 1, y);
  }
  if (y < STATE->unk_1c4->mapHeight - 1) {
    if (x > 0) {
      Ov006_ReefToSeaIfNotOpenWater(x - 1, y + 1);
    }
    Ov006_ReefToSeaIfNotOpenWater(x, y + 1);
    if (x < STATE->unk_1c4->mapWidth - 1) {
      Ov006_ReefToSeaIfNotOpenWater(x + 1, y + 1);
    }
  }
}

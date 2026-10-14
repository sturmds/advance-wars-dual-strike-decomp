#include "types.h"
#include "game/design_map_edit.h"
#include "game/Unk_02080440.h"

void Ov006_RefitJoinedCellsAround(int x, int y);

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;
extern s16 Ov006_gCrossDx[]; /* offsets: centre, left, right, up, down */
extern s16 Ov006_gCrossDy[];

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Runs Ov006_RefitJoinedCellsAround on tile x, y and on each of its four
 * neighbours that lies on the map. */
void Ov006_RefitJoinedCellsInCross(int x, int y) {
  int i;
  int nx;
  int ny;

  for (i = 0; i < DESIGN_CROSS_COUNT; i++) {
    nx = x + Ov006_gCrossDx[i];
    if (nx >= 0 && nx < STATE->unk_1c4->mapWidth) {
      ny = y + Ov006_gCrossDy[i];
      if (ny >= 0 && ny < STATE->unk_1c4->mapHeight) {
        Ov006_RefitJoinedCellsAround(nx, ny);
      }
    }
  }
}

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

void func_020f5878(int x, int y);

void func_ov001_02351538(void) {
  int x;
  int y;

  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->mapTiles->unk_2338[MAP_STATE->mapTiles->rowStart[y] + x] !=
          0) {
        MapUnit* unit =
            &MAP_STATE
                 ->units[MAP_STATE->mapTiles
                             ->unitAt[x + MAP_STATE->mapTiles->rowStart[y]]];

        if (unit->type == UNIT_OOZIUM) {
          func_020f5878(unit->x, unit->y);
          return;
        }
      }
    }
  }
}

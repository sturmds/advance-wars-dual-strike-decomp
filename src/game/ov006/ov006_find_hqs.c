#include "types.h"
#include "game/screen_other_map.h"
#include "game/terrain.h"
#include "game/Unk_ov006_0235f690.h"

void Ov006_SetHqPosition(int hqTile, int x,
                         int y); /* stores x, y as that army's HQ position */

extern Unk_ov006_0235f690* data_ov006_0236d640;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Finds the HQ of each army on the main screen's map, from the list of
 * property tiles (Unk_02080440_48.unk_608c), and records where it is in
 * hqX and hqY (-1: no HQ). Returns how many HQs it found. */
int Ov006_FindHqs(void) {
  int count = 0;
  int i;
  int tile;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  for (i = 0; i < 4; i++) {
    data_ov006_0236d640->hqX[i] = -1;
    data_ov006_0236d640->hqY[i] = -1;
  }
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  for (i = 0; i < 92; i++) {
    tile = STATE->unk_608c[i].tile;
    if (tile == 0xff) {
      break;
    }
    if (tile == 0) {
      continue;
    }
    switch (tile) {
      case MAP_TILE_MAKE(TERRAIN_HQ, 1):
      case MAP_TILE_MAKE(TERRAIN_HQ, 2):
      case MAP_TILE_MAKE(TERRAIN_HQ, 3):
      case MAP_TILE_MAKE(TERRAIN_HQ, 4):
        Ov006_SetHqPosition(tile, STATE->unk_608c[i].x, STATE->unk_608c[i].y);
        count++;
        break;
    }
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  return count;
}

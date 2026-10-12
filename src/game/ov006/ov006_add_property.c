#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov006_0235f690.h"

extern Unk_ov006_0235f690* data_ov006_0236d640;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Appends the property tile at (x, y) to both property lists of the screen in
 * data_027c0288.unk_00 (unk_6088 and unk_608c), ends each list after it with
 * 0xff, and points the tile at its new entry. Does nothing once 92 properties
 * are listed. */
void Ov006_AddProperty(int x, int y, int tile) {
  int count;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  MapTiles* tiles;

  count = data_ov006_0236d640->propertyCount;
  if (count >= 92) {
    return;
  }
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);

  tiles = STATE->mapTiles;
  tiles->unk_1928[x + tiles->rowStart[y]] = count;
  STATE->unk_6088[count].terrain = tile;
  STATE->unk_6088[count].x = x;
  STATE->unk_6088[count].y = y;
  STATE->unk_6088[count].claimCount = 0;
  STATE->unk_6088[count + 1].terrain = 0xff;
  STATE->unk_608c[count].tile = tile;
  STATE->unk_608c[count].x = x;
  STATE->unk_608c[count].y = y;
  STATE->unk_608c[count + 1].tile = 0xff;

  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  data_ov006_0236d640->propertyCount = count + 1;
}

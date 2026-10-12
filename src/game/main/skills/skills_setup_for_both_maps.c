#include "types.h"
#include "game/screen_other_map.h"

void Skills_SetupForBattle(int mode, int mapId, int frontSet, int backSet);

/* Sets up the battle skill sets on map screen 0 with the front CO in set 0
 * and the back CO in set 1. In a two-map battle whose other map has unk_30
 * clear, it repeats on screen 1 with both COs in set 1. */
void Skills_SetupForBothMaps(int mode, int mapId) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx((&data_027c0288.unk_00)[0]);
  saved027c = data_027c027c;
  Screen_SetCurrent((&data_027c0288.unk_00)[0]);
  Skills_SetupForBattle(mode, mapId, 0, 1);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  if (data_027c027c->unk_48[data_027c0284]->secondMapId != 0 &&
      OtherMap_GetUnk30() == 0) {
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx((&data_027c0288.unk_00)[1]);
    saved027c = data_027c027c;
    Screen_SetCurrent((&data_027c0288.unk_00)[1]);
    Skills_SetupForBattle(mode, mapId, 1, 1);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
}

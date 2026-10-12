#include "types.h"
#include "game/screen_other_map.h"
#include "game/terrain.h"

int Ov006_CountArmyFactories(int hqTile);
int Player_CountUnits(int player);

/* Checks the map on the main screen before it can be used: each army that
 * has an HQ also needs a base, airport, port or unit, and at least two
 * armies need to be in play. An army without an HQ is allowed (left out). */
BOOL Ov006_IsMapPlayable(void) {
  int armies;
  int army1;
  int army2;
  int army3;
  int army4;
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  armies = 0;

  army1 = Ov006_CountArmyFactories(MAP_TILE_MAKE(TERRAIN_HQ, 1));
  if (army1 == 0) {
    if (Player_CountUnits(1) > 0) {
      armies++;
      army1 = 1;
    }
  } else if (army1 > 0) {
    armies++;
  }
  army2 = Ov006_CountArmyFactories(MAP_TILE_MAKE(TERRAIN_HQ, 2));
  if (army2 == 0) {
    if (Player_CountUnits(2) > 0) {
      armies++;
      army2 = 1;
    }
  } else if (army2 > 0) {
    armies++;
  }
  army3 = Ov006_CountArmyFactories(MAP_TILE_MAKE(TERRAIN_HQ, 3));
  if (army3 == 0) {
    if (Player_CountUnits(3) > 0) {
      armies++;
      army3 = 1;
    }
  } else if (army3 > 0) {
    armies++;
  }
  army4 = Ov006_CountArmyFactories(MAP_TILE_MAKE(TERRAIN_HQ, 4));
  if (army4 == 0) {
    if (Player_CountUnits(4) > 0) {
      armies++;
      army4 = 1;
    }
  } else if (army4 > 0) {
    armies++;
  }

  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  if (army1 != 0 && army2 != 0 && army3 != 0 && army4 != 0 && armies >= 2) {
    return TRUE;
  }
  return FALSE;
}

#include "types.h"
#include "game/co_data.h"
#include "game/menu_state.h"

BOOL UnitRow_IsAvailable(int row);
int Player_GetUnitIconTile(int player, int unitType);
int Player_GetShownRangeBonus(int player, int slot, int unitType);
int Player_GetShownMoveBonus(int player, int slot, int unitType);
int Player_GetShownFirepower(int player, int slot, int unitType);
int GetUnitCombatClass(int unitType);
void Gfx_ClearLayerMap(int layer);
void Bg_PutTileQuad(int x, int y, int firstTile, int palette);
void Bg_PutThreeTiles(int x, int y, int firstTile, int palette);
void Bg_PutOneTile(int x, int y, int tile, int palette);
void Bg_PutTwoTiles(int x, int y, int firstTile, int palette);
void Gfx_RequestLayerCopy(int layer);

extern MenuState gMenuState;
extern u8 gUnitRowTypes[];

#define MENU (gMenuState.unk_0206d67c)

/* The unit page shows gUnitRowTypes as a grid. */
#define UNIT_GRID_ROWS 6
#define UNIT_GRID_COLUMNS 5

#define UNIT_ICON_TILE 0x365
#define RANGE_ICON_TILE 0x349
#define MOVE_ICON_TILE 0x347
/* Two tiles per value: -2, -1, then 1 to 3 */
#define BONUS_TILE(value) (0x34f + ((value) > 0 ? (value) - 1 : (value)) * 2)
/* Firepower gauge, three tiles per 5% step, from -20 steps to +20 */
#define GAUGE_TILE(step) (0x308 + (step) * 3)

/* Draws the unit page of the player info screen on layer 0: a grid of 6 rows
 * by 5 units from gUnitRowTypes, skipping rows UnitRow_IsAvailable hides.
 * Each unit gets its icon (palette: shown player + 4), a range or movement
 * bonus icon with its value, and a firepower gauge. Transports and units that
 * explode get no gauge. The CO icon table lookup here takes the unit type. */
void PlayerInfo_DrawUnitStats(void) {
  int row;
  int col;
  int unitType;
  int bonus;
  int firepower;

  Gfx_ClearLayerMap(0);
  for (row = 0; row < UNIT_GRID_ROWS; row++) {
    for (col = 0; col < UNIT_GRID_COLUMNS; col++) {
      if (!UnitRow_IsAvailable(row * UNIT_GRID_COLUMNS + col)) {
        continue;
      }
      unitType = gUnitRowTypes[row * UNIT_GRID_COLUMNS + col];
      Bg_PutTileQuad(
          1 + col * 6, 5 + row * 3,
          Player_GetUnitIconTile(MENU.player, unitType) + UNIT_ICON_TILE,
          MENU.player + 4);
      bonus = Player_GetShownRangeBonus(MENU.player, MENU.slot, unitType);
      if (bonus != 0) {
        Bg_PutOneTile(3 + col * 6, 6 + row * 3, RANGE_ICON_TILE, 3);
      } else {
        bonus = Player_GetShownMoveBonus(MENU.player, MENU.slot, unitType);
        if (bonus != 0) {
          Bg_PutOneTile(3 + col * 6, 6 + row * 3, MOVE_ICON_TILE, 3);
        }
      }
      switch (bonus) {
        case -2:
          Bg_PutTwoTiles(4 + col * 6, 6 + row * 3, BONUS_TILE(-2), 3);
          break;
        case -1:
          Bg_PutTwoTiles(4 + col * 6, 6 + row * 3, BONUS_TILE(-1), 3);
          break;
        case 1:
          Bg_PutTwoTiles(4 + col * 6, 6 + row * 3, BONUS_TILE(1), 3);
          break;
        case 2:
          Bg_PutTwoTiles(4 + col * 6, 6 + row * 3, BONUS_TILE(2), 3);
          break;
        case 3:
          Bg_PutTwoTiles(4 + col * 6, 6 + row * 3, BONUS_TILE(3), 3);
          break;
      }
      if (GetUnitCombatClass(unitType) == UNIT_CLASS_TRANSPORT) {
        continue;
      }
      if (GetUnitCombatClass(unitType) == UNIT_CLASS_EXPLODES) {
        continue;
      }
      firepower = Player_GetShownFirepower(MENU.player, MENU.slot, unitType);
      if (firepower == 0) {
        Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(0), 3);
      } else if (firepower < 0) {
        if (firepower < -95) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(-20), 3);
        } else if (firepower < -90) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(-19), 3);
        } else if (firepower < -85) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(-18), 3);
        } else if (firepower < -80) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(-17), 3);
        } else if (firepower < -75) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(-16), 3);
        } else if (firepower < -70) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(-15), 3);
        } else if (firepower < -65) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(-14), 3);
        } else if (firepower < -60) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(-13), 3);
        } else if (firepower < -55) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(-12), 3);
        } else if (firepower < -50) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(-11), 3);
        } else if (firepower < -45) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(-10), 3);
        } else if (firepower < -40) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(-9), 3);
        } else if (firepower < -35) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(-8), 3);
        } else if (firepower < -30) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(-7), 3);
        } else if (firepower < -25) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(-6), 3);
        } else if (firepower < -20) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(-5), 3);
        } else if (firepower < -15) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(-4), 3);
        } else if (firepower < -10) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(-3), 3);
        } else if (firepower < -5) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(-2), 3);
        } else {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(-1), 3);
        }
      } else {
        if (firepower > 95) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(20), 3);
        } else if (firepower > 90) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(19), 3);
        } else if (firepower > 85) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(18), 3);
        } else if (firepower > 80) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(17), 3);
        } else if (firepower > 75) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(16), 3);
        } else if (firepower > 70) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(15), 3);
        } else if (firepower > 65) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(14), 3);
        } else if (firepower > 60) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(13), 3);
        } else if (firepower > 55) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(12), 3);
        } else if (firepower > 50) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(11), 3);
        } else if (firepower > 45) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(10), 3);
        } else if (firepower > 40) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(9), 3);
        } else if (firepower > 35) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(8), 3);
        } else if (firepower > 30) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(7), 3);
        } else if (firepower > 25) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(6), 3);
        } else if (firepower > 20) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(5), 3);
        } else if (firepower > 15) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(4), 3);
        } else if (firepower > 10) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(3), 3);
        } else if (firepower > 5) {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(2), 3);
        } else {
          Bg_PutThreeTiles(3 + col * 6, 5 + row * 3, GAUGE_TILE(1), 3);
        }
      }
    }
  }
  Gfx_RequestLayerCopy(0);
}

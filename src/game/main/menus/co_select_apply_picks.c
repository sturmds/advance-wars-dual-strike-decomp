#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_select_screen.h"
#include "game/map_header.h"
#include "game/screen_other_map.h"

/* The CO the player picked for player slot column (1-4), front or back row.
 * Needed to match: coGrid read as rows of 2. */
#define PICKED_CO(column, row) \
  (((u8(*)[2])data_0229c73c->coGrid)[(column) - 1][row])
#define MAP_CO_PICKED 0x1c /* MapHeader.co: the player picks */

int Campaign_IsHardMode(void);
void MovementCost(void);
void Player_SetCo(int column, int row, int co);
void Skills_SetupForBothMaps(int gameMode, int mapId);

/* Ends CO select: writes the picked COs into the player slots. In a
 * campaign map on normal mode, a CO set by the map header stays unless the
 * header says the player picks. Then sets up the map (Skills_SetupForBothMaps,
 * MovementCost), and runs MovementCost for the second map too when there
 * is one. */
void CoSelect_ApplyPicks(void) {
  int column;
  int row;
  int co;

  if (MAP_STATE->gameMode != GAME_MODE_CAMPAIGN || Campaign_IsHardMode() == 1) {
    for (column = 1; column < data_0229c73c->columnCount + 1; column++) {
      for (row = 0; row < data_0229c73c->rowCount; row++) {
        MAP_STATE->playerCos[column][row] = PICKED_CO(column, row);
        Player_SetCo(column, row, PICKED_CO(column, row));
      }
    }
  } else {
    for (column = 1; column < data_0229c73c->columnCount + 1; column++) {
      for (row = 0; row < data_0229c73c->rowCount; row++) {
        co = Ov000_gMapHeaders[MAP_STATE->unk_1c4->mapId].co[column][row];
        if (co == MAP_CO_PICKED) {
          MAP_STATE->playerCos[column][row] = PICKED_CO(column, row);
          Player_SetCo(column, row, PICKED_CO(column, row));
        } else {
          MAP_STATE->playerCos[column][row] = co;
          Player_SetCo(
              column, row,
              Ov000_gMapHeaders[MAP_STATE->unk_1c4->mapId].co[column][row]);
        }
      }
    }
  }

  if (MAP_STATE->gameMode == 3) {
    return;
  }
  Skills_SetupForBothMaps(MAP_STATE->gameMode, MAP_STATE->unk_1c4->mapId);
  MovementCost();

  if (MAP_STATE->secondMapId == 0) {
    return;
  }
  if (OtherMap_GetUnk30() != 0) {
    return;
  }
  {
    Unk_027c0280* saved0280 = data_027c0280;
    Unk_02080440* saved027c;

    Screen_SetCurrentGfx((&data_027c0288.unk_00)[1]);
    saved027c = data_027c027c;
    Screen_SetCurrent((&data_027c0288.unk_00)[1]);
    MovementCost();
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
}

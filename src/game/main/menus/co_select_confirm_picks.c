#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_select_screen.h"
#include "game/screen_other_map.h"
#include "game/task.h"

/* The CO the player picked for player slot column (1-4), front or back row.
 * Needed to match: coGrid read as rows of 2. */
#define PICKED_CO(column, row) \
  (((u8(*)[2])data_0229c73c->coGrid)[(column) - 1][row])

void func_02001a20(int column, int row,
                   int co); /* Player_SetCo with the CO id bits of co */
void MovementCost(void);
void Skills_SetupForBothMaps(int gameMode, int mapId);
void Skills_LoadPlayer1EquippedOnMainScreen(void);
void UpdatePlayerSkills(int player);
Task* Task_FindByScript(const void* script);
void Task_JumpToLabel(Task* task, int label);

extern const TaskScriptCmd data_0215ab00[];

/* Script step after CO select: gives each player slot the CO picked for it
 * (unlike CoSelect_ApplyPicks, map header COs do not apply). Outside
 * Survival it then sets up the map (Skills_SetupForBothMaps, MovementCost), and the
 * second map too when there is one; in Survival it runs Skills_LoadPlayer1EquippedOnMainScreen and
 * the skills of player 1. Then sends the task running data_0215ab00 to
 * label 2. */
void CoSelect_ConfirmPicks(void) {
  int column;
  int row;

  for (column = 1; column < data_0229c73c->columnCount + 1; column++) {
    for (row = 0; row < data_0229c73c->rowCount; row++) {
      func_02001a20(column, row, PICKED_CO(column, row));
    }
  }

  if (MAP_STATE->gameMode != GAME_MODE_SURVIVAL) {
    Skills_SetupForBothMaps(MAP_STATE->gameMode, MAP_STATE->unk_1c4->mapId);
    MovementCost();
    if (MAP_STATE->secondMapId != 0 && OtherMap_GetUnk30() == 0) {
      Unk_027c0280* saved0280 = data_027c0280;
      Unk_02080440* saved027c;

      Screen_SetCurrentGfx((&data_027c0288.unk_00)[1]);
      saved027c = data_027c027c;
      Screen_SetCurrent((&data_027c0288.unk_00)[1]);
      MovementCost();
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
    }
  } else {
    Skills_LoadPlayer1EquippedOnMainScreen();
    UpdatePlayerSkills(1);
  }
  Task_JumpToLabel(Task_FindByScript(data_0215ab00), 2);
}

#include "types.h"
#include "game/build_menu.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern s8 data_ov000_022b3834[];

int Unit_GetValuePerHp(int playerIndex, int unitType);
BOOL BuildMenu_CanBuildUnit(int player, int unitType);

/* Fills gBuildMenuEntries with the unit types of the build order list
 * (data_ov000_022b3834, ends at -1) that are in one of the MOVE_GROUP_* bits
 * of moveGroups and that the current player may build (BuildMenu_CanBuildUnit), and
 * marks those the player cannot afford. */
void BuildMenu_FillList(int moveGroups) {
  u8 i;
  u8 type;
  u8 count;
  Unk_02080440_48* state;

  count = 0;
  for (i = 0; data_ov000_022b3834[i] != -1; i++) {
    type = data_ov000_022b3834[i];
    if ((moveGroups & Ov000_gUnitStats[type].moveGroup) &&
        BuildMenu_CanBuildUnit(MAP_STATE->currentPlayer, type)) {
      state = MAP_STATE;
      /* needed to match: the original compares unsigned */
      if ((u32)state->players[state->currentPlayer].funds <
          Unit_GetValuePerHp(state->currentPlayer, type) * 10) {
        gBuildMenuEntries[count].state = BUILD_ENTRY_TOO_EXPENSIVE;
      } else {
        gBuildMenuEntries[count].state = BUILD_ENTRY_AFFORDABLE;
      }
      gBuildMenuEntries[count].unitType = type;
      count++;
    }
  }
  gBuildMenuCount = count;
  gBuildMenuEntries[count].unitType = 0;
}

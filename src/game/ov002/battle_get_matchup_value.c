#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

int Battle_GetAirGroundMatchup(int side);

/* Needed to match: every case reads the entry again. */
#define ENTRY(side)          \
  (BATTLE_SIDE_DATA_AT(side) \
       ->unk_1c[Ov002_gBattleScene->sides[side].usesSecondary])

/* Like Battle_GetMatchupNibble0, for the third list of entries; 0 when the side has
 * nothing in gMenuState.battle.unk_40. Needed to match: that slot is read through
 * gMenuState's own address, as BATTLE_SIDE_DATA_AT does. */
int Battle_GetMatchupValue(int side) {
  if (((void**)&gMenuState + side)[0x40 / 4] == NULL) {
    return 0;
  }
  switch (Battle_GetAirGroundMatchup(side)) {
    case 0:
      return ENTRY(side)->unk_08_0;
    case 1:
      return ENTRY(side)->unk_08_4;
    case 2:
      return ENTRY(side)->unk_09;
    default:
      return ENTRY(side)->unk_08_0;
  }
}

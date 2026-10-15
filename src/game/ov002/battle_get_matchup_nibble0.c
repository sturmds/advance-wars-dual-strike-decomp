#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"

int Battle_GetAirGroundMatchup(int side);

/* Needed to match: every case reads the entry again. */
#define ENTRY(side)          \
  (BATTLE_SIDE_DATA_AT(side) \
       ->weapons[Ov002_gBattleScene->sides[side].usesSecondary])

/* Picks a 4-bit value from the side's weapon entry. Which one depends on the matchup
 * Battle_GetAirGroundMatchup reports for the side. */
int Battle_GetMatchupNibble0(int side) {
  switch (Battle_GetAirGroundMatchup(side)) {
    case 0:
      return ENTRY(side)->sameAltitude;
    case 1:
      return ENTRY(side)->groundVsAir;
    case 2:
      return ENTRY(side)->airVsGround;
    default:
      return ENTRY(side)->sameAltitude;
  }
}

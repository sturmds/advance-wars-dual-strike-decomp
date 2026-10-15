#include "types.h"
#include "game/menu_state.h"

BOOL HasTwoOrMoreTeams(void) {
  int teamUsed[4];
  int i;
  int teamCount;

  for (i = 0; i < (int)ARRAY_COUNT(teamUsed); i++) {
    teamUsed[i] = 0;
  }
  for (i = 0; i < gMenuState.setup.unk_2e3; i++) {
    teamUsed[gMenuState.setup.teams[i]] = 1;
  }
  teamCount = 0;
  for (i = 0; i < (int)ARRAY_COUNT(teamUsed); i++) {
    teamCount += teamUsed[i];
  }
  return teamCount >= 2;
}

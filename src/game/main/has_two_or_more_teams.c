#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

BOOL HasTwoOrMoreTeams(void) {
  int teamUsed[4];
  int i;
  int teamCount;

  for (i = 0; i < 4; i++) {
    teamUsed[i] = 0;
  }
  for (i = 0; i < gMenuState.setup.unk_2e3; i++) {
    teamUsed[gMenuState.setup.teams[i]] = 1;
  }
  teamCount = 0;
  for (i = 0; i < 4; i++) {
    teamCount += teamUsed[i];
  }
  return teamCount >= 2;
}

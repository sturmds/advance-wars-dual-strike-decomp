#include "types.h"
#include "game/event_queue.h"
#include "game/menu_state.h"

BOOL HasTwoOrMoreTeams(void);
int Sound_PlaySe(int);

extern MenuState gMenuState;

/* Like VsSetup_NextTeam, for slot (message - 19), down by one, 0 wrapping to 3. */
void VsSetup_PrevTeam(int message) {
  /* Needed to match: the slot is added to the state's address, and teams[0]
     * is read from that shifted view. */
  MenuState* shifted;

  Sound_PlaySe(0x10002);
  shifted = (MenuState*)((u8*)&gMenuState + (u8)(message - EVENT_VS_TEAM_PREV));
  do {
    if (shifted->setup.teams[0] == 0) {
      shifted->setup.teams[0] = 3;
    } else {
      shifted->setup.teams[0]--;
    }
  } while (!HasTwoOrMoreTeams());
}

#include "types.h"
#include "game/event_queue.h"
#include "game/menu_state.h"

BOOL HasTwoOrMoreTeams(void);
int Sound_PlaySe(int);

extern MenuState gMenuState;

/* Plays the cursor sound and moves the team of slot (message - 23) up by one,
 * 3 wrapping to 0, until at least two teams are in use. */
void VsSetup_NextTeam(int message) {
  /* Needed to match: the slot is added to the state's address, and teams[0]
     * is read from that shifted view. */
  MenuState* shifted;

  Sound_PlaySe(0x10002);
  shifted = (MenuState*)((u8*)&gMenuState + (u8)(message - EVENT_VS_TEAM_NEXT));
  do {
    if (shifted->setup.teams[0] == 3) {
      shifted->setup.teams[0] = 0;
    } else {
      shifted->setup.teams[0]++;
    }
  } while (!HasTwoOrMoreTeams());
}

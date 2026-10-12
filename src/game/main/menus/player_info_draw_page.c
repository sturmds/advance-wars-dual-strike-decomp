#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

void PlayerInfo_DrawCoProfile(void);
void CoStatsScreen_DrawSkills(void);
void PlayerInfo_DrawPowers(void);
void PlayerInfo_DrawUnitStats(void);

/* Draws the text of the page the player info screen shows. */
void PlayerInfo_DrawPage(void) {
  switch (gMenuState.unk_0206d67c.page) {
    case 0:
      PlayerInfo_DrawCoProfile();
      break;
    case 1:
      CoStatsScreen_DrawSkills();
      break;
    case 2:
      PlayerInfo_DrawPowers();
      break;
    default:
      PlayerInfo_DrawUnitStats();
      break;
  }
}

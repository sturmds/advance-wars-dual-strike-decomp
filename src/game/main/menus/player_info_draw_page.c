#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

void PlayerInfo_DrawCoName(void);
void CoStatsScreen_DrawSkills(void);
void func_0206e814(void);
void func_0206d67c(void);

/* Draws the text of the page the player info screen shows. */
void PlayerInfo_DrawPage(void)
{
    switch (gMenuState.unk_0206d67c.page) {
    case 0:
        PlayerInfo_DrawCoName();
        break;
    case 1:
        CoStatsScreen_DrawSkills();
        break;
    case 2:
        func_0206e814();
        break;
    default:
        func_0206d67c();
        break;
    }
}

#include "types.h"
#include "game/menu_state.h"

int BattleSetup_IsCursorOnCo(void);
void Sound_PlaySe(int id);
void VsSetup_RedrawSkills(void);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState *)&gMenuState)

#define SELECTED_SLOT() ((MENU->setup.unk_2e4 - 8) >> 1)
#define SELECTED_CO()   ((MENU->setup.unk_2e4 - 8) & 1)
#define SELECTED_SET()  MENU->setup.forceRankSets[SELECTED_SLOT()][SELECTED_CO()]

/* Moves the selected CO to its next Force Rank set, 0-3 with wrap. */
void VsSetup_NextForceRankSet(int unused)
{
    if (BattleSetup_IsCursorOnCo() == 0) {
        return;
    }
    if (MENU->setup.cursorSePlayed == 0) {
        Sound_PlaySe(0x10002);
        MENU->setup.cursorSePlayed = 1;
    }
    if (SELECTED_SET() == 3) {
        SELECTED_SET() = 0;
    } else {
        SELECTED_SET()++;
    }
    VsSetup_RedrawSkills();
}

#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"

int Sound_PlaySe(int);
void HelpText_Show(u32 textId, int tile);
int BattleSetup_IsCursorOnCo(void);
void VsSetup_RedrawSkills(void);
void VsSetup_RedrawAllSkills(void);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offsets out of the literal pool. */
#define MENU ((MenuState *)&gMenuState)

#define HELP_TEXT(n) (0x7f000000 + (n))
#define HELP_TILE 960

/* Called when the cursor moves to a player or CO button of the Vs setup
 * screen. Plays the cursor sound (once a frame), stores the button id in
 * setup.unk_2e4, redraws the skill window and shows the help text: one for
 * a CO button, another for the rest. */
void VsSetup_OnButtonSelected(MenuButton *button)
{
    if (MENU->setup.cursorSePlayed == 0) {
        Sound_PlaySe(0x10002);
        MENU->setup.cursorSePlayed = 1;
    }
    MENU->setup.unk_2e4 = button->id;
    VsSetup_RedrawSkills();
    VsSetup_RedrawAllSkills();
    if (BattleSetup_IsCursorOnCo() == 1) {
        HelpText_Show(HELP_TEXT(0x18), HELP_TILE);
    } else {
        HelpText_Show(HELP_TEXT(0x19), HELP_TILE);
    }
}

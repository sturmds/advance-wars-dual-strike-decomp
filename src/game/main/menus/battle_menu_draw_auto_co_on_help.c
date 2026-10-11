#include "types.h"

void BattleMenu_DrawHelpText(u32 textId);

/* Help line shown after Menu_ToggleAutoCo hands the player to the AI. */
void BattleMenu_DrawAutoCoOnHelp(void)
{
    BattleMenu_DrawHelpText(0xc00002e4);
}

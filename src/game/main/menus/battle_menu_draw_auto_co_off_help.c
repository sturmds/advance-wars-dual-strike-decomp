#include "types.h"

void BattleMenu_DrawHelpText(u32 textId);

/* Help line shown after Menu_ToggleAutoCo gives the player back control. */
void BattleMenu_DrawAutoCoOffHelp(void)
{
    BattleMenu_DrawHelpText(0xc00002e5);
}

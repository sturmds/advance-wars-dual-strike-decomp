#include "types.h"

void BattleMenu_DrawHelpText(u32 textId);

void BattleMenu_DrawMusicOffHelp(void)
{
    BattleMenu_DrawHelpText(0xc00002f8);
}

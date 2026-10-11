#include "types.h"

void BattleMenu_DrawHelpText(u32 textId);

void BattleMenu_DrawMusicOnHelp(void)
{
    BattleMenu_DrawHelpText(0xc00002f7);
}

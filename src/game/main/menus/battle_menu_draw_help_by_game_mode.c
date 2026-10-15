#include "types.h"
#include "game/Unk_02080440.h"

void BattleMenu_DrawHelpText(u32 textId);

/* Menu help callback: three texts, one for the campaign, one for survival
 * and one for every other mode. */
void BattleMenu_DrawHelpByGameMode(void) {
  switch (data_027c027c->unk_48[data_027c0284]->gameMode) {
    case GAME_MODE_CAMPAIGN:
      BattleMenu_DrawHelpText(0xc00002c9);
      break;
    case GAME_MODE_SURVIVAL:
      BattleMenu_DrawHelpText(0xc00002ca);
      break;
    default:
      BattleMenu_DrawHelpText(0xc00002c8);
      break;
  }
}

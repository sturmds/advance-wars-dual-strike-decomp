#include "types.h"
#include "game/menu_state.h"
#include "game/visual_mode.h"

void HelpText_Show(u32 textId, int tile);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offsets out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

#define HELP_TEXT(n) (0x7f000000 + (n))
#define HELP_TILE 960

/* Like VsSetup_ShowSettingHelp, with the texts 0x1a on (26 to 48). Called when a
 * setting row is picked (func_02064af0); what sets the two apart is unknown.
 * The name is a guess. */
void VsSetup_ShowSettingHelp2(void) {
  switch (MENU->setup.settingRow) {
    case 0:
      HelpText_Show(HELP_TEXT(26), HELP_TILE);
      break;
    case 1:
      switch (MENU->setup.unk_2dc) {
        case 0:
          HelpText_Show(HELP_TEXT(27), HELP_TILE);
          break;
        case 1:
          HelpText_Show(HELP_TEXT(28), HELP_TILE);
          break;
        case 2:
          HelpText_Show(HELP_TEXT(30), HELP_TILE);
          break;
        case 3:
          HelpText_Show(HELP_TEXT(29), HELP_TILE);
          break;
        case 4:
          HelpText_Show(HELP_TEXT(31), HELP_TILE);
          break;
      }
      break;
    case 2:
      HelpText_Show(HELP_TEXT(32) + MENU->setup.unk_2dd, HELP_TILE);
      break;
    case 3:
      HelpText_Show(HELP_TEXT(36), HELP_TILE);
      break;
    case 4:
      HelpText_Show(HELP_TEXT(37), HELP_TILE);
      break;
    case 5:
      HelpText_Show(HELP_TEXT(38), HELP_TILE);
      break;
    case 6:
      HelpText_Show(HELP_TEXT(39), HELP_TILE);
      break;
    case 7:
      switch (MENU->setup.visualMode) {
        case VISUAL_NONE:
          HelpText_Show(HELP_TEXT(40), HELP_TILE);
          break;
        case VISUAL_A:
          HelpText_Show(HELP_TEXT(41), HELP_TILE);
          break;
        case VISUAL_B:
          HelpText_Show(HELP_TEXT(42), HELP_TILE);
          break;
        case VISUAL_C:
          HelpText_Show(HELP_TEXT(43), HELP_TILE);
          break;
        case VISUAL_D:
          HelpText_Show(HELP_TEXT(44), HELP_TILE);
          break;
      }
      break;
    case 8:
      switch (MENU->setup.unk_2e2) {
        case 0:
          HelpText_Show(HELP_TEXT(45), HELP_TILE);
          break;
        case 1:
          HelpText_Show(HELP_TEXT(46), HELP_TILE);
          break;
        case 2:
          HelpText_Show(HELP_TEXT(47), HELP_TILE);
          break;
        case 3:
          HelpText_Show(HELP_TEXT(48), HELP_TILE);
          break;
      }
      break;
  }
}

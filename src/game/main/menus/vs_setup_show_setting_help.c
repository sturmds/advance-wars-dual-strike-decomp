#include "types.h"
#include "game/menu_state.h"
#include "game/visual_mode.h"

void HelpText_Show(u32 textId, int tile);

/* Needed to match: the cast keeps the field offsets out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

#define HELP_TEXT(n) (0x7f000000 + (n))
#define HELP_TILE 960

/* Shows the help text (group 0x7f00) for the setting row in setup.settingRow. For
 * weather (unk_2dc), the tileset (unk_2dd), visualMode and unk_2e2 the text
 * depends on the value. The name is a guess. */
void VsSetup_ShowSettingHelp(void) {
  switch (MENU->setup.settingRow) {
    case 0:
      HelpText_Show(HELP_TEXT(0), HELP_TILE);
      break;
    case 1:
      switch (MENU->setup.unk_2dc) {
        case 0:
          HelpText_Show(HELP_TEXT(1), HELP_TILE);
          break;
        case 1:
          HelpText_Show(HELP_TEXT(2), HELP_TILE);
          break;
        case 2:
          HelpText_Show(HELP_TEXT(4), HELP_TILE);
          break;
        case 3:
          HelpText_Show(HELP_TEXT(3), HELP_TILE);
          break;
        case 4:
          HelpText_Show(HELP_TEXT(5), HELP_TILE);
          break;
      }
      break;
    case 2:
      HelpText_Show(HELP_TEXT(6) + MENU->setup.unk_2dd, HELP_TILE);
      break;
    case 3:
      HelpText_Show(HELP_TEXT(10), HELP_TILE);
      break;
    case 4:
      HelpText_Show(HELP_TEXT(11), HELP_TILE);
      break;
    case 5:
      HelpText_Show(HELP_TEXT(12), HELP_TILE);
      break;
    case 6:
      HelpText_Show(HELP_TEXT(13), HELP_TILE);
      break;
    case 7:
      switch (MENU->setup.visualMode) {
        case VISUAL_NONE:
          HelpText_Show(HELP_TEXT(14), HELP_TILE);
          break;
        case VISUAL_A:
          HelpText_Show(HELP_TEXT(15), HELP_TILE);
          break;
        case VISUAL_B:
          HelpText_Show(HELP_TEXT(16), HELP_TILE);
          break;
        case VISUAL_C:
          HelpText_Show(HELP_TEXT(17), HELP_TILE);
          break;
        case VISUAL_D:
          HelpText_Show(HELP_TEXT(18), HELP_TILE);
          break;
      }
      break;
    case 8:
      switch (MENU->setup.unk_2e2) {
        case 0:
          HelpText_Show(HELP_TEXT(19), HELP_TILE);
          break;
        case 1:
          HelpText_Show(HELP_TEXT(20), HELP_TILE);
          break;
        case 2:
          HelpText_Show(HELP_TEXT(21), HELP_TILE);
          break;
        case 3:
          HelpText_Show(HELP_TEXT(22), HELP_TILE);
          break;
      }
      break;
  }
}

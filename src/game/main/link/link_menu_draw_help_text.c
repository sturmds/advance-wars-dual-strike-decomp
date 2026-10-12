#include "types.h"
#include "game/Unk_0229fb30.h"
#include "game/Unk_02160ac0.h"

void HelpText_Show(u32 textId, int tile);

/* Prints the help text of menu entry unk_45b (0-4); entry 4 depends on data_02160ac0->unk_83. */
void LinkMenu_DrawHelpText(void) {
  switch (data_0229fb30.unk_45b) {
    case 0:
      HelpText_Show(0x44000016, 960);
      break;
    case 1:
      HelpText_Show(0x44000017, 960);
      break;
    case 2:
      HelpText_Show(0x44000018, 960);
      break;
    case 3:
      HelpText_Show(0x44000019, 960);
      break;
    case 4:
      switch (data_02160ac0->unk_83) {
        case 0:
          HelpText_Show(0x4400001a, 960);
          break;
        case 1:
          HelpText_Show(0x4400001b, 960);
          break;
        case 2:
          HelpText_Show(0x4400001c, 960);
          break;
        case 3:
          HelpText_Show(0x4400001d, 960);
          break;
        case 4:
          HelpText_Show(0x4400001e, 960);
          break;
      }
      break;
  }
}

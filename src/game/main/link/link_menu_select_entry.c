#include "types.h"
#include "game/Unk_0229fb30.h"

void Sound_PlaySe(u32 soundId);
void LinkMenu_DrawHelpText(void);

/* Link message 6 from player 0: selects link menu entry `entry` and shows its help text. */
void LinkMenu_SelectEntry(int entry) {
  Sound_PlaySe(0x10002);
  data_0229fb30.unk_45b = entry;
  LinkMenu_DrawHelpText();
}

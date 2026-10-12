#include "types.h"
#include "game/Unk_0229fb30.h"
#include "game/Unk_02160ac0.h"

void Sound_PlaySe(u32 soundId);
void LinkMenu_DrawHelpText(void);

/* Link message 7 from player 0: raises the setting of the selected link
 * menu entry (data_0229fb30.unk_45b). Entries 0 and 2 toggle, the others
 * stop at their highest value. */
void LinkMenu_IncreaseSetting(void) {
  Sound_PlaySe(0x10002);
  switch (data_0229fb30.unk_45b) {
    case 0:
      data_02160ac0->unk_7f++;
      data_02160ac0->unk_7f &= 1;
      break;
    case 1:
      if (data_02160ac0->unk_78 < 95000) {
        data_02160ac0->unk_78 += 5000;
      }
      break;
    case 2:
      data_02160ac0->unk_80++;
      data_02160ac0->unk_80 &= 1;
      break;
    case 3:
      if (data_02160ac0->unk_81 < 8) {
        data_02160ac0->unk_81++;
      }
      break;
    case 4:
      if (data_02160ac0->unk_83 < 4) {
        data_02160ac0->unk_83++;
      }
      break;
  }
  LinkMenu_DrawHelpText();
}

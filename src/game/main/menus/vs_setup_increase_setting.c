#include "types.h"
#include "game/menu_state.h"
#include "game/visual_mode.h"

void Sound_PlaySe(u32 soundId);
void VsSetup_ShowSettingHelp(void);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offsets out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

#define SE_MOVE 0x10002

/* Steps the setting in the selected row (setup.settingRow) of the Vs setup
 * screen one value on, wrapping around, then shows the row's help and
 * plays a sound. VsSetup_DecreaseSetting steps the other way. */
void VsSetup_IncreaseSetting(void) {
  switch (MENU->setup.settingRow) {
    case 0:
      MENU->setup.fog++;
      MENU->setup.fog &= 1;
      break;
    case 1:
      if (MENU->setup.unk_2dc == 4) {
        MENU->setup.unk_2dc = 0;
      } else {
        MENU->setup.unk_2dc++;
      }
      break;
    case 2:
      if (MENU->setup.unk_2dd == 3) {
        MENU->setup.unk_2dd = 0;
      } else {
        MENU->setup.unk_2dd++;
      }
      break;
    case 3:
      /* Funds per property in steps of 500, up to 9500. Needed to
         * match: the test names gMenuState, the update MENU. */
      if (gMenuState.setup.fundsPerProperty < 9500) {
        MENU->setup.fundsPerProperty += 500;
      }
      break;
    case 4:
      /* 0 (off), then 5 to 99 */
      if (MENU->setup.unk_2de == 99) {
        MENU->setup.unk_2de = 0;
      } else if (MENU->setup.unk_2de == 0) {
        MENU->setup.unk_2de = 5;
      } else {
        MENU->setup.unk_2de++;
      }
      break;
    case 5:
      /* 0 (off), then unk_2e7 to unk_2e8 */
      if (MENU->setup.unk_2df == MENU->setup.unk_2e8) {
        MENU->setup.unk_2df = 0;
      } else if (MENU->setup.unk_2df == 0) {
        MENU->setup.unk_2df = MENU->setup.unk_2e7;
      } else {
        MENU->setup.unk_2df++;
      }
      break;
    case 6:
      MENU->setup.coEffectsOn++;
      MENU->setup.coEffectsOn &= 1;
      break;
    case 7:
      switch (MENU->setup.visualMode) {
        case VISUAL_NONE:
          MENU->setup.visualMode = VISUAL_A;
          break;
        case VISUAL_A:
          MENU->setup.visualMode = VISUAL_B;
          break;
        case VISUAL_B:
          MENU->setup.visualMode = VISUAL_C;
          break;
        case VISUAL_C:
          MENU->setup.visualMode = VISUAL_D;
          break;
        case VISUAL_D:
          MENU->setup.visualMode = VISUAL_NONE;
          break;
      }
      break;
    case 8:
      if (MENU->setup.unk_2e2 == 3) {
        MENU->setup.unk_2e2 = 0;
      } else {
        MENU->setup.unk_2e2++;
      }
      break;
  }
  VsSetup_ShowSettingHelp();
  Sound_PlaySe(SE_MOVE);
}

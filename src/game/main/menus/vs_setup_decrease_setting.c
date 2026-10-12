#include "types.h"
#include "game/menu_state.h"

void Sound_PlaySe(u32 soundId);
void VsSetup_ShowSettingHelp(void);

extern MenuState gMenuState;
extern MenuStateFrom200 data_0229ed30;

/* Needed to match: the cast keeps the field offsets out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

#define SE_MOVE 0x10002

/* Steps the setting in the selected row (setup.settingRow) of the Vs setup
 * screen one value back, wrapping around, then shows the row's help and
 * plays a sound. VsSetup_IncreaseSetting steps the other way. */
void VsSetup_DecreaseSetting(void) {
  switch (MENU->setup.settingRow) {
    case 0:
      MENU->setup.fog++;
      MENU->setup.fog &= 1;
      break;
    case 1:
      if (MENU->setup.unk_2dc == 0) {
        MENU->setup.unk_2dc = 4;
      } else {
        MENU->setup.unk_2dc--;
      }
      break;
    case 2:
      if (MENU->setup.unk_2dd == 0) {
        MENU->setup.unk_2dd = 3;
      } else {
        MENU->setup.unk_2dd--;
      }
      break;
    case 3:
      /* Funds per property in steps of 500, down to 500. Needed to
         * match: the test reads through data_0229ed30. */
      if (data_0229ed30.fundsPerProperty > 500) {
        MENU->setup.fundsPerProperty -= 500;
      }
      break;
    case 4:
      /* 0 (off), then 5 to 99 */
      if (MENU->setup.unk_2de == 0) {
        MENU->setup.unk_2de = 99;
      } else if (MENU->setup.unk_2de == 5) {
        MENU->setup.unk_2de = 0;
      } else {
        MENU->setup.unk_2de--;
      }
      break;
    case 5:
      /* 0 (off), then unk_2e7 to unk_2e8 */
      if (MENU->setup.unk_2df == 0) {
        MENU->setup.unk_2df = MENU->setup.unk_2e8;
      } else if (MENU->setup.unk_2df == MENU->setup.unk_2e7) {
        MENU->setup.unk_2df = 0;
      } else {
        MENU->setup.unk_2df--;
      }
      break;
    case 6:
      MENU->setup.coEffectsOn++;
      MENU->setup.coEffectsOn &= 1;
      break;
    case 7:
      switch (MENU->setup.unk_2e1) {
        case 0:
          MENU->setup.unk_2e1 = 4;
          break;
        case 1:
          MENU->setup.unk_2e1 = 0;
          break;
        case 2:
          MENU->setup.unk_2e1 = 1;
          break;
        case 3:
          MENU->setup.unk_2e1 = 2;
          break;
        case 4:
          MENU->setup.unk_2e1 = 3;
          break;
      }
      break;
    case 8:
      if (MENU->setup.unk_2e2 == 0) {
        MENU->setup.unk_2e2 = 3;
      } else {
        MENU->setup.unk_2e2--;
      }
      break;
  }
  VsSetup_ShowSettingHelp();
  Sound_PlaySe(SE_MOVE);
}

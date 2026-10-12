#include "types.h"
#include "game/link_menu_state.h"

void Sound_PlaySe(u32 soundId);
int Wireless_GetState(void);
void Link_Stop(void);
void func_0207bbd0(void);

#define SE_CANCEL 0x10009

/* B on the player list, unless a choice was made. Link states 2 and 3 run
 * Link_Stop; states 4 and 5 run func_0207bbd0 and lock the screen. */
void LinkMenu_Cancel(void) {
  if (data_0215fed0->locked == 1) {
    return;
  }
  switch (Wireless_GetState()) {
    case 3:
      Link_Stop();
      Sound_PlaySe(SE_CANCEL);
      return;
    case 2:
      Link_Stop();
      Sound_PlaySe(SE_CANCEL);
      return;
    case 4:
      func_0207bbd0();
      Sound_PlaySe(SE_CANCEL);
      data_0215fed0->locked = 1;
      return;
    case 5:
      func_0207bbd0();
      Sound_PlaySe(SE_CANCEL);
      data_0215fed0->locked = 1;
      break;
  }
}

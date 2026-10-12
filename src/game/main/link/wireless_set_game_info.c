#include "types.h"
#include "game/Unk_0215fe84.h"
#include "game/wireless_parent_info.h"
#include "sdk/wm.h"

void func_02079098(void* event);
int WM_SetGameInfo(void (*callback)(void* event), void* userGameInfo,
                   u16 length, u32 ggid, u16 tgid, u8 attribute);

/* Sets the beacon data of the parent. It accepts new children
 * (WM_ATTR_FLAG_ENTRY) while Unk_0215fe84.unk_18 is 1. True when the request
 * started. */
BOOL Wireless_SetGameInfo(void) {
  u8 attribute = 0;

  if (data_0215fe84->unk_18 == 1) {
    attribute |= WM_ATTR_FLAG_ENTRY;
  }
  if (WM_SetGameInfo(func_02079098, data_0229c8a0->userGameInfo,
                     data_0229c8a0->userGameInfoLength, data_0229c8a0->ggid,
                     data_0229c8a0->tgid, attribute) != 2) {
    return FALSE;
  }
  return TRUE;
}

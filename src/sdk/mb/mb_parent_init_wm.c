#include "types.h"
#include "sdk/mb.h"
#include "sdk/wm.h"

/* also declared in src/sdk/mb/download_play_set_up_work.c and 1 more */
u16 data_022aced0;
void* data_022acedc; /* also declared in src/sdk/mb/mb_parent_start_core.c */
/* also declared in src/game/main/func_0211c0d8.c and 21 more */
DlPlayBufferArea* data_022aced4;
/* also declared in src/game/main/func_0211be3c.c and 14 more */
DlPlayStationArea* data_022aced8;
/* declared in src/game/main/func_0211bfb8.c, with a type no header defines */
u8 data_022acee0[0x8];

void func_0211bfb8(int);
void MbParent_SetIndAndLifeTime(void);

/* Clears the parent's progress, then brings WM up (WM_Initialize)
 * unless the game started Download Play earlier, and installs the indication
 * callback. Returns 0, or 8 when WM_Initialize fails. */
int MbParent_InitWm(void) {
  data_022aced8->unk_528 = 0;
  data_022aced8->unk_52a = 0;
  data_022aced8->unk_526 = 0;
  data_022aced8->unk_548 = 0;
  func_0211bfb8(10);
  if (data_022aced4->parent.unk_320 == 0) {
    int result;

    do {
      result = WM_Initialize(data_022acedc, data_022aced8->wmCallback,
                             data_022aced0);
    } while (result == WM_ERRCODE_WM_DISABLE);
    if (result != WM_ERRCODE_OPERATING) {
      return 8;
    }
    WM_SetIndCallback(data_022aced8->wmCallback);
    data_022aced8->started = 1;
    return 0;
  }
  WM_SetIndCallback(data_022aced8->wmCallback);
  data_022aced8->started = 1;
  MbParent_SetIndAndLifeTime();
  return 0;
}

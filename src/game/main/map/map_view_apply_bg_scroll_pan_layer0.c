#include "types.h"
#include "game/Unk_02080440.h"
#include "game/screen_other_map.h"

extern int gCameraPresetIndex;
extern u8 data_021576fa[][20];
extern u8 data_021576fb[][20];

int MainLoop_GetFrameCount(void);
int SceneMode_IsCurrent(int a0);
int Map_IsThisMapsTurn(void);
void Bg_SetScroll(int layer, int scrollX, int scrollY);

#define GAME (data_027c027c->unk_48[data_027c0284])
#define VIEW (GAME->unk_1c4)

/* Screen config hook (data_02169f48 record 4). Only while SceneMode_IsCurrent(4)
 * holds: when it is the other map's turn and unk_86 is set, the second map
 * screen copies the first one's scroll and centres its cursor in the view.
 * Then it sets the scroll of background layers 0, 1 and 3 like
 * MapView_ApplyBgScroll, with layer 0 offset by the frame count (x + 1 per
 * 8 frames, y - 1 per 16). */
void MapView_ApplyBgScrollPanLayer0(void) {
  int panX = MainLoop_GetFrameCount() >> 3;
  int panY = -(MainLoop_GetFrameCount() >> 4);
  s16 scrollX;
  s16 scrollY;
  int halfW;
  int halfH;

  if (!SceneMode_IsCurrent(4)) {
    return;
  }

  if (!Map_IsThisMapsTurn() && GAME->unk_86 != 0) {
    halfW = data_021576fa[gCameraPresetIndex][0] >> 1;
    halfH = data_021576fb[gCameraPresetIndex][0] >> 1;
    {
      Unk_027c0280* saved0280 = data_027c0280;
      Unk_02080440* saved027c;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      scrollX = VIEW->scrollX;
      scrollY = VIEW->scrollY;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
    }
    {
      Unk_027c0280* saved0280 = data_027c0280;
      Unk_02080440* saved027c;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      VIEW->scrollX = scrollX;
      VIEW->scrollY = scrollY;
      GAME->cursorX = halfW + (VIEW->scrollX >> 4);
      GAME->cursorY = halfH + (VIEW->scrollY >> 4);
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
    }
  }

  Bg_SetScroll(0, panX + (VIEW->scrollX - VIEW->unk_26 * 16),
               panY + (VIEW->scrollY - VIEW->unk_28 * 16));
  Bg_SetScroll(1, VIEW->scrollX - VIEW->unk_26 * 16,
               VIEW->scrollY - VIEW->unk_28 * 16);
  Bg_SetScroll(3, VIEW->scrollX - VIEW->unk_26 * 16,
               VIEW->scrollY - VIEW->unk_28 * 16);
}

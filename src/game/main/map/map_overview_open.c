#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;

void func_020c4018(void);
void Player_SaveArmyColours(void);
void MapPreview_Start(u8* charBase, u16* screen, int firstTile, int palette);
void Gfx_RequestLayerCopy(int layer);
void Screen_ResetWindows(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* First step of script gMapOverviewScript, which MapCursor_HandleButtons starts
 * through Script_StartWithHook. Saves the view scroll, draws the map preview
 * on layer 0 and puts it in front, and sets up a darken blend on the other
 * layers that MapOverview_DimStep then fades in. "Overview" is a guess at what the
 * player sees. */
void MapOverview_Open(MapOverviewSlot* slot) {
  func_020c4018();
  slot->savedScrollX = STATE->unk_1c4->scrollX;
  slot->savedScrollY = STATE->unk_1c4->scrollY;
  Player_SaveArmyColours();
  MapPreview_Start(
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14),
      data_027c0280->layer0Map, 1, 1);
  Gfx_RequestLayerCopy(0);
  Screen_ResetWindows();
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
  data_027c0280->blendMode = BLEND_MODE_DARKEN;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendTopObj = 1;
}

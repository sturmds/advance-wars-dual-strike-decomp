#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Gfx_RequestLayerCopy(int layer);
void HelpText_Show(u32 textId, int tile);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern MenuState gMenuState;

/* Main screen: copies the map kept at the start of gMenuState back to
 * layer 2, then sets alpha blending with no first target and every BG and
 * OBJ as second targets (eva 8, evb 0), so only semi-transparent sprites
 * blend. Moves
 * the menu cursor off screen and shows help text 0x44000015, the entry
 * before LinkMenu_DrawHelpText's texts. */
void LinkMenu_RestoreMainScreen(void) {
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* savedScreen;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Mem_Copy32(&gMenuState, data_027c0280->layer2Map, BG_VISIBLE_SIZE);
  Gfx_RequestLayerCopy(2);
  data_027c0280->blendMode = BLEND_MODE_ALPHA;
  data_027c0280->blendEva = 8;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendBottomObj = 1;
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
  MenuCursor_SetRect(0, -16, 256, -16);
  HelpText_Show(0x44000015, 960);
}

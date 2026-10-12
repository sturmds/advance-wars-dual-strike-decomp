#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetCurrentBoth(void* state);
void Screen_SetBgMode(int mode);
void Screen_InitLayers(const void* config);
void Screen_SetLayer0Scroll(int x, int y);
void Screen_SetLayer1Scroll(int x, int y);
void Screen_SetLayer2Scroll(int x, int y);
void Screen_SetLayer3Scroll(int x, int y);
void Ov006_LoadMapeditGraphics(void);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern u8 data_021575c8[]; /* main screen layer config */

#define RESET_LAYERS()                                        \
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0; \
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1; \
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2; \
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3; \
  data_027c0280->blendMode = 0;                               \
  data_027c0280->blendEva = 16;                               \
  data_027c0280->blendEvb = 0;                                \
  data_027c0280->blendEvy = 0;                                \
  data_027c0280->dispCnt |= 0x40;                             \
  data_027c0280->unk_dd = 0;                                  \
  Screen_SetLayer0Scroll(0, 0);                               \
  Screen_SetLayer1Scroll(0, 0);                               \
  Screen_SetLayer2Scroll(0, 0);                               \
  Screen_SetLayer3Scroll(0, 0)

/* Design Room screen setup, called once from DesignRoom_Init. Main screen:
 * mode 0, BG0-BG2 and OBJ on, BG3 and the windows off. Both screens: layer
 * priorities 0 to 3, no blending, no scroll. Then loads the graphics
 * (Ov006_LoadMapeditGraphics). */
void Ov006_SetUpScreens(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Screen_SetBgMode(0);
  Screen_InitLayers(data_021575c8);
  data_027c0280->dispCnt |= 0x100;
  data_027c0280->dispCnt |= 0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt &= ~0x800;
  data_027c0280->dispCnt |= 0x1000;
  data_027c0280->dispCnt &= ~0x2000;
  data_027c0280->dispCnt &= ~0x4000;
  data_027c0280->dispCnt &= ~0x8000;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  saved027c = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  RESET_LAYERS();
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  RESET_LAYERS();
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  Ov006_LoadMapeditGraphics();
}

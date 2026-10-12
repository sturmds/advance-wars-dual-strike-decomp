#include "types.h"
#include "sdk/gx.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

void SVC_WaitVBlankIntr(void);
void GXS_SetGraphicsMode(int bgMode);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetBgMode(int mode);
void Screen_SetBg0Mode(int mode);

/* DISPCNT bit 16: display mode 1, BG and OBJ graphics. */
#define DISPCNT_MODE_GRAPHICS 0x10000

/* The state copy of the hardware setup: graphics mode, BG mode 0, black. */
#define INIT_SCREEN_STATE(screen)        \
  savedGfx = data_027c0280;              \
  Screen_SetCurrentGfx(screen);          \
  savedState = data_027c027c;            \
  Screen_SetCurrent(screen);             \
  data_027c0280->dispMode = 1;           \
  Screen_SetBgMode(0);                   \
  Screen_SetBg0Mode(0);                  \
  data_027c0280->masterBrightness = -16; \
  Screen_SetCurrent(savedState);         \
  Screen_SetCurrentGfx(savedGfx)

/* Called once from main: turns both display engines on in graphics mode,
 * BG mode 0, fully black, and sets each screen's state to match. */
void Screens_InitDisplay(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  SVC_WaitVBlankIntr();
  GX_DispOn();
  *REG_DB_DISPCNT |= DISPCNT_MODE_GRAPHICS; /* engine B display on, inlined */
  GX_SetGraphicsMode(1, 0, 0);
  GXx_SetMasterBrightness_(REG_MASTER_BRIGHT, -16);
  GXS_SetGraphicsMode(0);
  GXx_SetMasterBrightness_(IO_SUB_SCREEN_FADE, -16);

  INIT_SCREEN_STATE(data_027c0288.unk_00);
  INIT_SCREEN_STATE(data_027c0288.unk_04);
}

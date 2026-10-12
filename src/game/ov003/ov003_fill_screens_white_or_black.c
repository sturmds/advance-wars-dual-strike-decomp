#include "types.h"
#include "game/screen_other_map.h"
#include "game/ov003_unit.h"
#include "game/Unk_02160abc.h"

void Screen_SetCurrentBoth(void* state);

#define STATE data_ov003_02364928
#define GFX data_027c0280

#define SET_BLEND(mode, eva, evb, evy) \
  do {                                 \
    GFX->blendMode = (mode);           \
    GFX->blendEva = (eva);             \
    GFX->blendEvb = (evb);             \
    GFX->blendEvy = (evy);             \
  } while (0)

/* Turns every layer of the current screen full white when unk_0a of
 * data_02160abc equals the current slot's row, else full black. */
#define FILL_WHITE_OR_BLACK()                                            \
  do {                                                                   \
    if (data_02160abc->unk_0a == STATE->slots[STATE->currentSlot].row) { \
      SET_BLEND(BLEND_MODE_BRIGHTEN, 0, 0, 16);                          \
    } else {                                                             \
      SET_BLEND(BLEND_MODE_DARKEN, 0, 0, 16);                            \
    }                                                                    \
    GFX->blendTopBg[GFX->layer0Bg] = 1;                                  \
    GFX->blendTopBg[GFX->layer1Bg] = 1;                                  \
    GFX->blendTopBg[GFX->layer2Bg] = 1;                                  \
    GFX->blendTopBg[GFX->layer3Bg] = 1;                                  \
    GFX->blendTopObj = 1;                                                \
    GFX->blendTopBackdrop = 1;                                           \
    GFX->blendBottomBg[GFX->layer0Bg] = 0;                               \
    GFX->blendBottomBg[GFX->layer1Bg] = 0;                               \
    GFX->blendBottomBg[GFX->layer2Bg] = 0;                               \
    GFX->blendBottomBg[GFX->layer3Bg] = 0;                               \
    GFX->blendBottomObj = 0;                                             \
    GFX->masterBrightness = 0;                                           \
  } while (0)

/* Turns both screens full white or full black with the colour effect
 * registers. No direct callers. */
void Ov003_FillScreensWhiteOrBlack(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;

  savedGfx = data_027c0280;
  savedScreen = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  FILL_WHITE_OR_BLACK();
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  FILL_WHITE_OR_BLACK();
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
}

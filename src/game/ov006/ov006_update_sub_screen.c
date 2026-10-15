#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/screen_other_map.h"
#include "sdk/fx.h"

void Ov006_QueueScreen1Sprites(void);
int Ov006_IsMapPlayable(void);
BOOL SpriteFile_Queue(void* sprites, int frame, int a, int x, int y, int attr,
                      int b, int c);
void Oam_PutSmallNumber(u16 x, u16 y, int value);
void Screen_SetLayer3Scroll(int x, int y);
void Screen_SetLayer0Scroll(int x, int y);
void DesignMap_DrawTemplatePreview(int a, int b, int c, int d);
void Ov006_DrawPropertyCounts(void);

#define OV6 data_ov006_0236d640

/* Per-frame sub screen update of the Design Room (no direct callers).
 * While Ov006_IsMapPlayable, a sprite slides in from the left, easing
 * toward x = 18 and stopping at 18; otherwise it waits off screen at -40.
 * Draws the cursor tile x and y (cursorX, cursorY, from 1), the units
 * left for the current army (50 minus its count) and the properties left
 * (60 minus propertyCount), moves the layer 3 y scroll by half a pixel per frame,
 * then draws the count table. Flag 4 of unk_00, once: calls DesignMap_DrawTemplatePreview
 * and sets the sub screen layer 0 scroll to (0x1fa, 208). */
void Ov006_UpdateSubScreen(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  Unk_027c0280* mainSaved0280;
  Unk_02080440* mainSaved027c;
  Unk_027c0280* subSaved0280;

  Ov006_QueueScreen1Sprites();

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  if (Ov006_IsMapPlayable() != 0) {
    OV6->unk_58 += FX_Div(0x20012 - OV6->unk_58, 3 * FX32_ONE);
    if (OV6->unk_58 > 0x12000) {
      OV6->unk_58 = 0x12000;
    }
    SpriteFile_Queue(OV6->unk_70, 0, 0, (OV6->unk_58 >> 12) & 0x1ff, 4, 0xf2b0,
                     0, 0);
  } else {
    OV6->unk_58 = -0x28000;
  }
  SpriteFile_Queue(OV6->unk_70, 2, 0, 10, 145, 0xf2b0, 0, 0);
  Oam_PutSmallNumber(18, 153, OV6->cursorX + 1);
  SpriteFile_Queue(OV6->unk_70, 1, 0, 10, 167, 0xf2b0, 0, 0);
  Oam_PutSmallNumber(18, 175, OV6->cursorY + 1);
  SpriteFile_Queue(OV6->unk_70, 3, 0, 26, 138, 0xf2b0, 0, 0);
  Oam_PutSmallNumber(58, 137, 50 - (s8)OV6->unk_2c[OV6->unitArmy - 1]);
  SpriteFile_Queue(OV6->unk_70, 3, 0, 70, 138, 0xf2b0, 0, 0);
  Oam_PutSmallNumber(102, 137, 60 - OV6->propertyCount);
  Screen_SetLayer3Scroll(0, (OV6->unk_54 >> 12) & 0xff);
  /* the original adds a double: _dflt, _dadd, _dfix */
  OV6->unk_54 += 2048.0;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  Ov006_DrawPropertyCounts();

  if (OV6->unk_00 & 4) {
    OV6->unk_00 &= ~4;
    mainSaved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    mainSaved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    subSaved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    DesignMap_DrawTemplatePreview(0, 0, 68, 7);
    Screen_SetLayer0Scroll(0x1fa, 208);
    Screen_SetCurrentGfx(subSaved0280);
    Screen_SetCurrent(mainSaved027c);
    Screen_SetCurrentGfx(mainSaved0280);
  }
}

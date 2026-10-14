#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/unit_info_panel.h"
#include "game/unit_stats.h"
#include "game/text_print.h"

extern u32 data_ov000_022f4538[];
extern u32 data_ov000_022f4618[];
extern u32 data_ov000_022f4658[];
extern u32 data_ov000_022f4698[];
extern u32 data_ov000_022f5b1c[];
extern u32 data_ov000_022f5b38[];
extern u32 data_ov000_022f5b54[];
extern u32 data_ov000_022f5b70[];
extern u32 data_ov000_022f5b8c[];
extern u32 data_ov000_022f5ba8[];
extern u32 data_ov000_022f45f8[];
extern u8 data_ov000_022f4638[];
extern u32 data_ov000_022f4678[];

UnitStats Ov000_gUnitStats[UNIT_TYPE_COUNT] = {
#include "data/ov000/Ov000_gUnitStats.inc.c"
};

extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

/* Four bytes per panel page, passed on to Sprite_DrawPulsingCorners. */
extern UnitInfoPanelMarker data_ov000_0234f9e8[];

const void* Text_Get(u32 textId);
void Screen_SetCurrentGfx(void* state);
void Sprite_DrawPulsingCorners(int a, int b, int c, int d, int e, int f);

/* Draws the current page's marker on the second screen. Pages 4 and 11 pass
 * the pixel width of the unit's second and third description text instead of
 * the table's third byte. */
void UnitInfoPanel_DrawPageMarker(void) {
  int page = data_ov000_0234ec8c->page;
  int base = data_ov000_0234f9e8[page].x;
  int x = data_ov000_0234f9e8[page].y;
  int value = data_ov000_0234f9e8[page].width;
  int d = data_ov000_0234f9e8[page].height;
  void* savedGfx;

  switch (page) {
    case 4:
      value = Text_GetPrintWidth(Text_Get(
          Ov000_gUnitStats[data_ov000_0234ec8c->unit->type].text[1].id));
      break;
    case 11:
      value = Text_GetPrintWidth(Text_Get(
          Ov000_gUnitStats[data_ov000_0234ec8c->unit->type].text[2].id));
      break;
  }
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  Sprite_DrawPulsingCorners(data_ov000_0234ec8c->originX + base, x, value, d,
                            952, 15);
  Screen_SetCurrentGfx(savedGfx);
}

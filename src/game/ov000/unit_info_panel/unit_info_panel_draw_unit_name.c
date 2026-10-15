#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/text_print.h"

const void* Text_Get(u32 textId);
void Font_SetDigitEndTile(int tile);
void Text_PrintImmediate(s16 x, int y, void* target, u32 text, int a, int b);

/* Prints a unit's name, centred in a 7 tile wide area, and its two description
 * lines below it, in the area starting at pixel x. Text_GetPrintWidth gives a
 * text's width in pixels. */
void UnitInfoPanel_DrawUnitName(u32 x, MapUnit* unit) {
  /* needed to match: without the first assignment the compiler moves the
     * sum below the Font_SetDigitEndTile call */
  int indent = 0;

  indent = (7 - Text_GetPrintWidth(
                    Text_Get(Ov000_gUnitStats[unit->type].text[0].id)) /
                    8) /
           2;
  Font_SetDigitEndTile(0x162);
  Text_PrintImmediate((x >> 3) + indent, 1, data_027c0280->layer0Map,
                      Ov000_gUnitStats[unit->type].text[0].id, 0, 0);
  Font_SetDigitEndTile(0x170);
  if (Ov000_gUnitStats[unit->type].text[1].id != 0) {
    Text_PrintImmediate((int)(x + 8) / 8, 11, data_027c0280->layer0Map,
                        Ov000_gUnitStats[unit->type].text[1].id, 0, 0);
  }
  Font_SetDigitEndTile(0x180);
  if (Ov000_gUnitStats[unit->type].text[2].id != 0) {
    Text_PrintImmediate((int)(x + 8) / 8, 17, data_027c0280->layer0Map,
                        Ov000_gUnitStats[unit->type].text[2].id, 0, 0);
  }
}

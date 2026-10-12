#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/co_data.h"
#include "game/icon.h"
#include "game/unit_info_panel.h"
#include "game/unit_stats.h"

void Screen_SetCurrentGfx(void* state);
void Oam_PutSprite(int x, int y, void* shape, int tileAndPalette);
void Oam_PutSmallNumberFromTile0(u16 x, int y, u32 value);
void UnitInfoPanel_DrawIconList(void);

extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern void* data_ov000_0234f384[][15];
extern u8 data_ov000_022f5cc8[];
extern void* data_ov000_0234eef0;

/* Draws the unit in the panel on the second screen: the frame sprite for the
 * owner's army style, the unit type's icon and two more icons, then fuel and
 * the type's maximum fuel as numbers (one digit shifts 4 pixels left to line
 * up). The numbers are skipped for Oozium (UNIT_CLASS_ALL), which has no fuel
 * display. Everything is drawn relative to the panel's originX. The shifts
 * and the 37 and 6 icons are guesses. */
void UnitInfoPanel_DrawUnit(void) {
  MapUnit* unit = data_ov000_0234ec8c->unit;
  Unk_027c0280* savedGfx = data_027c0280;
  int shift;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  Oam_PutSprite(data_ov000_0234ec8c->originX + 48, 87,
                data_ov000_0234f384[unit->type][data_ov000_0234ec8c->style],
                (data_ov000_0234ec8c->buffer == 0 ? 0x214 : 0x254) | 0x3000);
  Icon_Draw(data_ov000_022f5cc8[unit->type], data_ov000_0234ec8c->originX + 74,
            40, 0, 0);
  Icon_Draw(37, data_ov000_0234ec8c->originX + 74, 56, 0, 0);
  Icon_Draw(6, data_ov000_0234ec8c->originX + 84, 72, 0, 0);
  Screen_SetCurrentGfx(savedGfx);
  if (Ov000_gUnitStats[unit->type].unitClass == UNIT_CLASS_ALL) {
    return;
  }
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  shift = unit->fuel < 10 ? -4 : 0;
  Oam_PutSmallNumberFromTile0(shift + data_ov000_0234ec8c->originX + 104, 72,
                              unit->fuel);
  shift = Ov000_gUnitStats[unit->type].fuel < 10 ? -4 : 0;
  Oam_PutSmallNumberFromTile0(shift + data_ov000_0234ec8c->originX + 116, 80,
                              Ov000_gUnitStats[unit->type].fuel);
  Oam_PutSprite(data_ov000_0234ec8c->originX + 107, 76, &data_ov000_0234eef0,
                0xf36a);
  UnitInfoPanel_DrawIconList();
  Screen_SetCurrentGfx(savedGfx);
}

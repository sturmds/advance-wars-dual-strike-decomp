#include "types.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Number_Draw(s16 x, s16 y, u16* map, u32 value, u16 entryBits,
                 u16 digitSet);
void Text_PrintImmediate(s16 x, s16 y, u16* map, u32 textId, u16 a, u16 b);
void Text_PrintStringImmediate(s16 x, s16 y, u16* map, const char* text, u16 a,
                               u16 b);
void UnitIcon_Draw(u16* map, u16 x, u16 y, u8 unitType, u16 a, u8 b,
                   u8 c); /* unit icon */
int UnitList_GetCargoBranch(
    int slot); /* which cargo tree piece goes before the unit: 0 none, 1-6 */
s32 HwDiv32(s32 a, s32 b);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern u8*
    gUnitListRows; /* unit list rows: slot of each unit, counted from the player's first slot; 0 empty */
/* Font glyphs that draw the cargo tree in front of a carried unit. */
extern char data_02167c74[];
extern char data_02167c78[];
extern char data_02167c7c[];
extern char data_02167c84[];
extern char data_02167c88[]; /* "/" between a value and its maximum */

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Text ids printed in the ammo column of a unit type without ammo. */
#define TEXT_NO_AMMO_UNK_17 0xc00000a8 /* UnitStats.unk_17 set */
#define TEXT_NO_AMMO 0xc00000ac

/* Draws one row of the unit list into layer 0 at tile row y + 8: the row
 * number, the cargo tree piece for a carried unit, the unit icon (flag
 * 0x100 once the unit has waited), its name, shown HP, fuel / max fuel and ammo /
 * max ammo. Carried units use text style 1. */
void UnitList_DrawRow(int y, int index) {
  int textStyle;
  int iconFlags;
  MapUnit* unit;
  int branchX;
  int iconX;
  const char* branch;
  int shownHp;

  if (gUnitListRows[index] == 0) {
    return;
  }
  unit = &GAME->units[gUnitListRows[index] + GAME->currentUnitBase];
  if (unit->flags & MAP_UNIT_CARRIED) {
    textStyle = 1;
  } else {
    textStyle = 0;
  }
  if (unit->flags & MAP_UNIT_WAITED) {
    iconFlags = 0x100;
  } else {
    iconFlags = GAME->currentUnitBase; /* as read, like BuildMenu_DrawColumn */
  }
  branchX = 0;
  switch (UnitList_GetCargoBranch(gUnitListRows[index])) {
    case 1:
      branch = data_02167c74;
      iconX = 1;
      break;
    case 2:
      branch = data_02167c78;
      iconX = 1;
      break;
    case 3:
      branch = data_02167c84;
      iconX = 2;
      break;
    case 4:
      branch = data_02167c7c;
      iconX = 2;
      break;
    case 5:
      branch = data_02167c74;
      iconX = 2;
      branchX = 1;
      break;
    case 6:
      branch = data_02167c78;
      iconX = 2;
      branchX = 1;
      break;
    default:
      branch = NULL;
      iconX = 0;
      break;
  }
  Number_Draw(3, y + 8, data_027c0280->layer0Map, index + 1, 0, 0);
  if (branch != NULL) {
    Text_PrintStringImmediate(branchX + 5, y + 8, data_027c0280->layer0Map,
                              branch, 0, textStyle);
  }
  UnitIcon_Draw(data_027c0280->layer0Map, iconX + 5, y + 8, unit->type,
                iconFlags, unit->cargo[0], unit->captureProgress);
  Text_PrintImmediate(9, y + 8, data_027c0280->layer0Map,
                      Ov000_gUnitStats[unit->type].text[0].id, 0, textStyle);
  if (unit->hp == 0) {
    shownHp = 0;
  } else {
    shownHp = HwDiv32(unit->hp - 1, 10) + 1;
  }
  Number_Draw(18, y + 8, data_027c0280->layer0Map, shownHp, 0, textStyle);
  Number_Draw(21, y + 8, data_027c0280->layer0Map, unit->fuel, 0, textStyle);
  Text_PrintStringImmediate(22, y + 8, data_027c0280->layer0Map, data_02167c88,
                            0, textStyle);
  Number_Draw(24, y + 8, data_027c0280->layer0Map,
              Ov000_gUnitStats[unit->type].fuel, 0, textStyle);
  if (Ov000_gUnitStats[unit->type].ammo == 0) {
    if (Ov000_gUnitStats[unit->type].unk_17 != 0) {
      Text_PrintImmediate(27, y + 8, data_027c0280->layer0Map,
                          TEXT_NO_AMMO_UNK_17, 0, textStyle);
    } else {
      Text_PrintImmediate(27, y + 8, data_027c0280->layer0Map, TEXT_NO_AMMO, 0,
                          textStyle);
    }
    return;
  }
  Number_Draw(27, y + 8, data_027c0280->layer0Map, unit->ammo, 0, textStyle);
  Text_PrintStringImmediate(28, y + 8, data_027c0280->layer0Map, data_02167c88,
                            0, textStyle);
  Number_Draw(29, y + 8, data_027c0280->layer0Map,
              Ov000_gUnitStats[unit->type].ammo, 0, textStyle);
}

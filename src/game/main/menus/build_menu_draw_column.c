#include "types.h"
#include "game/build_menu.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

const void* Text_Get(u32 textId);
void Text_PrintStringImmediate(s16 x, s16 y, u16* map, const char* text, u16 a,
                               u16 b);
void Number_Draw(s16 x, s16 y, u16* map, int value, u16 a, u16 b);
int Unit_GetValuePerHp(int playerIndex, int unitType);
void UnitIcon_Draw(u16* map, u16 x, u16 y, int unitType, int a, int b, int c);
void Gfx_RequestLayerCopy(int layer);

/* Draws up to BUILD_MENU_ROWS build menu entries from gBuildMenuEntries[first]
 * on layer 0, two tile rows apart below (x, y): unit icon, name and cost.
 * Entries the player cannot afford are drawn dimmed. */
void BuildMenu_DrawColumn(int x, int y, int first) {
  u16 row;
  u16 dimmed;
  int iconArg;
  const u8* name;
  int i;
  u8 buf[20];
  int rowY;
  int cost;

  for (row = 0; row < BUILD_MENU_ROWS; row++) {
    if (gBuildMenuEntries[first + row].unitType == 0) {
      break;
    }
    if (gBuildMenuEntries[first + row].state == BUILD_ENTRY_AFFORDABLE) {
      dimmed = 0;
      iconArg =
          MAP_STATE
              ->currentUnitBase; /* as read; why the icon takes this is unclear */
    } else {
      dimmed = 1;
      iconArg = 0x100;
    }
    name = Text_Get(
        Ov000_gUnitStats[gBuildMenuEntries[first + row].unitType].text[0].id);
    buf[0] = 25; /* control byte before the name; meaning unknown */
    i = 0;
    while (name[i] != 0) {
      buf[i + 1] = name[i];
      i++;
    }
    buf[i + 1] = 0;
    rowY = y + 1 + row * 2;
    Text_PrintStringImmediate(x + 3, rowY, data_027c0280->layer0Map,
                              (const char*)buf, 0, dimmed);
    cost = Unit_GetValuePerHp(MAP_STATE->currentPlayer,
                              gBuildMenuEntries[first + row].unitType) *
           10;
    Number_Draw(x + 14, rowY, data_027c0280->layer0Map, cost, 0, dimmed);
    UnitIcon_Draw(data_027c0280->layer0Map, x + 1, rowY,
                  gBuildMenuEntries[first + row].unitType, iconArg, 0, 0);
  }
  Gfx_RequestLayerCopy(0);
}

#include "types.h"
#include "game/screen_other_map.h"
#include "game/unit_info_panel.h"

/* also declared in src/game/ov000/unit_info_panel/unit_info_panel_open_for_type.c */
MapUnit data_ov000_02350510;
u8 data_ov000_02350520[0x10]; /* size: up to the next symbol */

u8 Player_GetArmyStyle(int playerIndex);
void Unit_Init(MapUnit* unit, int type);
void func_01ff9ac4(void);
void UnitInfoPanel_PutFrameBlock(void);
void UnitInfoPanel_DrawContents(void);

/* Like UnitInfoPanel_SetUnit, for a unit type instead of a unit on the map:
 * Unit_Init builds a fresh unit of that type in data_ov000_02350510 and the
 * panel shows it. The panel is drawn on the other screen. owner gives the
 * palette (Player.armyColour) and sprite set (army style). */
void UnitInfoPanel_SetUnitType(int owner, int unitType) {
  Unk_027c0280* saved0280;

  Unit_Init(&data_ov000_02350510, unitType);
  data_ov000_0234ec8c->owner = owner;
  data_ov000_0234ec8c->unit = &data_ov000_02350510;
  data_ov000_0234ec8c->palette = data_027c027c->unk_48[data_027c0284]
                                     ->players[data_ov000_0234ec8c->owner]
                                     .armyColour -
                                 1;
  data_ov000_0234ec8c->style =
      Player_GetArmyStyle(data_ov000_0234ec8c->owner) - 1;
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  func_01ff9ac4();
  UnitInfoPanel_PutFrameBlock();
  UnitInfoPanel_DrawContents();
  Screen_SetCurrentGfx(saved0280);
}

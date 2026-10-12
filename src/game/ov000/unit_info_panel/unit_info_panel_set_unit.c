#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"
#include "game/unit_info_panel.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

extern ScriptCommand data_ov000_0234f30c[];

s8 ScriptSlot_Find(const ScriptCommand* script);
u8 Player_GetArmyStyle(int playerIndex);
void UnitInfoPanel_Redraw(ScriptSlot* slot);

/* Points the panel (already open, with its script running) at another unit and
 * redraws it. palette reads Player.armyColour (the palette); style reads the
 * army style (the sprite set) of the unit's owner. */
void UnitInfoPanel_SetUnit(int owner, MapUnit* unit) {
  s16 slot = ScriptSlot_Find(data_ov000_0234f30c);

  data_ov000_0234ec8c->owner = owner;
  data_ov000_0234ec8c->unit = unit;
  data_ov000_0234ec8c->palette = data_027c027c->unk_48[data_027c0284]
                                     ->players[data_ov000_0234ec8c->owner]
                                     .armyColour -
                                 1;
  data_ov000_0234ec8c->style =
      Player_GetArmyStyle(data_ov000_0234ec8c->owner) - 1;
  UnitInfoPanel_Redraw(&data_027c027c->scriptSlots[slot]);
}

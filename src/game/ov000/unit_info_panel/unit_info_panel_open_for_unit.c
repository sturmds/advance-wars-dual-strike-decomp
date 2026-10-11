#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"
#include "game/unit_info_panel.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

extern ScriptCommand data_ov000_0234f30c[];

int SceneMode_IsCurrent(int a0);
s8 ScriptSlot_Find(const ScriptCommand *script);
u8 Player_GetArmyStyle(int playerIndex);
ScriptSlot *ScriptSlot_Start(const ScriptCommand *script, int arg);
void UnitInfoPanel_SetUnit(int owner, MapUnit *unit);

/* Opens the unit info panel for a unit on the map, or switches an open panel
 * to it. The owner comes from the unit's slot in the unit array; it gives the
 * palette (Player.armyColour) and sprite set (army style). Does nothing
 * unless SceneMode_IsCurrent(5) is true, or when the panel already shows the unit. */
void UnitInfoPanel_OpenForUnit(MapUnit *unit)
{
    BOOL running;
    int owner = MAP_UNIT_OWNER(data_027c027c->unk_48[data_027c0284], unit);

    if (SceneMode_IsCurrent(5) == 0) {
        return;
    }
    if (data_ov000_0234ec8c->unit == unit) {
        return;
    }
    running = ScriptSlot_Find(data_ov000_0234f30c) != -1;
    if (running) {
        UnitInfoPanel_SetUnit(owner, unit);
        return;
    }
    ScriptSlot_Start(data_ov000_0234f30c, 1);
    data_ov000_0234ec8c->buffer = 0;
    data_ov000_0234ec8c->originX = 120;
    data_ov000_0234ec8c->owner = owner;
    data_ov000_0234ec8c->unit = unit;
    data_ov000_0234ec8c->palette = data_027c027c->unk_48[data_027c0284]->players[data_ov000_0234ec8c->owner].armyColour - 1;
    data_ov000_0234ec8c->style = Player_GetArmyStyle(data_ov000_0234ec8c->owner) - 1;
}

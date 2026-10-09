#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"
#include "game/unit_info_panel.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

extern ScriptCommand data_ov000_0234f30c[];

int func_020f4638(int a0);
s8 func_02017130(const ScriptCommand *script);
u8 Player_GetArmyStyle(int playerIndex);
ScriptSlot *ScriptSlot_Start(const ScriptCommand *script, int arg);
void UnitInfoPanel_SetUnit(int owner, MapUnit *unit);

void func_ov000_022b1f90(MapUnit *unit)
{
    BOOL running;
    int owner = MAP_UNIT_OWNER(data_027c027c->unk_48[data_027c0284], unit);

    if (func_020f4638(5) == 0) {
        return;
    }
    if (data_ov000_0234ec8c->unit == unit) {
        return;
    }
    running = func_02017130(data_ov000_0234f30c) != -1;
    if (running) {
        UnitInfoPanel_SetUnit(owner, unit);
        return;
    }
    ScriptSlot_Start(data_ov000_0234f30c, 1);
    data_ov000_0234ec8c->buffer = 0;
    data_ov000_0234ec8c->unk_00 = 120;
    data_ov000_0234ec8c->owner = owner;
    data_ov000_0234ec8c->unit = unit;
    data_ov000_0234ec8c->palette = data_027c027c->unk_48[data_027c0284]->players[data_ov000_0234ec8c->owner].armyColour - 1;
    data_ov000_0234ec8c->style = Player_GetArmyStyle(data_ov000_0234ec8c->owner) - 1;
}

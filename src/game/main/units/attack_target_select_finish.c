#include "types.h"
#include "game/Unk_02080440.h"
#include "game/event_script.h"
#include "game/play_records.h"
#include "game/task.h"
#include "game/unit_stats.h"
#include "game/unit_target_entry.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern const EventScriptCommand gMapScrollScript[];
extern int data_022a6dd4;

void MenuDepth_Pop(void);
void Task_ResumeScript(Task *task);
void UnitMenu_Open(void);
UnitTargetEntry *func_020dc524(int index);
void func_020dea40(int defenderSlot);
void func_020de9f0(int x, int y);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Task step, likely run when the attack target choice ends; does nothing
 * while gMapScrollScript is running. A negative data_022a6dd4 means the player backed out: the menu
 * and task go away and UnitMenu_Open runs. Otherwise data_022a6dd4 picks the
 * entry of the target list: the acting unit attacks the unit in it, or the
 * object at its tile when unk_02 is set. A human player's attack is counted
 * in gPlayRecords as direct or indirect by the unit's minRange. */
void AttackTargetSelect_Finish(Task *task)
{
    UnitTargetEntry *target;

    if (ScriptSlot_Find(gMapScrollScript) != -1) {
        return;
    }
    if (data_022a6dd4 < 0) {
        MenuDepth_Pop();
        Task_Destroy(task);
        UnitMenu_Open();
        return;
    }
    target = func_020dc524(data_022a6dd4);
    if (target->unk_02 == 0) {
        func_020dea40(target->unitSlot & 0xff);
    } else {
        /* needed to match: the original reads the tile as s16 */
        func_020de9f0(target->unk_04, target->unk_06);
    }
    Task_ResumeScript(task);
    if (GAME->players[GAME->currentPlayer].controller != PLAYER_CONTROLLER_HUMAN) {
        return;
    }
    switch (Ov000_gUnitStats[GAME->actingUnit->type].minRange) {
    case ATTACK_NONE:
        return;
    case ATTACK_DIRECT:
        if (gPlayRecords.directAttacks < 999999) {
            gPlayRecords.directAttacks++;
        }
        return;
    default:
        if (gPlayRecords.indirectAttacks < 999999) {
            gPlayRecords.indirectAttacks++;
        }
        return;
    }
}

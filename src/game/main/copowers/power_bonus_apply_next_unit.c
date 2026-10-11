#include "types.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/task.h"
#include "game/script_slot.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern ScriptCommand gMapScrollScript[];

void Task_ResumeScript(Task *task);
s16 ScriptSlot_Find(const ScriptCommand *script);
void func_020f5888(s16 x, s16 y);           /* starts the map scroll to the tile */
void PowerEffect_Start(int x, int y, int block); /* tile effect sprite */
void func_020c4018(void);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define FRONT (STATE->players[STATE->currentPlayer].front)
#define BLOCK (gCoTable[FRONT.id & PLAYER_CO_ID_MASK].blocks[FRONT.unk_12])

/* Script step of the common power script (gCoNoPowerScript): applies the
 * power's unitBonus to the next unit of the current player that its
 * affectsUnit filter accepts, one unit every 5 frames once the map scroll is
 * done. task->unk_64 counts frames, task->unk_68 is the unit index within
 * the player's slots (up to 50). A unit on a shown tile gets the scroll to it
 * and the tile effect; a carried unit gets neither; a unit on a hidden tile
 * gets only the scroll. Resumes the script after the last unit. */
void PowerBonus_ApplyNextUnit(Task *task)
{
    MapUnit *unit;

    task->unk_64++;
    if (ScriptSlot_Find(gMapScrollScript) != -1) {
        return;
    }
    if (task->unk_64 > 4) { /* needed to match: if/else, not an early return */
        task->unk_64 = 0;
    } else {
        return;
    }
    for (; task->unk_68 < 51; task->unk_68++) {
        unit = &STATE->units[STATE->currentUnitBase + task->unk_68];
        if (unit->type == 0) {
            continue;
        }
        if (!BLOCK.affectsUnit(unit)) {
            continue;
        }
        if (STATE->mapTiles->unk_2338[unit->x + STATE->mapTiles->rowStart[unit->y]] == 0 || (unit->flags & MAP_UNIT_CARRIED)) {
            if (!(unit->flags & MAP_UNIT_CARRIED)) {
                func_020f5888(unit->x, unit->y);
                if (ScriptSlot_Find(gMapScrollScript) != -1) {
                    return;
                }
            }
            BLOCK.unitBonus(unit);
            task->unk_68++;
            break;
        }
        func_020f5888(unit->x, unit->y);
        if (ScriptSlot_Find(gMapScrollScript) != -1) {
            return;
        }
        BLOCK.unitBonus(unit);
        PowerEffect_Start(unit->x, unit->y, FRONT.unk_12);
        task->unk_68++;
        break;
    }
    if (task->unk_68 == 51) {
        Task_ResumeScript(task);
    }
    func_020c4018();
}

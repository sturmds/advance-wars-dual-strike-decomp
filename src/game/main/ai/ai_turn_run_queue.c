#include "types.h"
#include "game/ai_unit_action.h"
#include "game/ai_profile.h"
#include "game/unit_action.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern u32 data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

extern u8 data_0229ea0c[];

int Rng_Next(void);
s32 HwMod32(s32 numer, s32 denom);
int func_0209f8d0(MapUnit *unit);
int func_0212969c(void *p);

/* Runs gAiQueueRoutine for the next queue entry. A unit entry becomes
 * the active unit and is lifted off the unit map while the routine runs;
 * waited units are skipped. Entry 0 ends the queue and returns to the
 * phase step. If the routine made the unit act, the turn moves on to
 * show the action. */
void AiTurn_RunQueue(void)
{
    u8 entry = *gAiQueueCursor;
    MapUnit *unit;
    MapTiles *map;
    Unk_02080440_48 *game;

    if (entry == 0) {
        AI_TURN->state = AI_STATE_RUN_PHASE;
        return;
    }
    if (entry != AI_QUEUE_RUN_ONCE) {
        GAME->actingUnitIndex = entry;
        GAME->actingUnit = &GAME->units[GAME->actingUnitIndex];
        data_0229e160 = &data_021619c4->units[GAME->actingUnit->type - 1];
        unit = GAME->actingUnit;
        if ((unit->flags & MAP_UNIT_WAITED) || unit->type == 0) {
            gAiQueueCursor++;
            return;
        }
        GAME->unk_3e = unit->x;
        GAME->unk_40 = GAME->actingUnit->y;
        AI_TURN->flags &= ~1;
        AI_TURN->flags &= ~2;
        AI_TURN->unk_08 = 0;
        data_0229e198.action = 0;
        data_0229e198.pending = 0;
        data_0229e198.target = 0;
        data_0229e198.unk_07 = 0;
        map = GAME->mapTiles;
        unit = GAME->actingUnit;
        map->unitAt[unit->x + map->rowStart[unit->y]] = 0;
        GAME->actingUnit->flags |= MAP_UNIT_LOCKED;
        AI_TURN->roll = HwMod32(Rng_Next(), 100);
        AI_TURN->unk_05 = func_0209f8d0(GAME->actingUnit);
    } else {
        data_0229e198.pending = 0;
    }

    if (func_0212969c(data_0229ea0c) == 0) {
        gAiQueueRoutine();
    }

    if (data_0229e198.pending == 0) {
        if (*gAiQueueCursor != AI_QUEUE_RUN_ONCE) {
            game = GAME;
            map = game->mapTiles;
            map->unitAt[game->actingUnit->x + map->rowStart[game->actingUnit->y]] = game->actingUnitIndex;
        }
        unit = GAME->actingUnit;
        if (unit != NULL) {
            unit->flags &= ~MAP_UNIT_LOCKED;
        }
        gAiQueueCursor++;
    } else {
        gAiQueueCursor++;
        AI_TURN->state = AI_STATE_SHOW_ACTION;
    }
}

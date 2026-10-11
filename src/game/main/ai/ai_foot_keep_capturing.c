#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

int Unit_CaptureTurnsLeft(MapUnit *unit);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);
void Ai_ClearActingUnitTarget(void);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* Last step of AiFoot_ContinueCapture: an Infantry or Mech that has started
 * capturing keeps capturing where it stands. */
void AiFoot_KeepCapturing(void)
{
    if (STATE->actingUnit->type > UNIT_MECH || STATE->actingUnit->captureProgress == 0) {
        return;
    }
    if (Unit_CaptureTurnsLeft(STATE->actingUnit) >= 2) {
        STATE->unk_6088[STATE->mapTiles->unk_1928[TILE(STATE->actingUnit->x, STATE->actingUnit->y)]].claimCount++;
    }
    Ai_ClearActingUnitTarget();
    AiUnit_PerformAction(STATE->actingUnit->x, STATE->actingUnit->y, AI_ACTION_CAPTURE, 0, 0);
}

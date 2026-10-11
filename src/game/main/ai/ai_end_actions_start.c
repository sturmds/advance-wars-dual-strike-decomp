#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_build_state.h"
#include "game/map_unit.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

int Unit_CanRepairAt(MapUnit *unit, int x, int y);
int AiSites_Collect(void);
u8 AiSites_BuildGroups(void);
int AiPlayer_FrontCoNearerToSuper(void);
int AiUnit_GetRepairValue(int player, BOOL flag, MapUnit *unit);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Step 0 of the end-of-turn phase: fills the AiBuildState counts, counts the
 * current player's units, and totals AiUnit_GetRepairValue over those that can be
 * repaired where they stand (times 1.5, at least 1000). Meaning of the total
 * unknown. */
void AiEndActions_Start(void)
{
    int i;
    MapUnit *unit;
    BOOL flag;

    data_021619c0->recordCount = AiSites_Collect();
    data_021619c0->slotCount = AiSites_BuildGroups();
    flag = AiPlayer_FrontCoNearerToSuper() == 1;
    data_021619c0->unk_07 = 0;
    data_021619c0->unk_0c = 0;
    for (i = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1; i < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; i++) {
        unit = &STATE->units[i];
        if (unit->type != 0) {
            data_021619c0->unk_07++;
            if (Unit_CanRepairAt(unit, unit->x, unit->y)) {
                data_021619c0->unk_0c += AiUnit_GetRepairValue(STATE->currentPlayer, flag, unit);
            }
        }
    }
    data_021619c0->unk_0c = data_021619c0->unk_0c * 3 >> 1;
    if (data_021619c0->unk_0c < 1000) {
        data_021619c0->unk_0c = 1000;
    }
    data_021619c0->savingTarget = 0;
    data_021619c0->savingSkips = 2;
}

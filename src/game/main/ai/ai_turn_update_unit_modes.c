#include "types.h"
#include "game/ai_profile.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void func_0208d3fc(MapUnit *unit);
void func_0208d41c(MapUnit *unit);
void func_0208d478(MapUnit *unit);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Sets the active unit profile for each of the current player's units,
 * then runs the check for its serviceWanted mode (1: func_0208d41c, 2:
 * func_0208d3fc, else func_0208d478), which may change the mode. */
void AiTurn_UpdateUnitModes(void)
{
    int slot;
    MapUnit *unit;

    for (slot = Ov000_gPlayerFirstUnitSlot[GAME->currentPlayer] + 1;
         slot < Ov000_gPlayerFirstUnitSlot[GAME->currentPlayer] + 51; slot++) {
        unit = &GAME->units[slot];
        if (unit->type == UNIT_NONE) {
            continue;
        }
        gAiActiveUnitProfile = &gAiProfile->units[unit->type - 1];
        switch (unit->serviceWanted) {
        case 1:
            func_0208d41c(unit);
            break;
        case 2:
            func_0208d3fc(unit);
            break;
        default:
            func_0208d478(unit);
            break;
        }
    }
}

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

BOOL func_0209ec00(MapUnit *unit, int x, int y);
void func_020af700(void);
void func_020afb58(void);
void func_020aff0c(void);
void func_020b0604(void);
void func_020b81ac(void);

/* AI step shared by AiFoot_MoveAndAttack and many queue routines. When the
 * acting unit's unk_0a_12 is 1 or 2, runs func_020af700 or func_020afb58;
 * infantry and mechs first get func_020b0604. */
void func_020af5f8(void)
{
    if (!func_0209ec00(GAME->actingUnit, GAME->actingUnit->x, GAME->actingUnit->y)) {
        func_020aff0c();
    }
    if (GAME->actingUnit->unk_0a_12 == 0) {
        return;
    }
    if (GAME->actingUnit->type <= UNIT_MECH) {
        func_020b0604();
    }
    gAiTurnState->flags |= 2;
    func_020b81ac();
    switch (GAME->actingUnit->unk_0a_12) {
    case 1:
        func_020af700();
        break;
    case 2:
        func_020afb58();
        break;
    }
}

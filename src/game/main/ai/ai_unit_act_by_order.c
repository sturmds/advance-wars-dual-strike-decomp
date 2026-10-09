#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

BOOL func_0209dd7c(void);
BOOL func_020c0e3c(int player);
void AiUnit_ActOrder10(void);
void AiUnit_ActOrder9(void);
void AiUnit_ActOrder8(void);
void AiUnit_ActOrder7(void);
void AiUnit_ActOrder6(void);
void AiUnit_ActOrder5(void);
void AiUnit_ActOrder4(void);
void func_020a0ac4(void);
void func_020a0b04(void);
void func_020a0b44(void);
void func_020a0b80(void);

/* Queue routine for the units AiAction_QueueDirectUnits, AiAction_QueueAllDirectUnits and
 * AiAction_QueueAllDirectUnitsFog collect: runs the action sequence for the active unit's
 * AI order. */
void AiUnit_ActByOrder(void)
{
    Unk_02080440_48 *game = data_027c027c->unk_48[data_027c0284];

    switch (game->actingUnit->aiOrder) {
    case 0:
        func_020a0b80();
        break;
    case 1:
        func_020a0b44();
        break;
    case 2:
        if (func_0209dd7c() == TRUE) {
            func_020a0b04();
        } else {
            func_020a0b80();
        }
        break;
    case 3:
        func_020a0ac4();
        break;
    case 4:
        AiUnit_ActOrder4();
        break;
    case 5:
        AiUnit_ActOrder5();
        break;
    case 6:
        if (func_020c0e3c(game->currentPlayer) == TRUE) {
            AiUnit_ActOrder6();
        } else {
            AiUnit_ActOrder5();
        }
        break;
    case 8:
        AiUnit_ActOrder8();
        break;
    case 9:
        AiUnit_ActOrder9();
        break;
    case 10:
        AiUnit_ActOrder10();
        break;
    /* needed to match: case 7 comes last */
    case 7:
        AiUnit_ActOrder7();
        break;
    }
}

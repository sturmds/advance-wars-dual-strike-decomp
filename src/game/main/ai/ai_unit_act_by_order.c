#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

BOOL func_0209dd7c(void);
BOOL func_020c0e3c(int player);
void func_020a0924(void);
void func_020a0954(void);
void func_020a0988(void);
void func_020a09bc(void);
void func_020a0a0c(void);
void func_020a0a44(void);
void func_020a0a84(void);
void func_020a0ac4(void);
void func_020a0b04(void);
void func_020a0b44(void);
void func_020a0b80(void);

/* Queue routine for the units AiAction_QueueDirectUnits, AiAction_QueueAllDirectUnits and
 * func_0208f060 collect: runs the action sequence for the active unit's
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
        func_020a0a84();
        break;
    case 5:
        func_020a0a44();
        break;
    case 6:
        if (func_020c0e3c(game->currentPlayer) == TRUE) {
            func_020a0a0c();
        } else {
            func_020a0a44();
        }
        break;
    case 8:
        func_020a0988();
        break;
    case 9:
        func_020a0954();
        break;
    case 10:
        func_020a0924();
        break;
    /* needed to match: case 7 comes last */
    case 7:
        func_020a09bc();
        break;
    }
}

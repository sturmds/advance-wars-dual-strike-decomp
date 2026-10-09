#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

BOOL func_0209dd7c(void);
BOOL func_020c0e3c(int player);
void AiIndirect_MoveOrder10(void);
void func_020a0428(void);
void func_020a0498(void);
void func_020a0508(void);
void func_020a0590(void);
void func_020a0604(void);
void func_020a0680(void);
void func_020a06fc(void);
void func_020a0778(void);
void func_020a07f4(void);
void func_020a086c(void);

/* Queue routine for indirect units, set by AiAction_QueueIndirectMoves:
 * runs the move sequence for the acting unit's AI order. Same shape as
 * AiUnit_ActByOrder for direct units. */
void AiIndirect_MoveByOrder(void)
{
    Unk_02080440_48 *game = data_027c027c->unk_48[data_027c0284];

    switch (game->actingUnit->aiOrder) {
    case 0:
        func_020a086c();
        break;
    case 1:
        func_020a07f4();
        break;
    case 2:
        if (func_0209dd7c() == TRUE) {
            func_020a0778();
        } else {
            func_020a086c();
        }
        break;
    case 3:
        func_020a06fc();
        break;
    case 4:
        func_020a0680();
        break;
    case 5:
        func_020a0604();
        break;
    case 6:
        if (func_020c0e3c(game->currentPlayer) == TRUE) {
            func_020a0590();
        } else {
            func_020a0604();
        }
        break;
    case 7:
        func_020a0508();
        break;
    case 8:
        func_020a0498();
        break;
    case 9:
        func_020a0428();
        break;
    case 10:
        AiIndirect_MoveOrder10();
        break;
    }
}

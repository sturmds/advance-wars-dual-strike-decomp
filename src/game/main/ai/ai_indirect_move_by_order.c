#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL func_0209dd7c(void);
BOOL Player_HasHq(int player);
void AiIndirect_MoveOrder10(void);
void AiIndirect_MoveOrder9(void);
void AiIndirect_MoveOrder8(void);
void AiIndirect_MoveOrder7(void);
void AiIndirect_MoveOrder6(void);
void AiIndirect_MoveOrder5(void);
void AiIndirect_MoveOrder4(void);
void AiIndirect_MoveOrder3(void);
void AiIndirect_MoveOrder2(void);
void AiIndirect_MoveOrder1(void);
void AiIndirect_MoveOrder0(void);

/* Queue routine for indirect units, set by AiAction_QueueIndirectMoves:
 * runs the move sequence for the acting unit's AI order. Same shape as
 * AiUnit_ActByOrder for direct units. */
void AiIndirect_MoveByOrder(void) {
  Unk_02080440_48* game = data_027c027c->unk_48[data_027c0284];

  switch (game->actingUnit->aiOrder) {
    case 0:
      AiIndirect_MoveOrder0();
      break;
    case 1:
      AiIndirect_MoveOrder1();
      break;
    case 2:
      if (func_0209dd7c() == TRUE) {
        AiIndirect_MoveOrder2();
      } else {
        AiIndirect_MoveOrder0();
      }
      break;
    case 3:
      AiIndirect_MoveOrder3();
      break;
    case 4:
      AiIndirect_MoveOrder4();
      break;
    case 5:
      AiIndirect_MoveOrder5();
      break;
    case 6:
      if (Player_HasHq(game->currentPlayer) == TRUE) {
        AiIndirect_MoveOrder6();
      } else {
        AiIndirect_MoveOrder5();
      }
      break;
    case 7:
      AiIndirect_MoveOrder7();
      break;
    case 8:
      AiIndirect_MoveOrder8();
      break;
    case 9:
      AiIndirect_MoveOrder9();
      break;
    case 10:
      AiIndirect_MoveOrder10();
      break;
  }
}

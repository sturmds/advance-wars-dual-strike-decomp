#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void AiFogDirect_ActOrder7(void);
void AiFogDirect_ActOrders1_10(void);
void AiFogDirect_ActOtherOrders(void);

/* Queue routine for the direct units AiAction_QueueAllDirectUnitsFog
 * collects: runs one of three action sequences on the acting unit, chosen
 * by its AI order. The flag test is the inverse of the one in
 * AiUnit_ActByOrderGroup. */
void AiUnit_ActByOrderFog(void) {
  if (data_027c027c->unk_48[data_027c0284]->actingUnit->serviceWanted == 2) {
    gAiTurnState->stepFlags |= AI_STEP_NO_ADJUST;
  }

  switch (data_027c027c->unk_48[data_027c0284]->actingUnit->aiOrder) {
    case 0:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 8:
    case 9:
      AiFogDirect_ActOtherOrders();
      break;
    case 1:
    case 10:
      AiFogDirect_ActOrders1_10();
      break;
    case 7:
      AiFogDirect_ActOrder7();
      break;
  }
}

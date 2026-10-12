#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void AiIndirect_ActOrders1_7_10(void);
void AiIndirect_ActOtherOrders(void);

/* Queue routine for the units AiAction_QueueIndirectUnits collects: runs one of two
 * action sequences on the active unit, chosen by its AI order. */
void AiUnit_ActByOrderGroup(void) {
  if (data_027c027c->unk_48[data_027c0284]->actingUnit->serviceWanted != 2) {
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
      AiIndirect_ActOtherOrders();
      break;
    case 1:
    case 7:
    case 10:
      AiIndirect_ActOrders1_7_10();
      break;
  }
}

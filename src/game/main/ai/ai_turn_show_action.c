#include "types.h"
#include "game/ai_unit_action.h"

extern u32 data_0229eac4;

void AiShowAction_Step0(void);
void AiShowAction_Step1(void);
void AiShowAction_Step2(void);
void AiShowAction_Step3(void);
void AiShowAction_Step4(void);
void AiShowAction_Step5(void);
void AiShowAction_Step6(void);
void AiShowAction_Step7(void);
void AiShowAction_Step8(void);
void AiShowAction_Step9(void);
void AiShowAction_Step10(void);
void AiShowAction_Step11(void);

/* Runs one step of showing the action in gUnitAction, picked by
 * gAiTurnState->actionStep. The last steps set the state back to
 * AI_STATE_RUN_QUEUE and actionStep to 0. */
void AiTurn_ShowAction(void) {
  data_0229eac4 = 0;
  switch (gAiTurnState->actionStep) {
    case 0:
      AiShowAction_Step0();
      break;
    case 1:
      AiShowAction_Step1();
      break;
    case 2:
      AiShowAction_Step2();
      break;
    case 3:
      AiShowAction_Step3();
      break;
    case 4:
      AiShowAction_Step4();
      break;
    case 5:
      AiShowAction_Step5();
      break;
    case 6:
      AiShowAction_Step6();
      break;
    case 7:
      AiShowAction_Step7();
      break;
    case 8:
      AiShowAction_Step8();
      break;
    case 9:
      AiShowAction_Step9();
      break;
    case 10:
      AiShowAction_Step10();
      break;
    case 11:
      AiShowAction_Step11();
      break;
  }
}

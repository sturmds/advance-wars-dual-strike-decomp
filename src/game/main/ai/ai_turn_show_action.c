#include "types.h"
#include "game/ai_unit_action.h"

extern u32 data_0229eac4;

void func_020b91b0(void);
void func_020b9114(void);
void func_020b8cf0(void);
void AiShowAction_Step3(void);
void AiShowAction_Step4(void);
void AiShowAction_Step5(void);
void AiShowAction_Step6(void);
void func_020b8850(void);
void func_020b8798(void);
void func_020b86e0(void);
void AiShowAction_Step10(void);
void AiShowAction_Step11(void);

/* Runs one step of showing the action in gUnitAction, picked by
 * gAiTurnState->actionStep. The last steps set the state back to
 * AI_STATE_RUN_QUEUE and actionStep to 0. */
void AiTurn_ShowAction(void)
{
    data_0229eac4 = 0;
    switch (gAiTurnState->actionStep) {
    case 0:
        func_020b91b0();
        break;
    case 1:
        func_020b9114();
        break;
    case 2:
        func_020b8cf0();
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
        func_020b8850();
        break;
    case 8:
        func_020b8798();
        break;
    case 9:
        func_020b86e0();
        break;
    case 10:
        AiShowAction_Step10();
        break;
    case 11:
        AiShowAction_Step11();
        break;
    }
}

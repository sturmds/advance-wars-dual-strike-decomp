#include "types.h"
#include "game/ai_unit_action.h"

extern u32 data_0229eac4;

void func_020b91b0(void);
void func_020b9114(void);
void func_020b8cf0(void);
void func_020b869c(void);
void func_020b8654(void);
void func_020b8604(void);
void func_020b85e0(void);
void func_020b8850(void);
void func_020b8798(void);
void func_020b86e0(void);
void func_020b8568(void);
void func_020b8544(void);

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
        func_020b869c();
        break;
    case 4:
        func_020b8654();
        break;
    case 5:
        func_020b8604();
        break;
    case 6:
        func_020b85e0();
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
        func_020b8568();
        break;
    case 11:
        func_020b8544();
        break;
    }
}

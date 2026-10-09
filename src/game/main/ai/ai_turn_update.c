#include "types.h"
#include "game/ai_unit_action.h"

BOOL func_0201ad28(void);
void func_0208d984(void);
void AiTurn_RunPhase(void);
void AiTurn_RunQueue(void);
void func_020b9540(void);
void func_02093824(void);
void func_02099f88(void);
void func_0208c230(void);

/* Runs one step of the AI turn. Does nothing while func_0201ad28
 * returns nonzero. */
void AiTurn_Update(void)
{
    if (func_0201ad28()) {
        return;
    }
    switch (*gAiTurnState) {
    case AI_STATE_START:
        func_0208d984();
        break;
    case AI_STATE_RUN_PHASE:
        AiTurn_RunPhase();
        break;
    case AI_STATE_RUN_QUEUE:
        AiTurn_RunQueue();
        break;
    case AI_STATE_SHOW_ACTION:
        func_020b9540();
        break;
    case AI_STATE_END_ACTIONS:
        func_02093824();
        break;
    case 5:
        func_02099f88();
        break;
    case 6:
        func_0208c230();
        break;
    }
}

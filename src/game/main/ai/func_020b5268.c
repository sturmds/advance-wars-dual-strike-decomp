#include "types.h"
#include "game/ai_unit_action.h"

extern void (*data_0229e164)(void);
extern int (*data_0229e16c)(void);

void func_020b52d4(void);
int func_020b52cc(void);
void func_020b5750(void);
void func_020b5bf4(void);

void func_020b5268(void)
{
    data_0229e164 = func_020b52d4;
    data_0229e16c = func_020b52cc;
    func_020b5750();
    if (gAiTurnState->stepFlags & AI_STEP_BIT_08) {
        func_020b5bf4();
    }
}

#include "types.h"
#include "game/ai_unit_action.h"

extern void (*data_0229e164)(void);
extern int (*data_0229e16c)(void);
void func_020b56ac(void);
int func_020b5668(void);
void func_020b5750(void);
void func_020b5bf4(void);

void func_020b5604(void)
{
    data_0229e164 = func_020b56ac;
    data_0229e16c = func_020b5668;
    func_020b5750();
    if (gAiTurnState->stepFlags & AI_STEP_BIT_08) {
        func_020b5bf4();
    }
}

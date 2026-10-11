#include "types.h"
#include "game/ai_unit_action.h"

extern void (*data_0229e164)(void);
extern int (*data_0229e16c)(void);
void AiSeaTransport_PropertiesOutweighArmed(void);
int func_020b5198(void);
void func_020b5464(void);
void func_020b5750(void);
void func_020b5bf4(void);

void func_020b5138(void)
{
    data_0229e164 = AiSeaTransport_PropertiesOutweighArmed;
    data_0229e16c = func_020b5198;
    func_020b5750();
    if (gAiTurnState->stepFlags & AI_STEP_BIT_08) {
        func_020b5bf4();
    }
    func_020b5464();
}

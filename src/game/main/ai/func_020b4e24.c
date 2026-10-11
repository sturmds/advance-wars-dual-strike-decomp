#include "types.h"
#include "game/ai_unit_action.h"

extern void (*data_0229e164)(void);
extern int (*data_0229e16c)(void);
void AiSeaTransport_CargoReachesHq(void);
int func_020b4e84(void);
void func_020b5464(void);
void AiSeaTransport_ScoreUnloadTiles(void);
void func_020b5bf4(void);

void func_020b4e24(void)
{
    data_0229e164 = AiSeaTransport_CargoReachesHq;
    data_0229e16c = func_020b4e84;
    AiSeaTransport_ScoreUnloadTiles();
    if (gAiTurnState->stepFlags & AI_STEP_BIT_08) {
        func_020b5bf4();
    }
    func_020b5464();
}

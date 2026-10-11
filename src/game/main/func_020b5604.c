#include "types.h"
#include "game/ai_unit_action.h"

extern void (*data_0229e164)(void);
extern int (*data_0229e16c)(void);
void AiSeaTransport_FewFootUnitsNoTarget(void);
int AiSeaTransport_ScoreFootUnitsVsProperties(void);
void AiSeaTransport_ScoreUnloadTiles(void);
void func_020b5bf4(void);

void func_020b5604(void)
{
    data_0229e164 = AiSeaTransport_FewFootUnitsNoTarget;
    data_0229e16c = AiSeaTransport_ScoreFootUnitsVsProperties;
    AiSeaTransport_ScoreUnloadTiles();
    if (gAiTurnState->stepFlags & AI_STEP_BIT_08) {
        func_020b5bf4();
    }
}

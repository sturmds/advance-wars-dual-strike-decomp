#include "types.h"
#include "game/ai_unit_action.h"

extern void (*data_0229e164)(void);
extern int (*data_0229e16c)(void);
void AiSeaTransport_FewArmedUnitsNoTarget(void);
int AiSeaTransport_ScoreArmedCount(void);
void func_020b5268(void);
void AiSeaTransport_ScoreUnloadTiles(void);
void func_020b5bf4(void);

void func_020b5464(void)
{
    data_0229e164 = AiSeaTransport_FewArmedUnitsNoTarget;
    data_0229e16c = AiSeaTransport_ScoreArmedCount;
    AiSeaTransport_ScoreUnloadTiles();
    if (gAiTurnState->stepFlags & AI_STEP_BIT_08) {
        func_020b5bf4();
    }
    func_020b5268();
}

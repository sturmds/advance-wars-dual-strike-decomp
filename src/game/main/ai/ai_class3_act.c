#include "types.h"

void Ai_BuildThreatMaps(void);
void func_020af374(void);

/* Queue routine set by AiAction_QueueClass3Units. No unit type has AI
 * class 3, so retail never runs it. */
void AiClass3_Act(void)
{
    Ai_BuildThreatMaps();
    func_020af374();
}

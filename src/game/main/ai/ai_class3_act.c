#include "types.h"

void func_0209c948(void);
void func_020af374(void);

/* Queue routine set by AiAction_QueueClass3Units. No unit type has AI
 * class 3, so retail never runs it. */
void AiClass3_Act(void)
{
    func_0209c948();
    func_020af374();
}

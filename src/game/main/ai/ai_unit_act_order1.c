#include "types.h"

void Ai_ClearThreatMaps(void);
void AiUnit_MoveToOtherMap(void);
void func_020a9ac0(void);
void func_020aac28(void);
void func_020aacc0(void);
void AiUnit_SeekSeaTransport(void);
void func_020af540(void);
void AiUnit_PlanAttack(void);
void AiUnit_WaitInPlace(void);
void func_020b297c(void);

/* AiUnit_ActByOrder runs these AI steps for units with AI order 1. The
 * last one, AiUnit_WaitInPlace, makes the unit stay on its tile
 * (AiUnit_PerformAction with action 2). */
void AiUnit_ActOrder1(void)
{
    Ai_ClearThreatMaps();
    func_020af540();
    AiUnit_PlanAttack();
    func_020aacc0();
    AiUnit_SeekSeaTransport();
    func_020b297c();
    func_020a9ac0();
    func_020aac28();
    AiUnit_MoveToOtherMap();
    AiUnit_WaitInPlace();
}

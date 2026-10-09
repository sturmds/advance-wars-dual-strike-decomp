#include "types.h"

void Ai_BuildThreatMaps(void);
void func_020a9854(void);
void func_020a9ac0(void);
void func_020aac28(void);
void func_020aacc0(void);
void func_020ab014(void);
void func_020af5f8(void);
void func_020b0a70(void);
void AiUnit_PlanAttack(void);
void AiUnit_WaitInPlace(void);
void func_020b297c(void);

/* AiUnit_ActByOrder runs these AI steps for units with AI order 0, and
 * for order 2 when func_0209dd7c is FALSE. The last one, AiUnit_WaitInPlace,
 * makes the unit stay on its tile (AiUnit_PerformAction with action 2). */
void AiUnit_ActOrder0(void)
{
    Ai_BuildThreatMaps();
    func_020af5f8();
    AiUnit_PlanAttack();
    func_020b0a70();
    func_020aacc0();
    func_020ab014();
    func_020b297c();
    func_020a9ac0();
    func_020aac28();
    func_020a9854();
    AiUnit_WaitInPlace();
}

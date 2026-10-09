#include "types.h"

void Ai_BuildThreatMaps(void);
void func_020a9854(void);
void func_020a9ac0(void);
void func_020aa4a4(void);
void func_020aacc0(void);
void AiUnit_SeekSeaTransport(void);
void func_020af5f8(void);
void func_020b0a70(void);
void AiUnit_PlanAttack(void);
void AiUnit_WaitInPlace(void);
void func_020b297c(void);

/* AiUnit_ActByOrder runs these AI steps for units with AI order 4. The
 * last one, AiUnit_WaitInPlace, makes the unit stay on its tile
 * (AiUnit_PerformAction with action 2). */
void AiUnit_ActOrder4(void)
{
    Ai_BuildThreatMaps();
    func_020af5f8();
    AiUnit_PlanAttack();
    func_020b0a70();
    func_020aa4a4();
    AiUnit_SeekSeaTransport();
    func_020b297c();
    func_020aacc0();
    func_020a9ac0();
    func_020a9854();
    AiUnit_WaitInPlace();
}

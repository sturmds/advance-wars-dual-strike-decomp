#include "types.h"

void Ai_BuildThreatMaps(void);
void func_020a9e44(void);
void AiUnit_SeekSeaTransport(void);
void func_020af5f8(void);
void func_020b0a70(void);
void AiUnit_PlanAttack(void);
void AiUnit_WaitInPlace(void);
void func_020b297c(void);

/* AiUnit_ActByOrder runs these AI steps for units with AI order 9. The
 * last one, AiUnit_WaitInPlace, makes the unit stay on its tile
 * (AiUnit_PerformAction with action 2). */
void AiUnit_ActOrder9(void)
{
    Ai_BuildThreatMaps();
    func_020af5f8();
    AiUnit_PlanAttack();
    func_020b0a70();
    func_020a9e44();
    AiUnit_SeekSeaTransport();
    func_020b297c();
    AiUnit_WaitInPlace();
}

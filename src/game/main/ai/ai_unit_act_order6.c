#include "types.h"

void Ai_BuildThreatMaps(void);
void func_020a9854(void);
void func_020aa068(void);
void func_020ab014(void);
void func_020af5f8(void);
void func_020b0a70(void);
void AiUnit_PlanAttack(void);
void func_020b2904(void);
void func_020b297c(void);

/* AiUnit_ActByOrder runs these AI steps for units with AI order 6 when
 * func_020c0e3c is TRUE for the current player (otherwise the order 5
 * steps). The last one, func_020b2904, makes the unit stay on its tile
 * (AiUnit_PerformAction with action 2). */
void AiUnit_ActOrder6(void)
{
    Ai_BuildThreatMaps();
    func_020af5f8();
    AiUnit_PlanAttack();
    func_020b0a70();
    func_020aa068();
    func_020ab014();
    func_020b297c();
    func_020a9854();
    func_020b2904();
}

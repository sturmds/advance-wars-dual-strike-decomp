#include "types.h"

void func_0209ca48(void);
void func_020a9854(void);
void func_020a9ac0(void);
void func_020aac28(void);
void func_020aacc0(void);
void func_020ab014(void);
void func_020af540(void);
void AiUnit_PlanAttack(void);
void func_020b2904(void);
void func_020b297c(void);

/* AiUnit_ActByOrder runs these AI steps for units with AI order 1. The
 * last one, func_020b2904, makes the unit stay on its tile
 * (AiUnit_PerformAction with action 2). */
void AiUnit_ActOrder1(void)
{
    func_0209ca48();
    func_020af540();
    AiUnit_PlanAttack();
    func_020aacc0();
    func_020ab014();
    func_020b297c();
    func_020a9ac0();
    func_020aac28();
    func_020a9854();
    func_020b2904();
}

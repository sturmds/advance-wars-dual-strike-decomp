#include "types.h"

void func_0209ca48(void);
void func_020a9f64(void);
void func_020ab014(void);
void func_020af540(void);
void AiUnit_PlanAttack(void);
void func_020b2904(void);
void func_020b297c(void);

/* AiUnit_ActByOrder runs these AI steps for units with AI order 10. The
 * last one, func_020b2904, makes the unit stay on its tile
 * (AiUnit_PerformAction with action 2). */
void AiUnit_ActOrder10(void)
{
    func_0209ca48();
    func_020af540();
    AiUnit_PlanAttack();
    func_020a9f64();
    func_020ab014();
    func_020b297c();
    func_020b2904();
}

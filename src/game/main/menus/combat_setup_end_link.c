#include "types.h"

extern u32 data_0229eaec;

void func_0207b4e8(void);
void func_0207bfb8(void);

/* Last step of the Combat setup task script: clears the data_0229eaec flag that
 * CombatSetup_StartLink sets, then runs the link shutdown steps as LinkMode_Finish does. */
void CombatSetup_EndLink(void)
{
    data_0229eaec = 0;
    func_0207b4e8();
    func_0207bfb8();
}

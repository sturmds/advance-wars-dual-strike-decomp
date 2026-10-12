#include "types.h"

extern u32 gLinkPlay;

void func_0207b4e8(void);
void func_0207bfb8(void);

/* Last step of the Combat setup task script: clears the gLinkPlay flag that
 * CombatSetup_StartLink sets, then runs the link shutdown steps as LinkMode_Finish does. */
void CombatSetup_EndLink(void) {
  gLinkPlay = 0;
  func_0207b4e8();
  func_0207bfb8();
}

#include "types.h"

int Link_IsActive(void);
void func_0207b4e8(void);
void func_0207bfb8(void);
void GameFlagsMid_Restore(void);
void func_02103714(void);

/* In link mode runs GameFlagsMid_Restore, func_0207b4e8 (link state 2 to 3) and
 * func_0207bfb8; outside link mode runs func_02103714. Name is a guess from
 * those steps. */
void LinkMode_Finish(void) {
  if (Link_IsActive() == 1) {
    GameFlagsMid_Restore();
    func_0207b4e8();
    func_0207bfb8();
  } else {
    func_02103714();
  }
}

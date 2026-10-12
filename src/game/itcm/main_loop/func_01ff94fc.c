#include "types.h"
#include "game/main_loop.h"

extern MainLoop gMainLoop;

int Link_IsActive(void);
void Link_FlagError(void);
int func_020fcad8(void);
void func_020fc134(void);

void func_01ff94fc(void) {
  gMainLoop.unk_24 = 2;
  if (Link_IsActive() == 1) {
    Link_FlagError();
  }
  if (func_020fcad8() != 1) {
    return;
  }
  func_020fc134();
}

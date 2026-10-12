#include "types.h"
#include "game/Unk_0229fb30.h"

void func_0207e5d0(void);
void LinkMenu_PushBack(void);

/* Back in the two-player link menu: stage 1 queues event 2 (func_0207e5d0), stages 2 and 3
 * queue event 4. */
void LinkMenu_HandleBack(void) {
  switch (data_0229fb30.unk_630) {
    case 1:
      func_0207e5d0();
      break;
    case 2:
    case 3:
      LinkMenu_PushBack();
      break;
  }
}

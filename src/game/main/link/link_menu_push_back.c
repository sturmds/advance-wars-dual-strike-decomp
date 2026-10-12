#include "types.h"
#include "game/event_queue.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

int func_0207a260(void);

/* Queues event 4 (back in stage 2 and 3). The address test is never true; the original compares
 * an address in gMenuState with NULL here, and the compare is kept to match. */
void LinkMenu_PushBack(void) {
  if (func_0207a260() != 0 &&
      (u8*)&gMenuState + func_0207a260() * 0x1634 + LINK_PICKED_OFFSET ==
          NULL) {
    return;
  }
  EventQueue_Push(4, 0);
}

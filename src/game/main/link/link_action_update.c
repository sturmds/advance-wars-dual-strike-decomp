#include "types.h"

void func_02080440(void);
void LinkAction_StartNext(void);

extern u8 data_0229d8c0;
extern u16 data_0229d8c4;

/* Per-frame step for actions queued from the link. State 0 waits for the next
 * action (LinkAction_StartNext takes it off the queue); state 4 shows it. */
void LinkAction_Update(void) {
  u16 state = data_0229d8c4;

  data_0229d8c0 = 0;
  switch (state) {
    case 0:
      LinkAction_StartNext();
      break;
    case 4:
      func_02080440();
      break;
  }
}

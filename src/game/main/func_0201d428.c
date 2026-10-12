#include "types.h"
#include "game/event_script.h"

void func_020f5888(s16 x, s16 y);
void EventScript_ReleaseHoldAfterScroll(void);

BOOL func_0201d428(s16 index) {
  func_020f5888(data_022977f0[index].pc->unk_08,
                data_022977f0[index].pc->unk_0a);
  data_022977f0[index].hold = (u32)EventScript_ReleaseHoldAfterScroll;
  data_022977f0[index].pc++;
  return FALSE;
}

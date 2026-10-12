#include "types.h"
#include "game/co_select_screen.h"

/* Slot of `co` in the CO list; the slot of the end mark (0) when absent. */
int CoSelectScreen_ListSlotOfCo(int co) {
  u8* slot;
  u8 listed;
  u8* list;

  list = data_0229c73c->coList;
  listed = *list;
  slot = list;
  while (listed != 0) {
    if (listed == co) {
      return slot - list;
    }
    listed = *++slot;
  }
  return slot - list;
}

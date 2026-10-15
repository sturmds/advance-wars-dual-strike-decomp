#include "types.h"
#include "game/Unk_02080440.h"

void func_0201e954(void);

/* Battle menu entry handler (table at 0x02165d30, with the entry test func_020c01c0).
 * Stores the menu value in unk_14 like func_020c02f0 and func_020bd900, then
 * sets unk_1c to 2. Purpose not known yet. */
void func_020bfd50(int unused, u32 value) {
  data_027c027c->unk_48[data_027c0284]->unk_14 = value;
  func_0201e954();
  data_027c027c->unk_48[data_027c0284]->unk_1c = 2;
}

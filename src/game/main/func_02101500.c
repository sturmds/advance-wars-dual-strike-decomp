#include "types.h"
#include "game/name_entry_work.h"

void func_02101500(u8* a0) {
  NameEntry* work = gNameEntry;
  work->unk_11c = 1;
  work->cursor = *a0 - NAME_ENTRY_KEY_BUTTON_ID;
}

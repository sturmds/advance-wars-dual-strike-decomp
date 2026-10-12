#include "types.h"
#include "game/text_print.h"

const void* Text_Get(u32 textId);

s8 Text_PrintId(s16 x, s16 y, u16* map, u32 textId, u16 unk_30, u16 unk_32) {
  const void* t0 = Text_Get(textId);
  return Text_Print(x, y, map, t0, unk_30, unk_32);
}

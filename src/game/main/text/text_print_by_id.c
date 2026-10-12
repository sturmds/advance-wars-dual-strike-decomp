#include "types.h"

const void* Text_Get(u32 textId);
void func_020151c4(int x, int y, u16* map, const void* text, u16 unk_30,
                   u16 unk_32);

/* Looks up the string for textId and prints it at tile (x, y) of map
 * through func_020151c4. */
void Text_PrintById(int x, int y, u16* map, u32 textId, u16 unk_30,
                    u16 unk_32) {
  func_020151c4(x, y, map, Text_Get(textId), unk_30, unk_32);
}

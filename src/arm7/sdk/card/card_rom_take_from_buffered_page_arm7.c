#include "types.h"
#include "sdk/card_arm7.h"
#include "sdk/mi.h"

/* Copies the part of the page buffer that the request asked for to its destination. Returns whether bytes remain. */
int CardRom_TakeFromBufferedPage_Arm7(CARDiRomPage* page) {
  CARDiArm7Common* common = &data_arm7_03809280;
  u32 pageStart = common->src & ~(GAMECARD_ROM_PAGE_BYTES - 1);

  if (pageStart == page->address) {
    u32 offset = common->src - pageStart;
    u32 count = GAMECARD_ROM_PAGE_BYTES - offset;

    if (count > common->len) {
      count = common->len;
    }
    Mem_CopyBytes_Arm7(page->data + offset, common->dst, count);
    common->src += count;
    common->dst += count;
    common->len -= count;
  }
  return common->len != 0;
}

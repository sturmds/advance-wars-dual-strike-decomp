#include "types.h"
#include "game/Unk_ov005_0236bbb0.h"
#include "game/math.h"

void Mem_Copy32(const void* src, void* dst, u32 size);

static ShopButton data_ov005_0236b498[] = {
#include "data/ov005/data_ov005_0236b498.inc.c"
};

void Ov005_InitShopPaging(Unk_ov005_0236bbb0* work) {
  work->cursorRow = 0;
  work->drawnCursorRow = 0;
  work->savedCursorRow = 0;
  work->buttons = work->buttonBuf;
  work->page = 0;
  work->lastPage = HwDiv32(work->itemCount - 1, 6);
  Mem_Copy32(data_ov005_0236b498, work->buttons, sizeof(work->buttonBuf));
}

#include "types.h"
#include "sdk/gx_load.h"

/* Rows of three LCDC addresses, in 4 KB pages, by bank mask: columns 0, 1, 2. */
const u16 gGxTexLcdcPages0[][3] = {
#include "data/main/gGxTexLcdcPages0.inc.c"
};
/* volume table */
const s16 data_02151534[] = {
#include "data/main/data_02151534.inc.c"
};
extern u16 gGxTexLcdcPages1[];
extern u16 gGxTexLcdcPages2[];

int func_0210a948(void);

void GX_BeginLoadTex(void) {
  int banks = func_0210a948();
  int row =
      banks *
      6; /* byte offset: 3 halfwords per row; indexing the arrays does not match */

  data_022a98b0 = banks;
  data_022a98a4 = *(u16*)((u8*)gGxTexLcdcPages0 + row) << 12;
  data_022a98b4 = *(u16*)((u8*)gGxTexLcdcPages1 + row) << 12;
  data_022a98b8 = *(u16*)((u8*)gGxTexLcdcPages2 + row) << 12;
}

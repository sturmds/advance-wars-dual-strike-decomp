#include "types.h"
#include "sdk/gx_load.h"

/* LCDC address of the texture palette bank, in 4 KB pages, by bank >> 4. */
extern u16 gGxTexPlttLcdcPage[8];

int func_0210a934(void);

void GX_BeginLoadTexPltt(void) {
  int bank = func_0210a934();

  data_022a98ac = bank;
  data_022a98a8 = gGxTexPlttLcdcPage[bank >> 4] << 12;
}

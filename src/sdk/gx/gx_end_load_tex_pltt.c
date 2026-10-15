#include "types.h"
#include "sdk/gx_load.h"
#include "sdk/gx.h"

void GX_EndLoadTexPltt(void) {
  if (data_0216d958 != -1) {
    MI_WaitDma(data_0216d958);
  }
  GX_SetBankForTexPltt(data_022a98ac);
  data_022a98ac = 0;
  data_022a98a8 = 0;
}

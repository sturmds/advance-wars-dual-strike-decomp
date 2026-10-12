#include "types.h"
#include "sdk/gx_load.h"

void GX_SetBankForBGExtPltt(int bank);

void GX_EndLoadBGExtPltt(void) {
  if (data_0216d958 != -1) {
    MI_WaitDma(data_0216d958);
  }
  GX_SetBankForBGExtPltt(data_022a98a0);
  data_022a98a0 = 0;
  data_022a989c = 0;
  data_022a9898 = 0;
}

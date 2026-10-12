#include "types.h"
#include "sdk/gx_load.h"

void GX_SetBankForTex(int bank);

void GX_EndLoadTex(void) {
  if (data_0216d958 != -1) {
    MI_WaitDma(data_0216d958);
  }
  GX_SetBankForTex(data_022a98b0);
  data_022a98b8 = 0;
  data_022a98b4 = 0;
  data_022a98a4 = 0;
  data_022a98b0 = 0;
}

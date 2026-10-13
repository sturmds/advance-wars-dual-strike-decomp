#include "types.h"
#include "sdk/gx_load.h"

int data_022a9894; /* also declared in src/game/main/func_0210c610.c */
void GX_SetBankForSubBGExtPltt(int bank);

void GXS_EndLoadBGExtPltt(void) {
  if (data_0216d958 != -1) {
    MI_WaitDma(data_0216d958);
  }
  GX_SetBankForSubBGExtPltt(data_022a9894);
  data_022a9894 = 0;
}

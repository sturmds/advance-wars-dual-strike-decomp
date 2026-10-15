#include "types.h"
#include "sdk/gx_load.h"
#include "sdk/gx.h"

int data_022a98b8;
int data_022a98a4;
int data_022a98a8;
int data_022a98ac;
int data_022a98b0;
int data_022a98b4;

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

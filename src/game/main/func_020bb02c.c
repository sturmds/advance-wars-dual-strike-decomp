#include "types.h"

extern int data_02168314;
extern int data_022a6dcc;
extern int data_022a6dd0;
void TileCodeTable_Load(void*, void*, int);

void func_020bb02c(void) {
  int t0 = data_02168314;
  data_022a6dcc = t0;
  data_022a6dd0 = t0;
  TileCodeTable_Load(&data_022a6dd0, &data_022a6dcc, t0);
}

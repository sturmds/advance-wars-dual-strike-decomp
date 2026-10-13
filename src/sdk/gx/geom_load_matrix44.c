#include "types.h"
#include "sdk/gx.h"
#include "sdk/mi.h"

void Geom_LoadMatrix44(const MtxFx44* mtx) {
  *REG_GXFIFO = GXFIFO_CMD_MTX_LOAD_4X4;
  Fifo_Send16Words(mtx, (void*)REG_GXFIFO);
}

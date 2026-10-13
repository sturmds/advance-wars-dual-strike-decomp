#include "types.h"
#include "sdk/gx.h"
#include "sdk/mi.h"

void Geom_MultMatrix33(const MtxFx33* mtx) {
  *REG_GXFIFO = GXFIFO_CMD_MTX_MULT_3X3;
  MI_Copy36B(mtx, (void*)REG_GXFIFO);
}

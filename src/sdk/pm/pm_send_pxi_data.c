#include "sdk/pm.h"
#include "sdk/pxi.h"

void PMi_SendPxiData(u32 data) {
  while (PXI_SendWordByFifo(PXI_FIFO_TAG_PM, data, FALSE) != 0) {
  }
}

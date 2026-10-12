#include "types.h"
#include "game/transfer_queue.h"

void Mem_CopyAuto(const void* src, void* dst, u32 size);
void Mem_CopyUnits(const void* src, void* dst, u32 size, u32 unitBits);
void Data_Unpack(const void* src, void* dst);
void Mem_ZeroUnits(u32 value, const void* dst, u32 size, u32 unitBits);
void Mem_Zero32(u32 value, const void* dst, u32 size);
void TransferQueue_Clear(void);

/* Runs the queued transfers, last queued first, then empties the queue.
 * For the two clear kinds, `src` holds the address to clear. Called from
 * MainLoop_DefaultVBlank once the main loop has finished a frame. */
void TransferQueue_Run(void) {
  if (data_0216e04c == 0) {
    return;
  }
  for (data_0216e04c--; data_0216e04c >= 0; data_0216e04c--) {
    switch (data_0216e05c[data_0216e04c].kind) {
      case TRANSFER_COPY:
      case 1:
        Mem_CopyAuto(data_0216e05c[data_0216e04c].src,
                     data_0216e05c[data_0216e04c].dst,
                     data_0216e05c[data_0216e04c].size);
        break;
      case TRANSFER_COPY16:
        Mem_CopyUnits(data_0216e05c[data_0216e04c].src,
                      data_0216e05c[data_0216e04c].dst,
                      data_0216e05c[data_0216e04c].size, 16);
        break;
      case TRANSFER_UNPACK:
        Data_Unpack(data_0216e05c[data_0216e04c].src,
                    data_0216e05c[data_0216e04c].dst);
        break;
      case TRANSFER_CLEAR16:
        Mem_ZeroUnits(0, data_0216e05c[data_0216e04c].src,
                      data_0216e05c[data_0216e04c].size, 16);
        break;
      case TRANSFER_CLEAR32:
        Mem_Zero32(0, data_0216e05c[data_0216e04c].src,
                   data_0216e05c[data_0216e04c].size);
        break;
    }
  }
  TransferQueue_Clear();
}

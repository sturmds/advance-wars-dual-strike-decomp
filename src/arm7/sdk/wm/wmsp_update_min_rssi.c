#include "types.h"
#include "sdk/wmsp.h"

u8 Wmsp_DecodeRssi(int rawRssi);

/* Lowers WMStatus.unk_ba (the lowest RSSI seen) to the weakest child in the
   MP replies, or to 4 at most when capAt4 is set. */
void Wmsp_UpdateMinRssi(WlMpEndIndication* ind, BOOL capAt4) {
  // Needed to match: this declaration order sets the register allocation.
  int i;
  u32 minRssi;
  WmspWork* work;
  u32 rssi;
  u8* entries;

  work = func_arm7_027c05e0();
  minRssi = work->buf->status->unk_ba;

  if (capAt4) {
    if (minRssi > 4) {
      minRssi = 4;
    }
  } else {
    if (ind->recv.count < 1) {
      return;
    }
    i = 0;  // needed to match: the original sets i before entries
    entries = ind->recv.firstEntry;
    for (; i < ind->recv.count; i++) {
      // Byte 3 of each WMMpRecvData holds the raw RSSI.
      rssi = Wmsp_DecodeRssi((entries + ind->recv.stride * i)[3]);
      if (rssi < minRssi) {
        minRssi = rssi;
      }
    }
  }
  work->buf->status->unk_ba = minRssi;
}

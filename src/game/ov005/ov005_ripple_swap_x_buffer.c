#include "types.h"
#include "game/Unk_ov005_0236bb80.h"

extern Unk_ov005_0236bb80* data_ov005_0236bb80;

/* Switches the x line table to the buffer that is not in use (double buffering). */
void Ov005Ripple_SwapXBuffer(void) {
  Unk_ov005_0236bb80* work = data_ov005_0236bb80;

  if (work->xUsesBufferB != 0) {
    work->xLines = work->xBufferA;
    work->xUsesBufferB = 0;
  } else {
    work->xLines = work->xBufferB;
    work->xUsesBufferB = 1;
  }
}

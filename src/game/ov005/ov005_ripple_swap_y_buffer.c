#include "types.h"
#include "game/Unk_ov005_0236bb80.h"

extern Unk_ov005_0236bb80* data_ov005_0236bb80;

/* Switches the y line table to the buffer that is not in use (double buffering). */
void Ov005Ripple_SwapYBuffer(void) {
  Unk_ov005_0236bb80* work = data_ov005_0236bb80;

  if (work->yUsesBufferB != 0) {
    work->yLines = work->yBufferA;
    work->yUsesBufferB = 0;
  } else {
    work->yLines = work->yBufferB;
    work->yUsesBufferB = 1;
  }
}

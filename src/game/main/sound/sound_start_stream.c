#include "types.h"
#include "game/sound.h"

/* Starts sound archive stream strmNo from its beginning. */
void Sound_StartStream(int strmNo) {
  NNS_SndArcStrmStart(&gStrmHandle, strmNo, 0);
}

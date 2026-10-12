#include "types.h"
#include "game/sound.h"
#include "game/sound_arc.h"

/* Sound archive loaders: each loads one item into the sound heap and
 * returns TRUE on success. */

/* Loads the sound data the game keeps resident from boot: sequence
 * archive 1 and bank 0x30. */
void Sound_LoadResidentData(void) {
  NNS_SndArcLoadSeqArc(1, gSoundHeap);
  NNS_SndArcLoadBank(0x30, gSoundHeap);
}

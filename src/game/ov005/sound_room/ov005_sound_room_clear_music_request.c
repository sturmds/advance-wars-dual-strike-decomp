#include "types.h"
#include "game/Unk_ov005_0236bbac.h"

extern Unk_ov005_0236bbac* data_ov005_0236bbac;

/* Asks for no music: Ov005_UpdateMusic then fades out what plays. */
void Ov005SoundRoom_ClearMusicRequest(void) {
  Unk_ov005_0236bbac* state = data_ov005_0236bbac;
  state->musicId = -1;
  state->musicKind = -1;
}

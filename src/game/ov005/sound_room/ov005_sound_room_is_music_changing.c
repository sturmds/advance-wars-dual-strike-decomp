#include "types.h"
#include "game/Unk_ov005_0236bbac.h"

extern Unk_ov005_0236bbac* data_ov005_0236bbac;

/* 1 while Ov005_UpdateMusic fades out the playing music. */
int Ov005SoundRoom_IsMusicChanging(void) {
  return data_ov005_0236bbac->musicChanging;
}

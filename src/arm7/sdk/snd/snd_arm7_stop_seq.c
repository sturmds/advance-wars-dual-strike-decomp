#include "sdk/snd.h"
#include "sdk/snd_seq.h"

void SndArm7_StopSeq(int playerNo) {
  SndPlayer* player = &data_arm7_038085d0[playerNo];

  if (player->active) {
    SndPlayer_Stop(player);
    if (data_arm7_0380808c != NULL) {
      data_arm7_0380808c->playerStatus &= ~(1 << playerNo);
    }
  }
}

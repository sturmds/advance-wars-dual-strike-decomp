#include "sdk/snd_seq.h"
#include "sdk/snd.h"

int func_arm7_037ff7d8(SndExChannel* channel);
void func_arm7_037ff7e8(SndExChannel* channel);
int func_arm7_037ff7f4(SndExChannel* channel, int release);

void SndTrack_ReleaseChannels(SndTrack* track, SndPlayer* player, int release) {
  SndExChannel* channel;

  SndTrack_UpdateChannels(track, player, 0);
  for (channel = track->channels; channel != NULL; channel = channel->next) {
    if (func_arm7_037ff7d8(channel)) {
      if (release >= 0) {
        func_arm7_037ff7f4(channel, (u8)release);
      }
      channel->priority = 1;
      func_arm7_037ff7e8(channel);
    }
  }
}

#include "sdk/snd.h"

void func_arm7_037ff7e8(SndExChannel* channel);

#define TRACK_PAN_SPAN_UNSCALED 127

void SndTrack_UpdateChannels(SndTrack* track, SndPlayer* player, BOOL release) {
  int volume;
  int fader;
  int pan;
  int pitch;
  SndExChannel* channel;

  volume =
      data_arm7_03807180[player->volume] +
      (data_arm7_03807180[track->volume] + data_arm7_03807180[track->volume2]);
  fader = track->extFader + player->extFader;
  /* bend -128..127 scaled to bendRange semitones of 64 pitch units each */
  pitch = track->pitchBend;
  pitch *= track->bendRange << 6;
  pitch = (pitch >> 7) + track->extPitch;
  pan = track->pan;
  if (track->panRange != TRACK_PAN_SPAN_UNSCALED) {
    pan = (pan * track->panRange + 64) >> 7;
  }
  pan += track->extPan;
  if (pan < -128) {
    pan = -128;
  } else if (pan > 127) {
    pan = 127;
  }
  for (channel = track->channels; channel != NULL; channel = channel->next) {
    channel->userDecay2 = fader;
    if (channel->envStatus == ENV_STAGE_FADE) {
      continue;
    }
    channel->userDecay = volume;
    channel->userPitch = pitch;
    channel->userPan = pan;
    channel->panRange = track->panRange;
    channel->lfo.param = track->lfo;
    if (channel->length == 0 && release) {
      channel->priority = 1;
      func_arm7_037ff7e8(channel);
    }
  }
}

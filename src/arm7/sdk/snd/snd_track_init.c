#include "sdk/snd_seq.h"

void func_arm7_037ff3c4(SndLfo *lfo);

void SndTrack_Init(SndTrack *track)
{
    track->base = NULL;
    track->cursor = NULL;
    track->noteWait = 1;
    track->mute = 0;
    track->tie = 0;
    track->noteFinished = 0;
    track->portamento = 0;
    track->compare = 1;
    track->channelMaskSet = 0;
    track->unk_3b = 0;
    track->program = 0;
    track->priority = 64;
    track->volume = 127;
    track->volume2 = 127;
    track->extFader = 0;
    track->pan = 0;
    track->extPan = 0;
    track->pitchBend = 0;
    track->extPitch = 0;
    track->attack = 255;
    track->decay = 255;
    track->sustain = 255;
    track->release = 255;
    track->panRange = 127;
    track->bendRange = 2;
    track->portamentoKey = 60;
    track->portamentoTime = 0;
    track->sweepPitch = 0;
    track->transpose = 0;
    track->channelMask = 0xFFFF;
    func_arm7_037ff3c4(&track->lfo);
    track->unk_20 = 0;
    track->channels = NULL;
}

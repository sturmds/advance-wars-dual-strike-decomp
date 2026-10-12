#include "sdk/snd_seq.h"

void func_arm7_037ff3c4(SndLfo *lfo); /* LFO defaults: depth 0, range 1, speed 16 */

/* Puts a newly allocated channel back to its defaults: middle C, full velocity, all envelope settings at 127. */
void SndExChannel_Init(SndExChannel *channel, SndExChannelCallback callback, void *callbackArg, int priority)
{
    channel->next = NULL;
    channel->callback = callback;
    channel->callbackArg = callbackArg;
    channel->length = 0;
    channel->priority = priority;
    channel->volume = 127;
    channel->start = FALSE;
    channel->autoSweep = TRUE;
    channel->key = 60;
    channel->originalKey = 60;
    channel->velocity = 127;
    channel->initPan = 0;
    channel->userDecay = 0;
    channel->userDecay2 = 0;
    channel->userPitch = 0;
    channel->userPan = 0;
    channel->panRange = 127;
    channel->sweepPitch = 0;
    channel->sweepLength = 0;
    channel->sweepCounter = 0;
    channel->attack = 127;
    channel->decay = 127;
    channel->sustain = 127;
    channel->release = 127;
    func_arm7_037ff3c4(&channel->lfo.param);
}

#include "sdk/snd.h"
#include "sdk/snd_hw.h"

/* Quietest level Sound_DecibelToHwVolume_Arm7 knows, in tenths of a decibel. */
#define SND_VOLUME_DB_MIN (-723)

/* PSG channels run their timer in steps of 4. */
#define VOICE_SQUARE_RELOAD_ALIGN 0xFFFC

/*
 * Works out the volume, timer and pan of every active channel from its key,
 * velocity, envelope, sweep and LFO, and flags what changed for
 * SndExChannel_Update. A channel whose hardware stopped, or whose release
 * fell silent, is freed and its callback told. periodic is set on the
 * interval alarm's tick: envelopes, sweeps and LFOs advance only then.
 */
void SndExChannel_Main(BOOL periodic)
{
    SndExChannel *channel; /* needed to match: declared before i */
    int i;

    for (i = 0; i < AUDIO_CH_TOTAL; i++) {
        int volume = 0;
        int pitch = 0;
        int pan = 0;
        int lfo;
        u16 hwVolume;
        u16 timer;

        channel = &data_arm7_03808090.channels[i];

        if (!channel->active) {
            continue;
        }
        if (channel->start) {
            channel->sync |= VOICE_DIRTY_START;
            channel->start = FALSE;
        } else if (!SndHw_IsChannelBusy(i)) {
            if (channel->callback != NULL) {
                channel->callback(channel, VOICE_DONE, channel->callbackArg);
            } else {
                channel->priority = 0;
            }
            channel->volume = 0;
            channel->active = FALSE;
            continue;
        }

        volume += data_arm7_03807180[channel->velocity];
        pitch += (channel->key - channel->originalKey) << 6;

        volume += SndExChannel_UpdateEnvelope(channel, periodic);
        pitch += SndExChannel_UpdateSweep(channel, periodic);
        lfo = SndExChannel_UpdateLfo(channel, periodic);
        switch (channel->lfo.param.target) {
        case SND_LFO_VOLUME:
            volume += lfo;
            break;
        case SND_LFO_PAN:
            pan += lfo;
            break;
        case SND_LFO_PITCH:
            pitch += lfo;
            break;
        }

        pan += channel->initPan;
        if (channel->panRange != 127) {
            pan = (pan * channel->panRange + 64) >> 7;
        }
        volume += channel->userDecay;
        volume += channel->userDecay2;
        pitch += channel->userPitch;
        pan += channel->userPan;

        if (channel->envStatus == ENV_STAGE_FADE && volume <= SND_VOLUME_DB_MIN) {
            channel->sync = VOICE_DIRTY_STOP;
            if (channel->callback != NULL) {
                channel->callback(channel, VOICE_DONE, channel->callbackArg);
            } else {
                channel->priority = 0;
            }
            channel->volume = 0;
            channel->active = FALSE;
            continue;
        }

        hwVolume = Sound_DecibelToHwVolume_Arm7(volume);
        timer = Sound_PitchShiftTimer_Arm7(channel->wave.timer, pitch);
        if (channel->type == VOICE_TYPE_SQUARE_WAVE) {
            timer &= VOICE_SQUARE_RELOAD_ALIGN;
        }
        pan += 64;
        if (pan < 0) {
            pan = 0;
        } else if (pan > 127) {
            pan = 127;
        }

        if (hwVolume != channel->volume) {
            channel->volume = hwVolume;
            channel->sync |= VOICE_DIRTY_VOLUME;
        }
        if (timer != channel->timer) {
            channel->timer = timer;
            channel->sync |= VOICE_DIRTY_TIMER;
        }
        if (pan != channel->pan) {
            channel->pan = pan;
            channel->sync |= VOICE_DIRTY_PAN;
        }
    }
}

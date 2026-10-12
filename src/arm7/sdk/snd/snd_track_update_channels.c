#include "sdk/snd.h"

void func_arm7_037ff7e8(SndExChannel *channel);

#define TRACK_PAN_SPAN_UNSCALED 127

/*
 * Signed reads of SndTrack fields the header declares unsigned; the original
 * loads them with ldrsb/ldrsh. transpose holds the track's fader, unk_09 its
 * extra pan and unk_0c its extra pitch.
 */
#define TRACK_S8(field) (*(s8 *)&track->field)
#define TRACK_S16(field) (*(s16 *)&track->field)

void SndTrack_UpdateChannels(SndTrack *track, SndPlayer *player, BOOL release)
{
    int volume;
    int fader;
    int pan;
    int pitch;
    SndExChannel *channel;

    volume = data_arm7_03807180[player->volume] + (data_arm7_03807180[track->volume] + data_arm7_03807180[track->volume2]);
    fader = TRACK_S16(transpose) + player->extFader;
    /* bend -128..127 scaled to bendRange semitones of 64 pitch units each */
    pitch = TRACK_S8(pitchBend);
    pitch *= track->bendRange << 6;
    pitch = (pitch >> 7) + TRACK_S16(unk_0c);
    pan = TRACK_S8(pan);
    if (track->panRange != TRACK_PAN_SPAN_UNSCALED) {
        pan = (pan * track->panRange + 64) >> 7;
    }
    pan += TRACK_S8(unk_09);
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
            channel->unk_22 = 1;
            func_arm7_037ff7e8(channel);
        }
    }
}

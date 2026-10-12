#include "sdk/snd.h"

BOOL SndArm7_ReadInstData(SoundBank *bank, int programNo, int key, SoundBankNote *note);
SndExChannel *SndExChannel_Alloc(u32 channelMask, int priority, BOOL strong, SndExChannelCallback callback, void *callbackArg);
BOOL SndArm7_NoteOn(SndExChannel *channel, int key, int velocity, s32 length, SoundBank *bank, SoundBankNote *note);
void func_arm7_037ff68c(SndExChannel *channel);
void SndExChannel_SetAttack(SndExChannel *channel, int attack);
int func_arm7_037ff818(SndExChannel *channel, int decay);
void func_arm7_037ff810(SndExChannel *channel, int sustain);
int func_arm7_037ff7f4(SndExChannel *channel, int release);
void func_arm7_03800d8c(SndExChannel *channel, int status, void *track);

/* Channels each kind of note can play on. */
#define VOICE_SAMPLE_CAPABLE_MASK 0xFFFF
#define VOICE_SQUARE_CAPABLE_MASK 0x3F00
#define VOICE_NOISE_CAPABLE_MASK 0xC000

#define SEQ_ENV_FROM_INSTRUMENT 0xFF

void SndTrack_NoteOn(SndTrack *track, SndPlayer *player, int key, int velocity, s32 length)
{
    SndExChannel *channel = NULL;
    SoundBankNote note;
    u32 allowed;

    if (track->tie) {
        channel = track->channels;
        if (channel != NULL) {
            channel->key = key;
            channel->velocity = velocity;
        }
    }
    if (channel == NULL) {
        if (!SndArm7_ReadInstData((SoundBank *)player->bank, track->program, key, &note)) {
            return;
        }
        switch (note.type) {
        case SOUND_NOTE_WAVE:
        case SOUND_NOTE_DIRECT_WAVE:
            allowed = VOICE_SAMPLE_CAPABLE_MASK;
            break;
        case SOUND_NOTE_PSG:
            allowed = VOICE_SQUARE_CAPABLE_MASK;
            break;
        case SOUND_NOTE_NOISE:
            allowed = VOICE_NOISE_CAPABLE_MASK;
            break;
        default:
            return;
        }
        allowed &= track->channelMask;
        channel = SndExChannel_Alloc(allowed, player->priority + track->priority, track->channelMaskSet, func_arm7_03800d8c, track);
        if (channel == NULL) {
            return;
        }
        if (!SndArm7_NoteOn(channel, key, velocity, track->tie ? -1 : length, (SoundBank *)player->bank, &note)) {
            channel->unk_22 = 0;
            func_arm7_037ff68c(channel);
            return;
        }
        channel->next = track->channels;
        track->channels = channel;
    }
    if (track->attack != SEQ_ENV_FROM_INSTRUMENT) {
        SndExChannel_SetAttack(channel, track->attack);
    }
    if (track->decay != SEQ_ENV_FROM_INSTRUMENT) {
        func_arm7_037ff818(channel, track->decay);
    }
    if (track->sustain != SEQ_ENV_FROM_INSTRUMENT) {
        func_arm7_037ff810(channel, track->sustain);
    }
    if (track->release != SEQ_ENV_FROM_INSTRUMENT) {
        func_arm7_037ff7f4(channel, track->release);
    }
    channel->sweepPitch = *(s16 *)&track->sweepPitch; /* the field is declared u16; the original loads it signed */
    if (track->portamento) {
        /* glide from the previous key: 64 sweep units per semitone */
        channel->sweepPitch += (s16)((track->portamentoKey - key) << 6);
    }
    if (track->portamentoTime == 0) {
        channel->sweepLength = length;
        channel->autoSweep = FALSE;
    } else {
        int time = track->portamentoTime * track->portamentoTime;
        int distance = channel->sweepPitch < 0 ? -channel->sweepPitch : channel->sweepPitch;
        channel->sweepLength = (time * distance) >> 11;
    }
    channel->sweepCounter = 0;
}

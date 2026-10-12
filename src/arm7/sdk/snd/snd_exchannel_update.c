#include "sdk/snd.h"
#include "sdk/snd_hw.h"

void SndHw_StopChannel(int channel, int hold);
void SndHw_SetupPcm(int channel, u32 source, int format, int repeatMode, int loopStart, int length, int volume, int volumeShift, int timer, int pan);
void SndHw_SetupPsg(int channel, int duty, int volume, int volumeShift, int timer, int pan);
void SndHw_SetupNoise(int channel, int volume, int volumeShift, int timer, int pan);
void func_arm7_037fe848(int channel, int timer);
void SndHw_SetChannelVolume(int channel, int volume, int volumeShift);
void SndHw_SetChannelPan(int channel, int pan);

/* Repeat modes of the channel control register. */
#define AUDIO_CH_MODE_LOOPING 1
#define AUDIO_CH_MODE_SINGLE_PASS 2

/* Writes what changed in each channel since the last update to the hardware, then starts the new channels together. */
void SndExChannel_Update(void)
{
    SndExChannel *channel;
    int i;

    for (i = 0; i < AUDIO_CH_TOTAL; i++) {
        channel = &data_arm7_03808090.channels[i];
        if (channel->unk_03_3 == 0) {
            continue;
        }
        if (channel->unk_03_3 & VOICE_DIRTY_STOP) {
            SndHw_StopChannel(i, 0);
        }
        if (channel->unk_03_3 & VOICE_DIRTY_START) {
            switch (channel->type) {
            case VOICE_TYPE_SAMPLED:
                SndHw_SetupPcm(i, (u32)channel->data, channel->wave.format, channel->wave.loop ? AUDIO_CH_MODE_LOOPING : AUDIO_CH_MODE_SINGLE_PASS,
                               channel->wave.loopStart, channel->wave.loopLength, channel->volume & 0xFF, channel->volume >> 8, channel->timer, channel->pan);
                break;
            case VOICE_TYPE_SQUARE_WAVE:
                SndHw_SetupPsg(i, (int)channel->data, channel->volume & 0xFF, channel->volume >> 8, channel->timer, channel->pan);
                break;
            case VOICE_TYPE_NOISE:
                SndHw_SetupNoise(i, channel->volume & 0xFF, channel->volume >> 8, channel->timer, channel->pan);
                break;
            }
        } else {
            if (channel->unk_03_3 & VOICE_DIRTY_TIMER) {
                func_arm7_037fe848(i, channel->timer);
            }
            if (channel->unk_03_3 & VOICE_DIRTY_VOLUME) {
                SndHw_SetChannelVolume(i, channel->volume & 0xFF, channel->volume >> 8);
            }
            if (channel->unk_03_3 & VOICE_DIRTY_PAN) {
                SndHw_SetChannelPan(i, channel->pan);
            }
        }
    }
    for (i = 0; i < AUDIO_CH_TOTAL; i++) {
        channel = &data_arm7_03808090.channels[i];
        if (channel->unk_03_3 == 0) {
            continue;
        }
        if (channel->unk_03_3 & VOICE_DIRTY_START) {
            AUDIO_CH_REG_BLOCKS[i].flags |= AUDIO_CH_PLAYING_FLAG;
        }
        channel->unk_03_3 = 0;
    }
}

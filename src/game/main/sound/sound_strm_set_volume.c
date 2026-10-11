#include "types.h"
#include "game/sound_strm.h"

extern SoundStrmChannel data_022a8978[SOUND_STRM_CHANNEL_COUNT];   /* ring buffer of each hardware channel */

void func_0210fc88(u32 channelMask, int volume, int shift);
int SND_CalcChannelVolume(int volume);

/* Sets the volume of the stream and sends it to every channel, each with its
 * own offset. */
void SoundStrm_SetVolume(SoundStrm *strm, int volume)
{
    int channel;
    int i;
    int hwVolume;

    strm->volume = volume;
    for (i = 0; i < strm->channelCount; i++) {
        channel = strm->channels[i];
        hwVolume = SND_CalcChannelVolume(strm->volume + data_022a8978[channel].volume);
        func_0210fc88(1 << channel, hwVolume & 0xff, hwVolume >> 8);
    }
}

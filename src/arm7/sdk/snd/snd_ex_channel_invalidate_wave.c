#include "sdk/snd.h"
#include "sdk/snd_hw.h"

/* Stops every PCM channel playing a sample that lies in [start, end], before that memory is reused. */
void SndExChannel_InvalidateWave(const void *start, const void *end)
{
    u8 i;

    for (i = 0; i < AUDIO_CH_TOTAL; i++) {
        SndExChannel *channel = &data_arm7_03808090.channels[i];

        if (channel->active && channel->type == VOICE_TYPE_SAMPLED && start <= channel->data && channel->data <= end) {
            channel->start = FALSE;
            SndHw_StopChannel(i, 0);
        }
    }
}

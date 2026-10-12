#include "sdk/snd_seq.h"

SndTrack *SndPlayer_GetTrack(SndPlayer *player, int trackNo);

/* Stores value (size bytes: 1, 2 or 4) at a byte offset of each selected track; bit i of trackMask selects track i. */
void SndArm7_SetTrackParam(int playerNo, u32 trackMask, int offset, u32 value, int size)
{
    SndPlayer *player = &data_arm7_038085d0[playerNo];
    SndTrack *track;
    int i;

    for (i = 0; i < SND_TRACK_COUNT && trackMask != 0; i++, trackMask >>= 1) {
        if (trackMask & 1) {
            track = SndPlayer_GetTrack(player, i);
            if (track != NULL) {
                switch (size) {
                case 1:
                    *(u8 *)((u8 *)track + offset) = value;
                    break;
                case 2:
                    *(u16 *)((u8 *)track + offset) = value;
                    break;
                case 4:
                    *(u32 *)((u8 *)track + offset) = value;
                    break;
                }
            }
        }
    }
}

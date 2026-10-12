#include "sdk/snd.h"
#include "sdk/snd_seq.h"

void func_arm7_03800df4(SndPlayer *player);
void SndPlayer_Setup(SndPlayer *player, u32 bank);
int SndTrack_Alloc(void);
void SndTrack_Init(SndTrack *track);
void func_arm7_038010e4(SndTrack *track, int base, int offset); /* the definition types base as int */
void SndSeq_FillCache(u8 *address);
u8 SndSeq_ReadByte(u8 *address);
u16 SndTrack_ReadU16(SndTrack *track);

/* First byte of a sequence with more than one track; a 16-bit mask of the tracks to allocate follows. */
#define SEQ_CMD_ALLOC_TRACKS 0xFE

void SndArm7_PrepareSeq(int playerNo, u8 *seqBase, u32 seqOffset, u32 bank)
{
    SndPlayer *player = &data_arm7_038085d0[playerNo];
    int trackNo;
    SndTrack *track;
    int i;
    u16 trackMask;
    u8 cmd;

    if (player->active) {
        func_arm7_03800df4(player);
    }
    SndPlayer_Setup(player, bank);
    trackNo = SndTrack_Alloc();
    if (trackNo < 0) {
        return;
    }
    track = &data_arm7_03808810[trackNo];
    SndTrack_Init(track);
    func_arm7_038010e4(track, (int)seqBase, seqOffset);
    player->tracks[0] = trackNo;
    SndSeq_FillCache(track->cursor);
    cmd = SndSeq_ReadByte(track->cursor);
    track->cursor++;
    if (cmd != SEQ_CMD_ALLOC_TRACKS) {
        track->cursor--;
    } else {
        trackMask = SndTrack_ReadU16(track);
        for (trackMask >>= 1, i = 1; trackMask != 0; i++, trackMask >>= 1) {
            if (trackMask & 1) {
                trackNo = SndTrack_Alloc();
                if (trackNo < 0) {
                    break;
                }
                SndTrack_Init(&data_arm7_03808810[trackNo]);
                player->tracks[i] = trackNo;
            }
        }
    }
    player->active = TRUE;
    if (data_arm7_0380808c != NULL) {
        data_arm7_0380808c->playerStatus |= 1 << playerNo;
    }
}

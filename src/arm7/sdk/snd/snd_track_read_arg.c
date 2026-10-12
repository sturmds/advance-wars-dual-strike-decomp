#include "sdk/snd_seq.h"

u16 SndDrv_NextRandom(void);
s16 *SndPlayer_GetVariablePtr(SndPlayer *player, int index);
u32 SndTrack_ReadVarLength(SndTrack *track);
u16 SndTrack_ReadU16(SndTrack *track);
u8 SndSeq_ReadByte(u8 *address);

int SndTrack_ReadArg(SndTrack *track, SndPlayer *player, int type)
{
    int value;
    s16 *variable;
    u8 index;
    int random;
    s16 max;
    s16 min;

    switch (type) {
    case SEQ_ARG_FROM_U8:
        value = SndSeq_ReadByte(track->cursor);
        track->cursor++;
        break;
    case SEQ_ARG_FROM_U16:
        value = SndTrack_ReadU16(track);
        break;
    case SEQ_ARG_FROM_VARINT:
        value = SndTrack_ReadVarLength(track);
        break;
    /* Needed to match: this case comes before SEQ_ARG_FROM_RANGE. A missing variable returns an unassigned value. */
    case SEQ_ARG_FROM_PLAYER_VAR:
        index = SndSeq_ReadByte(track->cursor);
        track->cursor++;
        variable = SndPlayer_GetVariablePtr(player, index);
        if (variable != NULL) {
            value = *variable;
        }
        break;
    case SEQ_ARG_FROM_RANGE:
        min = SndTrack_ReadU16(track);
        max = SndTrack_ReadU16(track);
        random = SndDrv_NextRandom();
        value = (random * (max - min + 1)) >> 16;
        value += min;
        break;
    }
    return value;
}

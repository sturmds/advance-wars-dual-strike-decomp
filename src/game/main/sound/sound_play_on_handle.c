#include "types.h"
#include "game/sound_handle.h"

/* NitroSystem NNS_SndArcPlayerStartSeqArc and NNS_SndArcPlayerStartSeq:
 * return TRUE if the sequence started. */
BOOL func_02107a18(SoundHandle *handle, int seqArcNo, int index);
BOOL func_02107ab0(SoundHandle *handle, int seqNo);

extern BOOL data_0218fec0;                  /* sound system ready */

/* Plays sound soundId on handle. The high 16 bits pick a sequence
 * archive and the low 16 bits a sequence in it; with the high bits 0,
 * the low bits are a plain sequence number. */
void Sound_PlayOnHandle(SoundHandle *handle, u32 soundId)
{
    u32 seqArcNo;
    u32 index;

    if (data_0218fec0 == 0) {
        return;
    }
    seqArcNo = (soundId & 0xffff0000) >> 16;
    index = soundId & 0xffff;
    if (seqArcNo != 0) {
        func_02107a18(handle, seqArcNo, index);
    } else {
        func_02107ab0(handle, index);
    }
}

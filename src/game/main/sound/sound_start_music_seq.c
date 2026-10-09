#include "types.h"
#include "game/sound_handle.h"

/* NitroSystem NNS_SndArcPlayerStartSeq: returns TRUE if the sequence started. */
BOOL func_02107ab0(SoundHandle *handle, int seqNo);

extern SoundHandle data_0218fea8;           /* music handle */
extern BOOL data_0218fec0;                  /* sound system ready */

/* Starts sound archive sequence seqNo on the music handle. */
void Sound_StartMusicSeq(int seqNo)
{
    if (data_0218fec0 == 0) {
        return;
    }
    func_02107ab0(&data_0218fea8, seqNo);
}

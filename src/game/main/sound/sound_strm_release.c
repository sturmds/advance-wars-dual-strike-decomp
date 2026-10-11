#include "types.h"
#include "game/sound_strm.h"

extern LinkList data_022a892c;             /* active streams */

void func_02104f2c(int alarm);

/* Gives back the alarm of the stream and takes it out of the active list. */
void SoundStrm_Release(SoundStrm *strm)
{
    func_02104f2c(strm->alarm);
    LinkList_Remove(&data_022a892c, strm);
    strm->flags.active = 0;
}

#include "types.h"
#include "game/sound_strm.h"

void SoundAlarm_Free(int alarm);

/* Gives back the alarm of the stream and takes it out of the active list. */
void SoundStrm_Release(SoundStrm* strm) {
  SoundAlarm_Free(strm->alarm);
  LinkList_Remove(&data_022a892c, strm);
  strm->flags.active = 0;
}

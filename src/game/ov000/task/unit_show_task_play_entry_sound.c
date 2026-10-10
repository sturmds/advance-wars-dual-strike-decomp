#include "types.h"
#include "game/Unk_ov000_022db768.h"
#include "game/unit_show_task.h"

void Sound_PlaySe(int se);
void Sound_StopSubSe(int se);

/* Plays the entry's sound. Unlike ShowEntry_PlaySound, SHOW_SOUND_SPECIAL
 * plays nothing here, and SHOW_SOUND_STOP stops sub sound effect 10 (the
 * other stops 20). */
void UnitShowTask_PlayEntrySound(UnitShowTask *task)
{
    u16 sound = data_ov000_022db768[task->entry].sound;

    if (sound == SHOW_SOUND_NONE) {
        return;
    }
    switch (sound) {
    case SHOW_SOUND_SPECIAL:
        return;
    case SHOW_SOUND_STOP:
        Sound_StopSubSe(10);
        break;
    default:
        Sound_PlaySe(sound);
        break;
    }
}

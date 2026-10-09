#include "types.h"
#include "game/Unk_ov000_022db768.h"

void Sound_PlaySe(int se);
void Sound_StopSubSe(int se);
int func_ov000_022adc10(Unk_ov000_022db768_Owner *owner);

void ShowEntry_PlaySound(Unk_ov000_022db768_Owner *owner)
{
    u16 sound = data_ov000_022db768[owner->entry].sound;

    if (sound == SHOW_SOUND_NONE) {
        return;
    }
    switch (sound) {
    case SHOW_SOUND_SPECIAL:
        func_ov000_022adc10(owner);
        break;
    case SHOW_SOUND_STOP:
        Sound_StopSubSe(20);
        break;
    default:
        Sound_PlaySe(sound);
        break;
    }
}

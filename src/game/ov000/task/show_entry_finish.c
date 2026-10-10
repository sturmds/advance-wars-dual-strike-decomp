#include "types.h"
#include "game/Unk_ov000_022db768.h"

extern u8 data_ov000_022db70c[];

s8 Task_DestroyByScript(const void *script);
int UnitShowTask_PlayStepSound(Unk_ov000_022db768_Owner *owner);

int ShowEntry_Finish(Unk_ov000_022db768_Owner *owner)
{
    if (data_ov000_022db768[owner->entry].sound != SHOW_SOUND_SPECIAL) {
        Task_DestroyByScript(data_ov000_022db70c);
        return UnitShowTask_PlayStepSound(owner);
    }
    return UnitShowTask_PlayStepSound(owner);
}

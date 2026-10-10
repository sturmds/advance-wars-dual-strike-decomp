#include "types.h"
#include "game/Unk_ov000_022db768.h"
#include "game/unit_show_task.h"

int func_ov000_022adc10(UnitShowTask *task);

void UnitShowTask_PlaySpecialSound(UnitShowTask *task)
{
    if (data_ov000_022db768[task->entry].sound == SHOW_SOUND_SPECIAL) {
        func_ov000_022adc10(task);
    }
}

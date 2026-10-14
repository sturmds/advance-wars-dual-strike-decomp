#include "types.h"
#include "game/Unk_02160abc.h"
#include "game/Unk_02160ac0.h"
#include "game/task.h"

void Ov003ExitTask_Run(void);
extern u8 data_0229e0ac[];
extern u8 data_0229e0bc[];

Unk_02160ac0* data_02160ac0 = (Unk_02160ac0*)&data_0229e0bc;
Unk_02160abc* data_02160abc = (Unk_02160abc*)&data_0229e0ac;
TaskScriptCmd gOv003ExitTaskScript[] = {
#include "data/main/gOv003ExitTaskScript.inc.c"
};

#include "types.h"
#include "game/task.h"
#include "game/Unk_ov005_0236bbac.h"

void Task_Destroy(Task* task);

extern Unk_ov005_0236bbac* data_ov005_0236bbac;

void Ov005SoundRoom_DestroyMusicTask(void) {
  Task_Destroy(data_ov005_0236bbac->musicTask);
}

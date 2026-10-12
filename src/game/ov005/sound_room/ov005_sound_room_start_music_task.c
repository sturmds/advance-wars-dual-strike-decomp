#include "types.h"
#include "game/task.h"
#include "game/Unk_ov005_0236bbac.h"

extern const TaskScriptCmd data_ov005_0236af0c[];
extern Unk_ov005_0236bbac* data_ov005_0236bbac;

void Ov005SoundRoom_StartMusicTask(void) {
  Unk_ov005_0236bbac* state = data_ov005_0236bbac;

  state->musicId = -1;
  state->playingMusicId = -1;
  state->musicKind = -1;
  state->playingMusicKind = -1;
  state->musicChanging = 0;
  state->musicPlaying = 0;
  state->musicTask = Task_Create(data_ov005_0236af0c, TASK_LIST(4));
}

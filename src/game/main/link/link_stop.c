#include "types.h"
#include "game/task.h"
#include "game/Unk_0215fe8c.h"

extern Unk_0215fe8c* data_0215fe8c;
extern const TaskScriptCmd data_0215fea0[];
extern const TaskScriptCmd data_0215feb8[];

int Task_DestroyByScript(const void* script);
void Wireless_Stop(void);

/* Leaves link mode: clears the mode and peer flags and stops the two link tasks. */
void Link_Stop(void) {
  data_0215fe8c->linkActive = 0;
  data_0215fe8c->replyOk = 0;
  data_0215fe8c->unk_0c = 0;
  Wireless_Stop();
  Task_DestroyByScript(data_0215fea0);
  Task_DestroyByScript(data_0215feb8);
}

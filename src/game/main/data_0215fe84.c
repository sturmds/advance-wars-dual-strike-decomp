#include "types.h"
#include "game/Unk_0215fe84.h"
#include "game/Unk_0215fe88.h"
#include "game/Unk_0215fe8c.h"
#include "game/task.h"

void Link_CheckAbort(void);
void Wireless_ExpireParents(void);
extern u8 data_0229c8c0[];
extern u8 data_0229c928[];
extern u8 data_0229ca68[];
void func_0207ab2c(void);

Unk_0215fe84* data_0215fe84 = (Unk_0215fe84*)&data_0229c928;
Unk_0215fe88* data_0215fe88 = (Unk_0215fe88*)&data_0229c8c0;
Unk_0215fe8c* data_0215fe8c = (Unk_0215fe8c*)&data_0229ca68;
TaskScriptCmd data_0215fe90[] = {
#include "data/main/data_0215fe90.inc.c"
};
TaskScriptCmd data_0215fea0[] = {
#include "data/main/data_0215fea0.inc.c"
};
TaskScriptCmd data_0215feb8[] = {
#include "data/main/data_0215feb8.inc.c"
};

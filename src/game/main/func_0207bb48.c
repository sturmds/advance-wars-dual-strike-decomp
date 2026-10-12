#include "types.h"
#include "game/task.h"
#include "game/Unk_0215fe8c.h"

extern Unk_0215fe8c *data_0215fe8c;
extern const TaskScriptCmd data_0215feb8[];

int Task_DestroyByScript(const void *script);
u16 Wireless_GetChildAids(void);
int Wireless_GetState(void);
void func_02077f34(int a);
void Wireless_SetEntry(int a);

/* 1 starts the data_0215feb8 task, anything else stops it. */
void func_0207bb48(int start)
{
    if (start == 1) {
        data_0215fe8c->unk_0c = 0;
        Task_Create(data_0215feb8, NULL);
    } else {
        data_0215fe8c->unk_0c = 1;
        Task_DestroyByScript(data_0215feb8);
        data_0215fe8c->unk_08 = Wireless_GetChildAids();
    }
    if (Wireless_GetState() == 4) {
        Wireless_SetEntry(start);
    } else {
        func_02077f34(start);
    }
}

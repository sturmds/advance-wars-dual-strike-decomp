#include "types.h"

typedef struct Unk_02364108_0 Unk_02364108_0;

struct Unk_02364108_0 {
    u8 pad_00[0x70];
    int unk_70;
};

void Task_Destroy(int);
extern Unk_02364108_0 *data_ov005_0236bbac;

void func_ov005_02364108(void)
{
    Task_Destroy(data_ov005_0236bbac->unk_70);
}

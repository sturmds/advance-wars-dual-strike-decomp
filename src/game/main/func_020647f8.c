#include "types.h"

typedef struct Unk_gMenuState Unk_gMenuState;

struct Unk_gMenuState {
    u8 pad_00[0x2eb];
    u8 unk_2eb;
};

extern Unk_gMenuState gMenuState;
void Task_CreateChild(void *, int, void *, int);
extern u8 data_0215d038[];

void func_020647f8(int a0)
{
    gMenuState.unk_2eb = 1;
    Task_CreateChild(data_0215d038, a0, &gMenuState, 1);
}

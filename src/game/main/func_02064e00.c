#include "types.h"

typedef struct Unk_gMenuState Unk_gMenuState;

struct Unk_gMenuState {
    u8 pad_00[0x2e6];
    u8 unk_2e6;
};

void Sound_PlaySe(int);
extern Unk_gMenuState gMenuState;
int func_02066344(void *);

int func_02064e00(int a0)
{
    Sound_PlaySe(0x10002);
    gMenuState.unk_2e6 = a0;
    return func_02066344(&gMenuState);
}

#include "types.h"

typedef struct Unk_0229fb30 Unk_0229fb30;

struct Unk_0229fb30 {
    u8 pad_00[0x45b];
    u8 unk_45b;
};

void Sound_PlaySe(int);
extern Unk_0229fb30 data_0229fb30;
int LinkMenu_DrawHelpText(void *);

int func_02086810(int a0)
{
    Sound_PlaySe(0x10002);
    data_0229fb30.unk_45b = a0;
    return LinkMenu_DrawHelpText(&data_0229fb30);
}

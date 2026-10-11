#include "types.h"
#include "game/Unk_0229fb30.h"

void func_0207e5d0(void);
void LinkMenu_PushBack(void);

void func_0207e2a8(void)
{
    switch (data_0229fb30.unk_630) {
    case 1:
        func_0207e5d0();
        break;
    case 2:
    case 3:
        LinkMenu_PushBack();
        break;
    }
}

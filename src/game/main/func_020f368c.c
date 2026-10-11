#include "types.h"
#include "game/screen_other_map.h"

void func_020f35f4(void);
void func_020f3604(int a, int b);
void func_020f3660(void);
void func_020f4990(int a);
int Scene_GetKind(void);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Picks what to start from the value behind Scene_GetKind: func_020f3660 in a
 * two-map battle whose other screen has unk_30 0, else by value 1, 2 or other. */
void func_020f368c(void)
{
    if (GAME->secondMapId != 0 && OtherMap_GetUnk30() == 0) {
        func_020f3660();
        return;
    }
    switch (Scene_GetKind()) {
    case 2:
        func_020f3604(9, 0x8ca0);
        break;
    case 1:
        func_020f35f4();
        break;
    default:
        func_020f4990(5);
        break;
    }
}

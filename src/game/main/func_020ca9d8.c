#include "types.h"
#include "game/Unk_02080440.h"
#include "game/screen_other_map.h"

void func_020cab50(void);
int SceneMode_IsCurrent(int a0);
#define GAME (data_027c027c->unk_48[data_027c0284])
#define RUN_ON_SCREEN(screen) \
    do { \
        saved0280 = data_027c0280; \
        Screen_SetCurrentGfx(screen); \
        saved027c = data_027c027c; \
        Screen_SetCurrent(screen); \
        func_020cab50(); \
        Screen_SetCurrent(saved027c); \
        Screen_SetCurrentGfx(saved0280); \
    } while (0)

void func_020ca9d8(void)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    func_020cab50();
    if (GAME->secondMapId == 0) {
        return;
    }
    if (OtherMap_GetUnk30() != 0) {
        return;
    }
    if (data_027c027c == data_027c0288.unk_00) {
        if (SceneMode_IsCurrent(4) == 0) {
            return;
        }
        RUN_ON_SCREEN(data_027c0288.unk_04);
    } else {
        RUN_ON_SCREEN(data_027c0288.unk_00);
    }
}

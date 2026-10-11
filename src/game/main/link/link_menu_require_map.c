#include "types.h"
#include "game/task.h"

int LinkMenu_PlayerHasMap(int player);
void func_0207c044(Task *task, u32 textId);
int Task_JumpToLabel(Task *task, int label);

/* Script step. When neither of the two link players has a map, shows text 0x48000003 through
 * func_0207c044 and jumps to label 255. */
void LinkMenu_RequireMap(Task *task)
{
    int anyMap = 0;
    int player;

    for (player = 0; player < 2; player++) {
        if (LinkMenu_PlayerHasMap(player) == 1) {
            anyMap = 1;
        }
    }
    if (anyMap != 0) {
        return;
    }
    func_0207c044(task, 0x48000003);
    Task_JumpToLabel(task, 255);
}

#include "types.h"
#include "game/task.h"
#include "game/Unk_02290118.h"
#include "game/screen_other_map.h"

extern const TaskScriptCmd data_021670d4[];

int Campaign_FindFreeProfile(void);
void func_02103ca8(void);
void func_020e6d24(void);
void func_02103c84(void);
void Screen_SetCurrentBoth(void *state);
void ForceRank_LoadSets(int mode);

/* Same as Campaign_StartHard with hardMode 0. */
void Campaign_StartNormal(void)
{
    int slot = Campaign_FindFreeProfile();
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    func_02103ca8();
    saved0280 = data_027c0280;
    saved027c = data_027c027c;
    Screen_SetCurrentBoth(data_027c0288.unk_00);
    data_027c027c->unk_48[data_027c0284]->gameMode = GAME_MODE_CAMPAIGN;
    Screen_SetCurrentBoth(data_027c0288.unk_04);
    data_027c027c->unk_48[data_027c0284]->gameMode = GAME_MODE_CAMPAIGN;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    data_02290118.unk_01 = slot;
    data_02290118.unk_00 = 0;
    data_02290118.hardMode = 0;
    data_02290118.unk_04 = 0;
    data_02290118.unk_06 = 0;
    data_02290118.unk_05 = 0;
    data_02290118.unk_08 = 0;
    data_02290118.unk_0a = 0;
    data_02290118.unk_0c = 0;
    data_02290118.unk_0e = 0;
    func_020e6d24();
    ForceRank_LoadSets(0);
    func_02103c84();
    Task_Create(data_021670d4, TASK_LIST(3));
}

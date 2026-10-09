#include "types.h"
#include "game/force_rank_screen.h"
#include "game/menu_button.h"

void Sound_PlaySe(int id);
void func_0203fc28(void);
void Mem_Free(void *res);
void *Task_Create(const void *script, void *parentOrList);

extern const u32 data_0215a86c[];

/* B button: leaves the screen. func_0203fc28 runs before the work area is
 * freed (it calls ForceRank_SaveSets). */
void ForceRankScreen_Close(void)
{
    Sound_PlaySe(0x10009);
    MenuButtons_DisableTouch();
    MenuButtons_RemoveSelectable();
    func_0203fc28();
    Mem_Free(gForceRankScreen);
    Task_Create(data_0215a86c, (void *)3);
}

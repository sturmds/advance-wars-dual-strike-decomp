#include "types.h"
#include "game/menu_button.h"
#include "game/Unk_02160ac0.h"

void HelpBand_Stop(void);
void CombatModeCoPortrait_Destroy(void);
void Wallpaper_DestroyTasks(void);
void Screen1_DestroyTextTasks(void);
int CombatCo_NumberFromId(int coId);
void Mem_Free(void *ptr);

extern u8 gCombatModeCos[];             /* CO ids of the seven slots */
extern u8 *data_0229e144;

/* Tears down the Combat mode menu. The selected slot becomes whatever
 * CombatCo_NumberFromId makes of its CO, and the one-byte row memory is freed. */
void CombatModeMenu_Close(void)
{
    HelpBand_Stop();
    MenuButtons_RemoveAll();
    CombatModeCoPortrait_Destroy();
    Wallpaper_DestroyTasks();
    Screen1_DestroyTextTasks();
    data_02160ac0->co = CombatCo_NumberFromId(gCombatModeCos[data_02160ac0->co]);
    Mem_Free(data_0229e144);
}

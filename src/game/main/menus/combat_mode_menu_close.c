#include "types.h"
#include "game/menu_button.h"
#include "game/Unk_02160ac0.h"

void HelpBand_Stop(void);
void func_02088c38(void);
void Wallpaper_DestroyTasks(void);
void Screen1_DestroyTextTasks(void);
int CombatCo_NumberFromId(int coId);
void Mem_Free(void *ptr);

extern u8 data_021613a0[];             /* CO ids of the seven slots */
extern u8 *data_0229e144;

/* Tears down the Combat mode menu. The selected slot becomes whatever
 * CombatCo_NumberFromId makes of its CO, and the one-byte row memory is freed. */
void CombatModeMenu_Close(void)
{
    HelpBand_Stop();
    MenuButtons_RemoveAll();
    func_02088c38();
    Wallpaper_DestroyTasks();
    Screen1_DestroyTextTasks();
    data_02160ac0->unk_19 = CombatCo_NumberFromId(data_021613a0[data_02160ac0->unk_19]);
    Mem_Free(data_0229e144);
}

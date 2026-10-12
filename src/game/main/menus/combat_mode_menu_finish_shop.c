#include "types.h"
#include "game/menu_button.h"
#include "game/Unk_02160ac0.h"

void Sound_PlaySe(u32 soundId);
void func_0208ac7c(void);
void func_020b9890(void);
void Task_ResumeByScript(void* script);
extern u8 data_02161570[];

/* START in the Combat mode shop: leaves it like func_02089260 does, with
 * another sound and func_020b9890 (sets the byte at +13 of the current
 * player) instead of func_020b9864 (clears it). Does nothing while no
 * cell is in use. */
void CombatModeMenu_FinishShop(void) {
  if (data_02160ac0->cellCount == 0) {
    return;
  }
  Sound_PlaySe(0x10006);
  MenuButtons_RemoveSelectable();
  func_0208ac7c();
  func_020b9890();
  Task_ResumeByScript(data_02161570);
}

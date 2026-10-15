#include "types.h"
#include "game/Unk_02160ac0.h"
#include "game/menu_button.h"
#include "game/menu_state.h"

void HelpBand_Stop(void);
void Wallpaper_DestroyTasks(void);
void Screen1_DestroyTextTasks(void);
int CombatCo_NumberFromId(int coId);

extern u8 data_02160adc[];
extern u8 data_02160ae0[];

/* Tears down the Combat setup screen and copies its choices into data_02160ac0: the map file
 * of the picked button, each player's row, and each row's CO id (through CombatCo_NumberFromId). Then
 * unk_83 + 1 is split into unk_84 (set when it is above 2) and unk_82 (reduced by 2 then). */
void func_02087cd4(void) {
  int i;

  HelpBand_Stop();
  MenuButtons_RemoveAll();
  Wallpaper_DestroyTasks();
  Screen1_DestroyTextTasks();
  data_02160ac0->unk_7d = data_02160adc[data_02160ac0->unk_7e];
  for (i = 0; i < (int)ARRAY_COUNT(gMenuState.combat.players); i++) {
    data_02160ac0->unk_70[i] = gMenuState.combat.players[i].row;
  }
  for (i = 1; i < COMBAT_ROW_COUNT + 1; i++) {
    LINK_ROWS[i - 1].unk_19 =
        CombatCo_NumberFromId(data_02160ae0[LINK_ROWS[i - 1].unk_19]);
  }
  data_02160ac0->unk_82 = data_02160ac0->unk_83 + 1;
  data_02160ac0->unk_84 = 0;
  if (data_02160ac0->unk_82 > 2) {
    data_02160ac0->unk_84 = 1;
    data_02160ac0->unk_82 -= 2;
  }
}

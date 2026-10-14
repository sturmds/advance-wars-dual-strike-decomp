#include "types.h"
#include "game/Unk_022a7fac.h"

int BattleSave_GetRecordId(int a, int b);
void func_02017cf8(int a);
void func_0210322c(void);

extern Unk_022a7fac data_022a7fac;

/* Starts the War Room. When data_022a7fac.byRecord[5] is set, starts the script
 * slot with the number BattleSave_GetRecordId(1, 0) gives instead; what the flag
 * means is unknown. */
void MainMenu_StartWarRoom(void) {
  if (data_022a7fac.byRecord[5] != 0) {
    func_02017cf8(BattleSave_GetRecordId(1, 0));
  } else {
    func_0210322c();
  }
}

#include "types.h"
#include "game/Unk_022a7fac.h"

void func_02017cf8(int a);
void Versus_Start(void);

extern Unk_022a7fac data_022a7fac;

/* Starts Versus mode through Versus_Start. When data_022a7fac.unk_06 is set,
 * starts script slot 6 instead; what the flag means is unknown. */
void MainMenu_StartVersus(void) {
  if (data_022a7fac.unk_06 != 0) {
    func_02017cf8(6);
  } else {
    Versus_Start();
  }
}

#include "types.h"
#include "game/Unk_022a7fac.h"

void func_02017cf8(int a);
void func_02103544(void);

extern Unk_022a7fac data_022a7fac;

/* Starts Versus mode through func_02103544. When data_022a7fac.unk_06 is set,
 * starts script slot 6 instead; what the flag means is unknown. */
void MainMenu_StartVersus(void)
{
    if (data_022a7fac.unk_06 != 0) {
        func_02017cf8(6);
    } else {
        func_02103544();
    }
}

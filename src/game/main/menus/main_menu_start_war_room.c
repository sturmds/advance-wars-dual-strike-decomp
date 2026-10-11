#include "types.h"
#include "game/Unk_022a7fac.h"

int func_02019330(int a, int b);
void func_02017cf8(int a);
void func_0210322c(void);

extern Unk_022a7fac data_022a7fac;

/* Starts the War Room. When data_022a7fac.unk_05 is set, starts the script
 * slot with the number func_02019330(1, 0) gives instead; what the flag
 * means is unknown. */
void MainMenu_StartWarRoom(void)
{
    if (data_022a7fac.unk_05 != 0) {
        func_02017cf8(func_02019330(1, 0));
    } else {
        func_0210322c();
    }
}

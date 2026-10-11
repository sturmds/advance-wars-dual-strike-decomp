#include "types.h"
#include "game/main_menu.h"

void func_020313cc(u8 value);
void func_020313f4(void);

/* Resets the main menu state: clears the cursors and the byte at 0x11. */
void MainMenu_Init(void)
{
    data_021599d0->unk_11 = 0;
    func_020313cc(0);
    func_020313f4();
}

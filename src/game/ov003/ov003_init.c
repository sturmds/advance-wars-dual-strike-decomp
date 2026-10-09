#include "types.h"
#include "game/screen_other_map.h"

void func_01ff9c28(void (*callback)(void));
void func_01ff9c48(void (*callback)(void));
void func_ov003_023517e0(void);
void func_ov003_023511ac(void);
void Ov003_InitState(void);
void func_ov003_02364854(void);
void func_020d2ef8(void);
void func_ov003_02351894(void);
void Screen_SetCurrentBoth(void *state);

/* Sets up the overlay on the first screen, then installs
 * func_020d2ef8 and func_ov003_02351894 as the two main loop callbacks. */
void Ov003_Init(void)
{
    Unk_027c0280 *saved0280 = data_027c0280;
    Unk_02080440 *saved027c;

    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    func_01ff9c28(NULL);
    func_01ff9c48(NULL);
    func_ov003_023517e0();
    func_ov003_023511ac();
    Ov003_InitState();
    func_ov003_02364854();
    Screen_SetCurrentBoth(data_027c0288.unk_00);
    func_01ff9c28(func_020d2ef8);
    func_01ff9c48(func_ov003_02351894);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
}

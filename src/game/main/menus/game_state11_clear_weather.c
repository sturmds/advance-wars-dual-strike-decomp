#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

void Weather_StartEffect(int weather);
void func_ov000_022af520(void);

/* Handler for game state 11. Clears the weather effect, calls
 * func_ov000_022af520, then switches to state 13. */
void Game_State11_ClearWeather(void)
{
    Weather_StartEffect(0);
    func_ov000_022af520();
    STATE->unk_18 = 13;
}

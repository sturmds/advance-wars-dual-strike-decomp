#include "types.h"
#include "game/Unk_02080440.h"
#include "game/screen_other_map.h"

void func_0200a5e0(int screen);
void Screens_SelectConfigs(void);
void func_020c4018(void);
void CurrentPlayer_LoadPalettes(void);
int SceneMode_IsCurrent(int a0);

extern Unk_02080440_48 data_02183b08;

#define GAME  (data_027c027c->unk_48[data_027c0284])
#define HOOKS (data_027c027c->unk_50[data_027c0284])

/* Like func_020c9778, but keeps the current screen index (0 when the current
 * map state is data_02183b08) and does not call func_020c0734. On screen 0's
 * map state it runs the unk_58 hook with the weather and tileset,
 * func_020c4018 and CurrentPlayer_LoadPalettes; on screen 1 too when SceneMode_IsCurrent(4) is
 * true. No direct callers. */
void func_020c98b4(void)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    func_0200a5e0(GAME == &data_02183b08 ? MAP_STATE_FIRST : MAP_STATE_SECOND);
    Screens_SelectConfigs();

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    if (HOOKS->unk_58 != NULL) {
        HOOKS->unk_58(GAME->unk_1c4->weather, GAME->unk_1c4->tileset);
    }
    func_020c4018();
    CurrentPlayer_LoadPalettes();
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    if (SceneMode_IsCurrent(4) == 0) {
        return;
    }
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    if (HOOKS->unk_58 != NULL) {
        HOOKS->unk_58(GAME->unk_1c4->weather, GAME->unk_1c4->tileset);
    }
    func_020c4018();
    CurrentPlayer_LoadPalettes();
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
}

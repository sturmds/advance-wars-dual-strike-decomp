#include "types.h"
#include "game/screen_other_map.h"

void Screen_SetCurrentBoth(void *state);
void MapRules_SetDefaults(void);
void func_020e8c28(void); /* gLinkPlay = 0 */

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Puts the game settings back to their boot defaults: Versus mode on both
 * screen states, map 1, the map rules reset, link play off. */
void Game_ResetSettings(void)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    STATE->musicReady = 1;
    saved0280 = data_027c0280;
    saved027c = data_027c027c;
    Screen_SetCurrentBoth((&data_027c0288.unk_00)[0]);
    STATE->gameMode = GAME_MODE_VERSUS;
    Screen_SetCurrentBoth((&data_027c0288.unk_00)[1]);
    STATE->gameMode = GAME_MODE_VERSUS;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    STATE->unk_59 = 0;
    STATE->unk_1c4->mapId = 1;
    STATE->unk_82 = 1;
    MapRules_SetDefaults();
    func_020e8c28();
}

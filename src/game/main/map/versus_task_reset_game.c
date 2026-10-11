#include "types.h"
#include "game/Unk_02080440.h"

void Game_ResetSettings(void);
u32 Link_IsActive(void);
void Map_SetupPlayerCos(void);
void MapSelect_ResetTables(void);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern int gLinkPlay;

/* First step of the data_021673dc task script, which Versus_Start starts
 * after it sets gameMode 2 (Versus, going by the other mode readers).
 * Game_ResetSettings puts the game settings back to their boot defaults (gameMode
 * 2 again) and clears the link flag gLinkPlay; musicReady survives the
 * reset. In link play (Link_IsActive returns 1) the flag is set again and
 * MapSelect_ResetTables clears the map select tables. */
void VersusTask_ResetGame(void)
{
    u8 musicReady = data_027c027c->unk_48[data_027c0284]->musicReady;

    Game_ResetSettings();
    if (Link_IsActive() == 1) {
        gLinkPlay = 1;
    }
    data_027c027c->unk_48[data_027c0284]->musicReady = musicReady;
    Map_SetupPlayerCos();
    if (Link_IsActive() == 1) {
        MapSelect_ResetTables();
    }
}

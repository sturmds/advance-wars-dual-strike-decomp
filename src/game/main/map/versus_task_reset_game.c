#include "types.h"
#include "game/Unk_02080440.h"

void Game_ResetSettings(void);
u32 func_0207bd80(void);
void Map_SetupPlayerCos(void);
void func_02064774(void);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern int gLinkPlay;

/* First step of the data_021673dc task script, which Versus_Start starts
 * after it sets gameMode 2 (Versus, going by the other mode readers).
 * Game_ResetSettings puts the game settings back to their boot defaults (gameMode
 * 2 again) and clears the link flag gLinkPlay; musicReady survives the
 * reset. In link play (func_0207bd80 returns 1) the flag is set again and
 * func_02064774 clears the link buffers in data_0229c768. */
void VersusTask_ResetGame(void)
{
    u8 musicReady = data_027c027c->unk_48[data_027c0284]->musicReady;

    Game_ResetSettings();
    if (func_0207bd80() == 1) {
        gLinkPlay = 1;
    }
    data_027c027c->unk_48[data_027c0284]->musicReady = musicReady;
    Map_SetupPlayerCos();
    if (func_0207bd80() == 1) {
        func_02064774();
    }
}

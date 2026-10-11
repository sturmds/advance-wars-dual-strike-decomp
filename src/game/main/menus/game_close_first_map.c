#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u32 gLinkPlay;

#define STATE (data_027c027c->unk_48[data_027c0284])

void func_020c0704(void);
void func_020d6a04(void);
void func_02080708(void);
int func_0207bd80(void);
void Link_SendStopRequest(void);
void SpriteTask_DestroyAllFirstScreen(void);

/* Ends the first map's game: sets up the map screen again, drops the sprite
 * tasks of the first screen when gLinkPlay is set, and returns to
 * state 0. */
void Game_CloseFirstMap(void)
{
    func_020c0704();
    func_020d6a04();
    STATE->unk_18 = 0;
    func_02080708();
    if (func_0207bd80() == 1) {
        Link_SendStopRequest();
    }
    if (gLinkPlay == 0) {
        return;
    }
    SpriteTask_DestroyAllFirstScreen();
}

#include "types.h"
#include "game/menu_state.h"
#include "game/player_info_task.h"

extern MenuState gMenuState;

/* Sets up the sprite task of the player info screen for the shown CO. */
void PlayerInfo_SpriteTaskInit(PlayerInfoSpriteTask *task)
{
    task->slideStartX = 0;
    task->slideX = 0;
    task->coButton = (gMenuState.unk_0206d67c.player - 1) * 2 + gMenuState.unk_0206d67c.slot;
    task->prevCoButton = task->coButton;
    task->unk_2a = 8;
    task->unk_2c = 0;
    task->unk_34 = 1;
}

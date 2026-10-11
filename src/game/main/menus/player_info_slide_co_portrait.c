#include "types.h"
#include "game/menu_state.h"
#include "game/player_info_task.h"
#include "game/screen_other_map.h"

extern MenuState gMenuState;

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Sprite_QueueCoPortrait(int x, int y, int arg2, int arg3, int list, BOOL flip);
void Co_LoadArmyGraphicsAndPalette(int coId, int firstTile, int paletteSlot, int army);

#define MENU (gMenuState.unk_0206d67c)

/* MENU.coShown[player][slot]. Needed to match: the original adds the offset
 * of coShown to gMenuState's address at run time. */
#define SHOWN_CO_OFFSET ((u32) & ((MenuState *)0)->unk_0206d67c.coShown)
#define SHOWN_CO(player, slot) (((u8 *)&gMenuState + (player) * 2 + (slot))[SHOWN_CO_OFFSET])

/* Per-frame callback of the player info sprite task, drawn on the screen of data_027c0288.unk_04.
 * When the selected CO changes, the shown portrait slides 144 pixels out over
 * 8 frames (slidingIn 0), waits 4 more frames, loads the new CO's graphics and
 * slides back in (slidingIn 1). Picking the shown CO again while it slides out
 * turns it around. slideFrame counts the slide's frames, waitFrames the wait. */
void PlayerInfo_SlideCoPortrait(PlayerInfoSpriteTask *task)
{
    Unk_027c0280 *saved0280;
    int player;

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    if (task->coButton != (MENU.player - 1) * 2 + MENU.slot) {
        task->coButton = (MENU.player - 1) * 2 + MENU.slot;
        if (task->waitFrames == 0) {
            if (task->coButton == task->prevCoButton) {
                if (task->slidingIn == 0) {
                    task->slideStartX = task->slideX;
                    task->slideFrame = 0;
                    task->slidingIn = 1;
                }
            } else if (task->slidingIn == 1) {
                task->slideStartX = task->slideX;
                task->slideFrame = 0;
                task->slidingIn = 0;
            }
        }
        task->waitFrames = 0;
    }
    if (task->slidingIn == 0) {
        task->slideX = task->slideStartX + Math_Ease(1, 0, 144, task->slideFrame, 8);
        if (task->slideX > 144) {
            task->slideX = 144;
        }
    } else {
        task->slideX = Math_Ease(4, task->slideStartX, 0, task->slideFrame, 8);
    }
    Sprite_QueueCoPortrait((256 - (72 - task->slideX)) & 0x1ff, 160, 0, 108, 0, FALSE);
    if (task->slideFrame == 8) {
        if (task->slidingIn == 0) {
            if (task->waitFrames < 4) {
                task->waitFrames++;
            } else {
                task->prevCoButton = task->coButton;
                player = task->coButton >> 1;
                Co_LoadArmyGraphicsAndPalette(SHOWN_CO(player, task->coButton & 1), 108, 0, player + 1);
                task->slideStartX = task->slideX;
                task->slidingIn = 1;
                task->slideFrame = 0;
                task->waitFrames = 0;
            }
        }
    } else {
        task->slideFrame++;
    }
    Screen_SetCurrentGfx(saved0280);
}

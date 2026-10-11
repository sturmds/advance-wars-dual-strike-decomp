#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

void Sprite_QueueAt(int list, int x, int y, void *data, int attr, int tile);
void Task_ResumeScript(Task *task);
void SubScreen_QueueTwoSprites(int x);
void func_020773a0(int x);

extern u8 data_027c0060;

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16 *)&(task)->unk_4c)

/* Task update: for 8 frames, draws the sprite that
 * SubScreenSprite_ZoomInUpdate zooms in, at a different spot each frame
 * within 2 pixels of (222, 144) on the sub screen, so it shakes; then
 * resumes the task's script. func_020773a0 and SubScreen_QueueTwoSprites draw the rest
 * of the scene. */
void SubScreenSprite_ShakeUpdate(Task *task)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    FRAME(task)++;
    func_020773a0(0);
    SubScreen_QueueTwoSprites(0);

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    switch (FRAME(task)) {
    case 1:
        Sprite_QueueAt(3, 224, 142, &data_027c0060, 0x70b8, 0);
        break;
    case 2:
        Sprite_QueueAt(3, 220, 146, &data_027c0060, 0x70b8, 0);
        break;
    case 3:
        Sprite_QueueAt(3, 224, 146, &data_027c0060, 0x70b8, 0);
        break;
    case 4:
        Sprite_QueueAt(3, 220, 142, &data_027c0060, 0x70b8, 0);
        break;
    case 5:
        Sprite_QueueAt(3, 224, 144, &data_027c0060, 0x70b8, 0);
        break;
    case 6:
        Sprite_QueueAt(3, 222, 142, &data_027c0060, 0x70b8, 0);
        break;
    case 7:
        Sprite_QueueAt(3, 220, 144, &data_027c0060, 0x70b8, 0);
        break;
    case 8:
        Sprite_QueueAt(3, 222, 146, &data_027c0060, 0x70b8, 0);
        break;
    }
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    if (FRAME(task) == 8) {
        FRAME(task) = 0;
        Task_ResumeScript(task);
    }
}

#include "types.h"
#include "game/Unk_022977a4.h"
#include "game/rect.h"
#include "game/task.h"

extern u16 data_02297be0[][32];       /* tile map of the event text window */
extern Rect gEventChoiceButtons[4];          /* touch buttons of the four choices */

s8 Text_PrintId(s16 x, s16 y, u16 *map, u32 textId, u16 unk_30, u16 unk_32);
void EventChoice_DrawCursor(Task *task);
void DialogueCursor_Animate(void);
void TileMap_GetPixelPos(u16 *tile, u16 *tileMap, s16 *pixelPos);
void EventScript_GetTextWindowRect(Rect *rect);
void EventChoice_DrawRecord(int index, int x, int y);

/* The cursor sits in Task.waitFrames, read as a signed halfword (as in
 * EventChoice_Update). */
#define CURSOR(task) (*(s16 *)&(task)->waitFrames)

/* First step of the four-choice event prompt (task script data_02158548,
 * whose update is EventChoice_Update): prints the prompt text 0xc00002dd at
 * tile (7, 1), draws the cursor, sets the touch buttons of the choices over
 * the text window and draws the line of the record under the cursor. */
void EventChoice_Init(Task *task)
{
    s16 pos[2];
    int i;

    Text_PrintId(7, 1, data_02297be0[0], 0xc00002dd, data_022977a4.unk_00 << 12, data_022977a4.unk_04);
    EventChoice_DrawCursor(task);
    DialogueCursor_Animate();
    /* unk_34 holds a tile pointer here (EventCmd_StartTextMapTask); pos is not used */
    TileMap_GetPixelPos(*(u16 **)&task->unk_34, data_02297be0[0], pos);
    for (i = 0; i < 4; i++) {
        EventScript_GetTextWindowRect(&gEventChoiceButtons[i]); /* the whole text window */
    }
    gEventChoiceButtons[0].x += 64;
    gEventChoiceButtons[0].y += 8;
    gEventChoiceButtons[0].width = 32;
    gEventChoiceButtons[0].height = 16;
    gEventChoiceButtons[1].x += 104;
    gEventChoiceButtons[1].y += 8;
    gEventChoiceButtons[1].width = 32;
    gEventChoiceButtons[1].height = 16;
    gEventChoiceButtons[2].x += 144;
    gEventChoiceButtons[2].y += 8;
    gEventChoiceButtons[2].width = 32;
    gEventChoiceButtons[2].height = 16;
    gEventChoiceButtons[3].x += 192;
    gEventChoiceButtons[3].y += 8;
    gEventChoiceButtons[3].width = 56;
    gEventChoiceButtons[3].height = 16;
    EventChoice_DrawRecord(CURSOR(task), 7, 3);
}

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/event_script.h"
#include "game/map_unit.h"
#include "game/task.h"
#include "game/unit_target_entry.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern const EventScriptCommand gMapScrollScript[];
extern int data_022a6dd4;

s16 ScriptSlot_Find(const EventScriptCommand *script);
u32 Keypad_ReadConfirmCancel(void);
u32 MapTouch_ReadListTileTap(int count);
void func_020ef260(int *index, int count);
void MapTouch_UpdateListCursor(int *index, int count);
UnitTargetEntry *func_020dc524(int index);
void SecondScreen_ShowCursorInfo(int tile, MapUnit *unit);
void Sound_PlaySe(u32 soundId);
void func_020f5888(int x, int y);
void func_020f5c4c(void);
void ScreenConfig_RunActionHook(void);
void Map_ScrollViewToTile(int arg);
void MapScreen_DrawInfoPanel(void);

/* Bits Keypad_ReadConfirmCancel (buttons) and MapTouch_ReadListTileTap (touch) return. */
#define INPUT_CONFIRM 0x10
#define INPUT_CANCEL  0x20

#define GAME (data_027c027c->unk_48[data_027c0284])
#define TILE_INDEX(x, y) ((x) + GAME->mapTiles->rowStart[y])

/* Task step of the attack target picker (script data_02166430, started by
 * TargetListPicker_Init): task->unk_64 is the picked entry of the target
 * list, task->unk_66 the entry count. The d-pad and touch move the pick and
 * the cursor follows it, with a sound and the second screen info on a change.
 * Confirm stores the pick in data_022a6dd4 for AttackTargetSelect_Finish;
 * cancel stores -1 and puts the cursor back on the acting unit. Does nothing
 * while the map scroll script gMapScrollScript runs. */
void TargetListPicker_Update(Task *task)
{
    int old;
    int count;
    u16 input;
    u16 buttons;
    UnitTargetEntry *target;
    BOOL moved;
    int index;

    moved = FALSE;

    if (ScriptSlot_Find(gMapScrollScript) != -1) {
        return;
    }
    index = (s16)task->unk_64;
    count = task->unk_66;
    buttons = Keypad_ReadConfirmCancel();
    input = buttons | MapTouch_ReadListTileTap(count);
    if (input & INPUT_CANCEL) {
        GAME->cursorX = GAME->actingUnitDestX;
        GAME->cursorY = GAME->actingUnitDestY;
        data_022a6dd4 = -1;
        ScreenConfig_RunActionHook();
        Task_Destroy(task);
        func_020f5c4c();
        func_020f5888(GAME->cursorX, GAME->cursorY);
        return;
    }
    if (input & INPUT_CONFIRM) {
        data_022a6dd4 = (s16)task->unk_64;
        Task_Destroy(task);
        func_020f5c4c();
        return;
    }
    old = index;
    func_020ef260(&index, count);
    MapTouch_UpdateListCursor(&index, count);
    if (old != index) {
        moved = TRUE;
    }
    task->unk_64 = index;
    target = func_020dc524((s16)task->unk_64);
    /* needed to match: the original reads the tile as s16 */
    GAME->cursorX = *(s16 *)&target->unk_04;
    GAME->cursorY = *(s16 *)&target->unk_06;
    if (moved) {
        SecondScreen_ShowCursorInfo(GAME->mapTiles->tiles[TILE_INDEX(GAME->cursorX, GAME->cursorY)],
                                    &GAME->units[GAME->mapTiles->unitAt[TILE_INDEX(GAME->cursorX, GAME->cursorY)]]);
        Sound_PlaySe(0x1000b);
    }
    func_020f5888((s16)target->unk_04, (s16)target->unk_06);
    if (ScriptSlot_Find(gMapScrollScript) != -1) {
        ScreenConfig_RunActionHook();
        return;
    }
    Map_ScrollViewToTile(2);
    MapScreen_DrawInfoPanel();
}

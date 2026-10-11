#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/task.h"
#include "sdk/gx.h"

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern u32 gMapGameRunning;     /* 1 once Battle_ResetTurnState has run, 0 while a battle is set up */

void func_0210b97c(void);
void func_01ffd6f4(void);
void EventScript_UpdateAll(void);
void Game_RunStateFrame(void);
void ScriptSlot_RunAll(void);
void Task_RunList(Task *task);
void Map3d_Draw(void);
void MapView_QueueTileSprites(void);

/* The main loop step that the map game and the Design Room install. Runs
 * the map state's frame (Game_RunStateFrame) when gMapGameRunning is 1, the
 * script slots and task lists 1, 2, 3, 5 and 4. On the first screen, when
 * BG0 shows 3D, it starts the 3D frame first and swaps the 3D buffers last;
 * on the second screen it runs that screen config's unk_1c callback. */
void MainLoop_MapFrame(void)
{
    if ((void *)data_027c027c == data_027c0288.unk_00 && data_027c0280->bg0Mode == 1) {
        func_0210b97c();
        func_01ffd6f4();
    }
    EventScript_UpdateAll();
    if (gMapGameRunning != 0 && gMapGameRunning == 1) {
        Game_RunStateFrame();
    }
    ScriptSlot_RunAll();
    Task_RunList(data_027c027c->taskLists[1]);
    Task_RunList(data_027c027c->taskLists[2]);
    Task_RunList(data_027c027c->taskLists[3]);
    Task_RunList(data_027c027c->taskLists[5]);
    Task_RunList(data_027c027c->taskLists[4]);
    if ((void *)data_027c027c == data_027c0288.unk_00) {
        Map3d_Draw();
    } else {
        if (data_027c027c->unk_50[data_027c0284]->unk_1c != NULL) {
            data_027c027c->unk_50[data_027c0284]->unk_1c();
        }
        MapView_QueueTileSprites();
    }
    if ((void *)data_027c027c == data_027c0288.unk_00 && data_027c0280->bg0Mode == 1) {
        *REG_SWAP_BUFFERS = 2;
    }
}

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/task.h"
#include "sdk/gx.h"
#include "game/main_loop.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;

void Map3d_BeginFrame(void);
void EventScript_UpdateAll(void);
void Game_RunStateFrame(void);
void ScriptSlot_RunAll(void);
void Task_RunList(Task* task);
void Map3d_Draw(void);
void MapView_QueueTileSprites(void);

/* The main loop step that the map game and the Design Room install. Runs
 * the map state's frame (Game_RunStateFrame) when gMapGameRunning is 1, the
 * script slots and task lists 1, 2, 3, 5 and 4. On the first screen, when
 * BG0 shows 3D, it starts the 3D frame first and swaps the 3D buffers last;
 * on the second screen it runs that screen config's unk_1c callback. */
void MainLoop_MapFrame(void) {
  if ((void*)data_027c027c == data_027c0288.unk_00 &&
      data_027c0280->bg0Mode == 1) {
    G3X_Reset();
    Map3d_BeginFrame();
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
  if ((void*)data_027c027c == data_027c0288.unk_00) {
    Map3d_Draw();
  } else {
    if (data_027c027c->unk_50[data_027c0284]->unk_1c != NULL) {
      data_027c027c->unk_50[data_027c0284]->unk_1c();
    }
    MapView_QueueTileSprites();
  }
  if ((void*)data_027c027c == data_027c0288.unk_00 &&
      data_027c0280->bg0Mode == 1) {
    *REG_SWAP_BUFFERS = 2;
  }
}

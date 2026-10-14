#include "types.h"
#include "game/task.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022977a4.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/screen_other_map.h"
#include "game/main_loop.h"

void* Mem_Alloc(u32 size);
void MI_CpuFill8(void* dst, int value, int size);
void func_020b98bc(void);
void func_020f4a84(void);
void Players_Reset(void);
void MapRules_SetDefaults(void);
void Players_ApplyBattleSetup(void);
void Ov006_ResetMapView(int arg);
void Units_ClearAll(void);
void MapObjects_Clear(void);
void Skills_SetupForBothMaps(int a0, int mapId);
void MovementCost(void);
void CurrentPlayer_SyncState(void);
void CurrentPlayer_StartTurn(void);
void func_020c8888(void);
void Screens_SelectConfigs(void);
void Gfx_CopyBgBufferOnScreen1(void);
void Camera_ResetToPreset1(void* p);
void Ov006_SetUpScreens(void);
u8 Ov006_CountPropertyTiles(void);
void Ov006_FindHqs(void);
void Battle_RegisterFrameCallbacks(void);
void MainLoop_SetFrameCallback(void (*callback)(void));
void MainLoop_MapFrame(void);
void Font_SetDigitEndTile(int tile);
void MapCursor_SetTile(int x, int y, s16* tile, u16* pixel);
void Ov006_PostRequest(int request);
void Sound_StartSong(int songId);
void Ov006_LoadArmyTerrainGraphics(void);

extern Unk_ov006_0235f690* data_ov006_0236d640;
extern u8 data_022a7d5c[];
extern const TaskScriptCmd data_ov006_0236a218[];
void Ov006_UpdateSubScreen(void);

static TaskScriptCmd data_ov006_0236a1e0[] = {
#include "data/ov006/data_ov006_0236a1e0.inc.c"
};

#define STATE (data_027c027c->unk_48[data_027c0284])
#define OV6 data_ov006_0236d640

/* Second step of the data_ov006_0236a1f0 task script (func_ov006_02350ec0
 * starts it): sets up overlay 6, which looks like the Design Room (a map
 * filled with one terrain, a name field, four armies). Allocates the
 * overlay state, then on both screens loads map 4 with default rules and
 * runs battle start (Players_ApplyBattleSetup) as day 1, player 1. It gives each army
 * its default CO (Andy, Olaf, Eagle, Kanbei; Nell in the neutral slot),
 * puts the view at tile (7, 4) and starts the overlay's two tasks and song
 * 0x34. gameMode is left as it was. */
void DesignRoom_Init(Task* task) {
  /* The code reads the upper half of task->prev as an s16. The task is new
     * and alone in its list here, so it is likely always 0; what the field
     * meant in the original task struct is unknown. */
  s16 keep = ((s16*)&task->prev)[1];
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  void (*callback)(int);

  OV6 = Mem_Alloc(sizeof(Unk_ov006_0235f690));
  MI_CpuFill8(OV6, 0, sizeof(Unk_ov006_0235f690));
  OV6->unk_d8[0] = 0;
  if (keep == 0) {
    MI_CpuFill8(OV6, 0, sizeof(Unk_ov006_0235f690));
  }
  func_020b98bc();
  STATE->unk_1c4->playerCount = 4;
  func_020f4a84();

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  STATE->unk_1c4->mapId = 4;
  Players_Reset();
  MapRules_SetDefaults();
  Players_ApplyBattleSetup();
  STATE->currentPlayer = 1;
  STATE->day = 1;
  STATE->unk_1c4->unk_37 = 0;
  Ov006_ResetMapView(keep);
  Units_ClearAll();
  MapObjects_Clear();
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  STATE->unk_1c4->mapId = 4;
  Players_Reset();
  MapRules_SetDefaults();
  Players_ApplyBattleSetup();
  STATE->currentPlayer = 1;
  STATE->day = 1;
  STATE->unk_1c4->unk_37 = 0;
  Ov006_ResetMapView(keep);
  Units_ClearAll();
  MapObjects_Clear();
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  STATE->players[0].front.id = CO_NELL;
  STATE->players[1].front.id = CO_ANDY;
  STATE->players[2].front.id = CO_OLAF;
  STATE->players[3].front.id = CO_EAGLE;
  STATE->players[4].front.id = CO_KANBEI;
  Skills_SetupForBothMaps(4, STATE->unk_1c4->mapId);
  MovementCost();
  CurrentPlayer_SyncState();
  CurrentPlayer_StartTurn();
  func_020c8888();
  data_022977a4.unk_04 = 0x13a;
  Screens_SelectConfigs();
  callback = data_027c027c->unk_50[data_027c0284]->unk_54;
  if (callback != NULL) {
    callback(1);
  }
  Gfx_CopyBgBufferOnScreen1();
  Camera_ResetToPreset1(data_022a7d5c);
  Ov006_SetUpScreens();
  if (keep == 0) {
    OV6->unk_00 |= 0x1000;
    OV6->propertyCount = Ov006_CountPropertyTiles();
    Ov006_FindHqs();
  }
  Battle_RegisterFrameCallbacks();
  MainLoop_SetFrameCallback(MainLoop_MapFrame);
  gMapGameRunning = 0;
  Font_SetDigitEndTile(0x45);
  MapCursor_SetTile(7, 4, &STATE->cursorX, &STATE->cursorPixelX);
  *(s8*)&OV6->chosenSlot = -1; /* needed to match: stored as a signed -1 */
  OV6->toolTile = 1;
  OV6->tileArmy = 1;
  OV6->unitArmy = 1;
  OV6->terrainPage = OV6->tileArmy;
  OV6->unitPage = OV6->unitArmy;
  Ov006_PostRequest(0);
  OV6->unk_5c = Task_Create(data_ov006_0236a218, TASK_LIST(3));
  Sound_StartSong(0x34);
  Ov006_LoadArmyTerrainGraphics();
  OV6->unk_60 = Task_Create(data_ov006_0236a1e0, TASK_LIST(3));
}

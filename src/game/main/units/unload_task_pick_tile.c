#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/play_records.h"
#include "game/player.h"
#include "game/task.h"

extern int data_022a6dd4;
extern u8 data_022a6b30;
extern u8 data_022a6b34[];
extern s8 data_0216637b
    []; /* 5 bytes per direction: first path step to the tile in that direction */
extern s8 data_0216637c[]; /* the path end marker after it */

u32 NeighborTilePicker_Update(int* direction, int mask);
void Task_Destroy(Task* task);
void Task_ResumeScript(Task* task);
void MenuDepth_Pop(void);
void UnitMenu_Open(void);
int UnitPath_StopAtEnemy(int startX, int startY, int unitType, s8* path,
                         int end);
int UnitPath_CountSteps(const s8* path);
void* UnitShowTask_CreateMoving(int x, int y, int owner, int unitType,
                                void* arg);
void UnitType_PlayUnloadSound(int unitType);

/* Bits NeighborTilePicker_Update returns: A and B. */
#define INPUT_CONFIRM 0x10
#define INPUT_CANCEL 0x20

/* player index 0-3 of the acting unit: its slot in units divided by 64 */
#define OWNER_INDEX(s) \
  ((((u32)(((s)->actingUnit - (s)->units) & 0xc0)) << 10) >> 16)

/* Task step of the unload command while the player picks the drop tile:
 * unk_64 holds the mask of free directions, unk_66 the picked one and
 * unk_68 the cargo slot. B closes the picker and puts the view target back
 * on the acting unit's start tile. A records the direction (data_022a6dd4,
 * and plus one in data_022a6b34), starts the cargo unit's one-step move
 * show, and sets unk_6a (and 5 in data_022a6b34) when an enemy blocks the
 * step. Human players' moves count in gPlayRecords.tilesMoved. */
void UnloadTask_PickTile(Task* task) {
  int direction;
  u32 input;
  MapUnit* cargo;
  int mask = task->unk_64; /* needed to match: read before direction */

  direction = task->unk_66;
  input = NeighborTilePicker_Update(&direction, mask);
  task->unk_66 = direction;
  if (input & INPUT_CANCEL) {
    Task_Destroy(task);
    MenuDepth_Pop();
    UnitMenu_Open();
    MAP_STATE->cursorX = MAP_STATE->actingUnitStartX;
    MAP_STATE->cursorY = MAP_STATE->actingUnitStartY;
    return;
  }
  if (!(input & INPUT_CONFIRM)) {
    return;
  }
  data_022a6dd4 = task->unk_66;
  cargo = &MAP_STATE->units[MAP_STATE->actingUnit->cargo[task->unk_68]];
  data_022a6b34[data_022a6b30] = task->unk_66 + 1;
  MAP_STATE->pathSteps[0] = data_0216637b[task->unk_66 * 5];
  MAP_STATE->pathSteps[1] = data_0216637c[task->unk_66 * 5];
  if (UnitPath_StopAtEnemy(MAP_STATE->actingUnitDestX,
                           MAP_STATE->actingUnitDestY, cargo->type,
                           (s8*)MAP_STATE->pathSteps, -1) == 1) {
    task->unk_6a = 1;
    data_022a6b34[data_022a6b30] = 5;
  }
  UnitShowTask_CreateMoving(
      (u16)MAP_STATE->actingUnitDestX, (u16)MAP_STATE->actingUnitDestY,
      OWNER_INDEX(MAP_STATE), cargo->type, MAP_STATE->pathSteps);
  if (MAP_STATE->players[MAP_STATE->currentPlayer].controller ==
      PLAYER_CONTROLLER_HUMAN) {
    if (gPlayRecords.tilesMoved +
            UnitPath_CountSteps((s8*)MAP_STATE->pathSteps) >=
        999999) {
      gPlayRecords.tilesMoved = 999999;
    } else {
      gPlayRecords.tilesMoved += UnitPath_CountSteps((s8*)MAP_STATE->pathSteps);
    }
  }
  Task_ResumeScript(task);
  UnitType_PlayUnloadSound(cargo->type);
}

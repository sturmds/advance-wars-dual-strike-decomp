#include "types.h"
#include "game/task.h"
#include "game/map_header.h"
#include "game/save_progress.h"
#include "game/Unk_02080440.h"
#include "game/link_map_record_sync_task.h"
#include "game/math.h"

void* Mem_Alloc(u32 size);
void Task_Destroy(Task* task);
u8 Link_GetOwnPlayer(void);
void DesignMap_LoadFromSlot(int slot, DesignMapRecord* dest);

/* Design maps: ids MAP_CUSTOM_FIRST + 3 * player + slot, one of the three
 * map slots of each link player. */
static inline BOOL MapId_IsCustom(u16 mapId) {
  BOOL custom = FALSE;

  if (MAP_IS_CUSTOM(mapId)) {
    custom = TRUE;
  }
  return custom;
}

/* Needed to match: the original turns the result into a BOOL twice more
 * before the comparison with TRUE; these two wrappers keep both steps. */
static inline BOOL MapId_IsDesign(u16 mapId) {
  BOOL design = FALSE;

  if (MapId_IsCustom(mapId)) {
    design = TRUE;
  }
  return design;
}

static inline BOOL IsDesignMap(u16 mapId) {
  return MapId_IsDesign(mapId) ? TRUE : FALSE;
}

/* First step of the map record swap: for each of the battle's two maps that is
 * a design map, notes which link player made it, and that player loads the map's
 * record into the send buffer. Ends the task when neither map is a design map.
 * Records of maps that are not design maps are skipped. */
void LinkMapRecordSync_Init(LinkMapRecordSyncTask* task) {
  task->sentBytes = 0;
  task->receivedBytes = 0;
  task->blockSize = 2 * sizeof(DesignMapRecord);
  task->sendDone = 0;
  task->started = 0;
  task->receiveStarted = 0;
  task->senders[0] = -1;
  task->senders[1] = -1;
  if (IsDesignMap(MAP_STATE->unk_1c4->mapId) == TRUE) {
    task->senders[0] = HwDiv32(MAP_STATE->unk_1c4->mapId - MAP_CUSTOM_FIRST, 3);
  }
  if (IsDesignMap(MAP_STATE->secondMapId) == TRUE) {
    task->senders[1] = HwDiv32(MAP_STATE->secondMapId - MAP_CUSTOM_FIRST, 3);
  }
  if (task->senders[0] == -1 && task->senders[1] == -1) {
    Task_Destroy((Task*)task);
    return;
  }
  task->receiveBuffer = data_02291774.bytes;
  if (task->senders[0] == -1) {
    task->sentBytes = sizeof(DesignMapRecord);
    task->receivedBytes = task->sentBytes;
  }
  if (task->senders[1] == -1) {
    task->blockSize = sizeof(DesignMapRecord);
  }
  task->sendBuffer = Mem_Alloc(2 * sizeof(DesignMapRecord));
  if (task->senders[0] == Link_GetOwnPlayer()) {
    DesignMap_LoadFromSlot(
        HwMod32(MAP_STATE->unk_1c4->mapId - MAP_CUSTOM_FIRST, 3),
        (DesignMapRecord*)task->sendBuffer);
  }
  if (task->senders[1] == Link_GetOwnPlayer()) {
    DesignMap_LoadFromSlot(
        HwMod32(MAP_STATE->secondMapId - MAP_CUSTOM_FIRST, 3),
        (DesignMapRecord*)(task->sendBuffer + sizeof(DesignMapRecord)));
  }
}

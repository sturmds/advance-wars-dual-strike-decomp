#include "types.h"
#include "game/design_map.h"
#include "game/map_header.h"
#include "game/link_data_sync_task.h"

extern DesignMap data_022a6c84[];

void* Mem_Alloc(u32 size);
u8 DesignMap_GetPlayerCount(u8 index);
int DesignMap_CopyName(u8 index, char* dest);

#define DESIGN_MAP_NAME_SIZE 17
#define SEND_ROWS(task) ((DesignMap*)(task)->sendBuffer)

/* Sets up the design map swap: copies this player's first three design
 * map rows into a new send buffer (an unused row's name as zeros) and
 * clears the transfer counters. Bytes 0-1 of each row are not written. */
void LinkDesignMapSync_Init(LinkDataSyncTask* task) {
  s8 name[DESIGN_MAP_NAME_SIZE];
  int row;
  int i;

  task->sendBuffer = Mem_Alloc(LINK_DESIGN_MAP_BLOCK_SIZE);
  for (row = 0; row < LINK_DESIGN_MAPS_PER_PLAYER; row++) {
    SEND_ROWS(task)[row].playerCount = DesignMap_GetPlayerCount(row);
    if (DesignMap_CopyName(row, (char*)name) == 1) {
      for (i = 0; i < DESIGN_MAP_NAME_SIZE; i++) {
        SEND_ROWS(task)[row].name[i] = name[i];
      }
    } else {
      for (i = 0; i < DESIGN_MAP_NAME_SIZE; i++) {
        SEND_ROWS(task)[row].name[i] = 0;
      }
    }
    for (i = 0; i < ARMY_SLOT_COUNT; i++) {
      SEND_ROWS(task)[row].armyColours[i] = data_022a6c84[row].armyColours[i];
    }
    SEND_ROWS(task)[row].unk_19 = data_022a6c84[row].unk_19;
    SEND_ROWS(task)[row].unk_1a = data_022a6c84[row].unk_1a;
  }
  task->sentBytes = 0;
  task->receivedBytes = 0;
  task->sendDone = 0;
  task->started = 0;
  task->receiveStarted = 0;
}

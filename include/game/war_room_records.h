#ifndef GAME_WAR_ROOM_RECORDS_H
#define GAME_WAR_ROOM_RECORDS_H

#include "types.h"

/* First War Room map id; record tables index maps from here. */
#define WAR_ROOM_FIRST_MAP 139

/* One of the three best results kept per War Room map. */
typedef struct WarRoomRecord {
  u32 frontCo : 5;
  u32 backCo : 5; /* 0: no tag partner */
  u32 days : 10;  /* days the win took; 0: no record in this place */
  u32 score : 10;
  u32 : 2;
} WarRoomRecord;

/* gWarRoomRecords (main bss): records by MapListTables.subStrip, map
 * (id - WAR_ROOM_FIRST_MAP) and place. */
extern WarRoomRecord gWarRoomRecords[][44][3];

#endif

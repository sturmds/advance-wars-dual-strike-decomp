#ifndef GAME_WAR_ROOM_RECORDS_H
#define GAME_WAR_ROOM_RECORDS_H

#include "types.h"

/* First War Room map id; record tables index maps from here. */
#define WAR_ROOM_FIRST_MAP 139

/* One of the three best results kept per War Room map. */
typedef struct WarRoomRecord {
    u32 frontCo : 5;
    u32 backCo : 5;                    /* 0: no tag partner */
    u32 unk_0a : 10;                   /* 0: no record in this place */
    u32 unk_14 : 10;
    u32 : 2;
} WarRoomRecord;

/* data_02290c34 (main bss): records by MapListTables.unk_108, map
 * (id - WAR_ROOM_FIRST_MAP) and place. */
extern WarRoomRecord data_02290c34[][44][3];

#endif

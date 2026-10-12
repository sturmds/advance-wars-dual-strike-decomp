#include "types.h"
#include "game/war_room_records.h"

/* Rank for a score (War Room record score); its top ranks start at 200, 250 and 280. */
u32 Score_ToRank_020e5428(u32 score);

/* Marker offset of the best record of a War Room map: 5 minus its rank,
 * with ranks 0 and 1 counted as 2. MapSelect_DrawMapNames draws the tile
 * (4 - result) from the marker row. */
int MapSelect_GetRecordMarker(int list, int mapId) {
  u32 grade = Score_ToRank_020e5428(
      gWarRoomRecords[list][mapId - WAR_ROOM_FIRST_MAP][0].score);
  if (grade <= 1) {
    grade = 2;
  }
  return 5 - grade;
}

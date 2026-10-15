#include "types.h"
#include "game/battle_save.h"
#include "game/map_header.h"
#include "game/Unk_02080440.h"
#include "game/save_progress.h"

int Save_LoadRecord(
    int recordId,
    void*
        buffer); /* the definition takes only recordId; this call also passes r1 */
int func_0201fe28(int recordId);

#define VERSUS_SAVE_RECORD 6

/* Whether the main menu can offer to continue a saved battle of gameMode.
 * Only the versus save is checked: it is loaded, and a player-made map in
 * it needs its design map record (ids 10 and 11, for the first and second
 * map) to be usable. Other modes always pass. */
BOOL VersusSave_CanContinue(int gameMode) {
  BattleSaveRecord* record = &data_02291774.battle;
  int mapId;
  int secondMapId;

  if (gameMode != GAME_MODE_VERSUS) {
    return 1;
  }
  Save_LoadRecord(VERSUS_SAVE_RECORD, record);
  mapId = record->maps[0].unk_6c.mapId;
  secondMapId = record->maps[0].settings.secondMapId;
  if (mapId != 0 && MAP_IS_CUSTOM(mapId) && func_0201fe28(10) != 0) {
    return 0;
  }
  if (secondMapId != 0 && MAP_IS_CUSTOM(secondMapId) &&
      func_0201fe28(11) != 0) {
    return 0;
  }
  return 1;
}

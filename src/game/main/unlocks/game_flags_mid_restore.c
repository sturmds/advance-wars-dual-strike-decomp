#include "types.h"
#include "game/battle_save.h"
#include "game/save_progress.h"

u8 gGameFlagsLow[GAME_FLAGS_LOW_SIZE];
SaveGameFlagsMid gGameFlagsMid;
SaveGameFlagsMid gGameFlagsMidBackup;
void DesignMap_RefreshAllRows(void);

/* Puts the backed-up game flags back and redraws the design map rows. */
void GameFlagsMid_Restore(void) {
  gGameFlagsMid = gGameFlagsMidBackup;
  DesignMap_RefreshAllRows();
}

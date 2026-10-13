#include "types.h"
#include "game/save_progress.h"

u8 gGameFlagsLow[4];
/* also declared in src/game/main/link/link_map_flags_sync_update.c and 5 more */
SaveGameFlagsMid gGameFlagsMid;
/* also declared in src/game/main/unlocks/game_flags_mid_backup.c */
SaveGameFlagsMid gGameFlagsMidBackup;
void DesignMap_RefreshAllRows(void);

/* Puts the backed-up game flags back and redraws the design map rows. */
void GameFlagsMid_Restore(void) {
  gGameFlagsMid = gGameFlagsMidBackup;
  DesignMap_RefreshAllRows();
}

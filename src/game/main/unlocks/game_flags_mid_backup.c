#include "types.h"
#include "game/save_progress.h"

/* Copies the game flags to a backup when a link battle starts;
 * GameFlagsMid_Restore puts them back when the battle ends. */
void GameFlagsMid_Backup(void) { gGameFlagsMidBackup = gGameFlagsMid; }

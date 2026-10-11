#include "types.h"
#include "game/save_progress.h"

extern SaveGameFlagsMid gGameFlagsMid;
extern SaveGameFlagsMid gGameFlagsMidBackup;
void DesignMap_RefreshAllRows(void);

/* Puts the backed-up game flags back and redraws the design map rows. */
void GameFlagsMid_Restore(void)
{
    gGameFlagsMid = gGameFlagsMidBackup;
    DesignMap_RefreshAllRows();
}

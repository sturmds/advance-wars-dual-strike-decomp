#include "types.h"
#include "game/sound_arc.h"

extern SoundArc *data_022a8a60;             /* current sound archive */

BOOL func_02112988(SoundArcRange *range, const char *path);
BOOL func_021128b4(FSFile *file, SoundArcRange range);
BOOL SoundArc_ReadTables(SoundArc *arc, void *heap, int symbolLoad);

/* Opens the sound archive file at path, reads its tables (into heap when
 * given) and makes it the current sound archive. The current archive stays
 * unchanged on failure. */
void SoundArc_Open(SoundArc *arc, const char *path, void *heap, int symbolLoad)
{
    arc->info = NULL;
    arc->files = NULL;
    arc->unk_84 = 0;
    if (!func_02112988(&arc->range, path)) {
        return;
    }
    FS_InitFile(&arc->file);
    if (!func_021128b4(&arc->file, arc->range)) {
        return;
    }
    arc->opened = 1;
    if (SoundArc_ReadTables(arc, heap, symbolLoad)) {
        data_022a8a60 = arc;
    }
}

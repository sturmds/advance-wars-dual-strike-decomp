#include "types.h"
#include "game/sound_arc.h"

extern SoundArc* gCurrentSoundArc;

BOOL FS_ConvertPathToFileID(SoundArcRange* range, const char* path);
BOOL FS_OpenFileFast(FSFile* file, SoundArcRange range);
BOOL SoundArc_ReadTables(SoundArc* arc, void* heap, int symbolLoad);

/* Opens the sound archive file at path, reads its tables (into heap when
 * given) and makes it the current sound archive. The current archive stays
 * unchanged on failure. */
void SoundArc_Open(SoundArc* arc, const char* path, void* heap,
                   int symbolLoad) {
  arc->info = NULL;
  arc->files = NULL;
  arc->symbols = NULL;
  if (!FS_ConvertPathToFileID(&arc->range, path)) {
    return;
  }
  FS_InitFile(&arc->file);
  if (!FS_OpenFileFast(&arc->file, arc->range)) {
    return;
  }
  arc->opened = 1;
  if (SoundArc_ReadTables(arc, heap, symbolLoad)) {
    gCurrentSoundArc = arc;
  }
}

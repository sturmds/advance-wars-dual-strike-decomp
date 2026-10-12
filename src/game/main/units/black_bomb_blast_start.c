#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7c5c.h"
#include "game/black_bomb_blast_task.h"
#include "game/map_unit.h"
#include "game/task.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern const TaskScriptCmd data_02168f10[];
extern char data_02169180[]; /* "/bmap" */
extern char data_0216919c[]; /* "05e" */
extern char data_021691a0[]; /* "060" */
extern char data_021691a4[]; /* "05f" */

int MapScript_RunList10(int id, MapUnit* unit);
void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          u16 paletteOffset, int size);
void Gfx_QueueUnpackedFileCopy(const char* fileName, void* dest);
void Gfx_QueueFileCopy(const char* fileName, void* dest, int size);

/* Runs map script list 10 with id 45, loads the explosion graphics from
 * /bmap (for the 3D engine on its screen; on the other screen into sprite
 * tile gfxTile and sprite palette `palette`, which follows the 16
 * background palettes in the palette buffer), then starts the blast task
 * at tile (x, y) under parent. Only BlackBombExplosion_StartBlast calls it. */
void BlackBombBlast_Start(int x, int y, int gfxTile, int palette, int damage,
                          Task* parent) {
  BlackBombBlastTask* task;

  MapScript_RunList10(45, 0);
  if (data_027c027c == data_027c0288.unk_00) {
    FS_ChangeDir(data_02169180);
    Gfx_QueueUnpackedFileCopy(data_0216919c, data_022a7c5c->unk_68);
    Gfx_QueueFileCopy(data_021691a0, data_022a7c5c->unk_6c, 32);
  } else {
    FS_ChangeDir(data_02169180);
    File_UnpackTo(data_027c0280->objVram + gfxTile * 32, data_021691a4);
    Palette_LoadFromFile(data_021691a0, 0, (palette + 16) << 5, 32);
  }
  task = (BlackBombBlastTask*)Task_CreateChild(data_02168f10, parent);
  task->tileX = x;
  task->tileY = y;
  task->sprite = gfxTile | (palette << 12);
  task->damage = damage;
}

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7c5c.h"
#include "game/screen_layer0_blend.h"
#include "game/ex_machina_bolt_task.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern char data_02168b60[];
extern char data_02168b68[];
extern char data_02168b6c[];
extern char data_02168b70[];
extern char data_02168b74[];
extern char data_02168b78[];
extern char data_02168b7c[];

void FS_ChangeDir(const char *dir);
void File_UnpackTo(void *dest, const char *fileName);
void Palette_LoadFromFile(const char *fileName, int fileOffset, int paletteOffset, int size);
void Screen_SetLayerScreenSize(int layer, int screenSize);
void func_01ff8c84(int layer);
void Gfx_RequestLayerCopy(int layer);
void TileMap_AddToEntries(u16 *entries, u16 byteCount, int addend);
void Bg_FillRect(u16 *map, int x, int y, int width, int height, int tile);
void Gfx_QueueUnpackedFileCopy(const char *fileName, void *dest);
void Gfx_QueueFileCopy(const char *fileName, void *dest, int size);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Script step of gExMachinaBoltScript: loads the bolt picture from /syogun into
 * layer 0 (tiles, map, palette) and the animation that ExMachinaBolt_PlayAnim
 * plays (3D on the screen that shows the 3D engine, sprite tiles on the
 * other), then sets layer 0 up to blend in. When the target tile is in the
 * top 12 rows of the view (of 16-pixel tiles), the top (12 - row) * 2 rows of
 * 8-pixel map tiles are blanked; why is not known. */
void ExMachinaBolt_LoadGraphics(ExMachinaBoltTask *task)
{
    int row;

    FS_ChangeDir(data_02168b60);
    Screen_SetLayerScreenSize(0, 1);
    func_01ff8c84(0);
    Gfx_RequestLayerCopy(0);
    File_UnpackTo(data_027c0280->bgVram + (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14),
                  data_02168b68);
    File_UnpackTo(data_027c0280->layer0Map, data_02168b6c);
    TileMap_AddToEntries(data_027c0280->layer0Map, 0x800, 0x1000);
    row = task->tileY - (STATE->unk_1c4->scrollY >> 4);
    if (row < 12)
        Bg_FillRect(data_027c0280->layer0Map, 0, 0, 32, (12 - row) * 2, 0);
    Palette_LoadFromFile(data_02168b70, 0, 32, 32);
    Gfx_RequestLayerCopy(0);
    if (data_027c027c == data_027c0288.unk_00) {
        Gfx_QueueUnpackedFileCopy(data_02168b74, data_022a7c5c->unk_68);
        Gfx_QueueFileCopy(data_02168b78, data_022a7c5c->unk_6c, 32);
    } else {
        File_UnpackTo(data_027c0280->objVram + 0x3280, data_02168b7c);
        Palette_LoadFromFile(data_02168b78, 0, 0x260, 32);
    }
    Screen_SetLayer0Blend();
}

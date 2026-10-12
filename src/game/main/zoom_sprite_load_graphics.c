#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7c5c.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern u8 data_ov000_023503a0;
extern char data_02166cc8[]; /* "/bmap" */
extern char data_02166cd0[]; /* "/national" */
extern char data_02166cdc[]; /* palette file in /bmap */
extern const char* data_ov000_022f6674[];
extern const char* data_ov000_022f6688[];

void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
void SpriteAnim_SetAnimation(void* sprite, int frame);
void Gfx_QueueUnpackedFileCopy(const char* fileName, void* dest);
void Gfx_QueueFileCopy(const char* fileName, void* dest, int size);

/* Sets the sprite's animation (the next one when the sprite is on the right
 * half of the screen), then loads the graphics for the game language from
 * /national and a palette from /bmap: into the buffers of data_022a7c5c on
 * the screen that shows the 3D engine, into sprite VRAM and the palette buffer
 * on the other, for the sprite of a ZoomSpriteTask. */
void ZoomSprite_LoadGraphics(int screenX, int screenY, int animation,
                             void* sprite) {
  if (screenX >= 128) {
    SpriteAnim_SetAnimation(sprite, animation + 1);
  } else {
    SpriteAnim_SetAnimation(sprite, animation);
  }
  if (data_027c027c == data_027c0288.unk_00) {
    FS_ChangeDir(data_02166cd0);
    Gfx_QueueUnpackedFileCopy(data_ov000_022f6674[data_ov000_023503a0],
                              data_022a7c5c->unk_68);
    FS_ChangeDir(data_02166cc8);
    Gfx_QueueFileCopy(data_02166cdc, data_022a7c5c->unk_6c, 32);
  } else {
    FS_ChangeDir(data_02166cd0);
    File_UnpackTo(data_027c0280->objVram + 0x3280,
                  data_ov000_022f6688[data_ov000_023503a0]);
    FS_ChangeDir(data_02166cc8);
    Palette_LoadFromFile(data_02166cdc, 0, 608, 32);
  }
}

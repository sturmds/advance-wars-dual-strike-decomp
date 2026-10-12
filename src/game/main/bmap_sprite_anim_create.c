#include "types.h"
#include "game/Unk_022a7c5c.h"
#include "game/sprite_anim.h"

extern char data_02169180[]; /* "/bmap" */

SpriteAnim* SpriteAnim_Create(const void* file, int a, int b);
void FS_ChangeDir(const char* dir);

/* Creates a sprite from a /bmap file whose tiles and palette were loaded to
 * the buffers in data_022a7c5c (the 3D screen path of BlackBombBlast_Start).
 * SpriteAnim_Create gets 14 as its last argument when flag is set, else 6. */
SpriteAnim* BmapSpriteAnim_Create(const void* file, int flag) {
  SpriteAnim* sprite;

  if (flag) {
    sprite = SpriteAnim_Create(file, 1, 14);
  } else {
    sprite = SpriteAnim_Create(file, 1, 6);
  }
  FS_ChangeDir(data_02169180);
  sprite->tileBase = (u32)data_022a7c5c->unk_68;
  sprite->unk_28 = (u32)data_022a7c5c->unk_6c;
  return sprite;
}

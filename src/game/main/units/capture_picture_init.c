#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_02022404.h"
#include "game/capture_picture_task.h"

extern Unk_027c0280 *data_027c0280;
extern u8 data_ov000_023503a0;              /* game language */
extern const char *data_ov000_022f66c4[];   /* per language: capture text graphics in /national */
extern char data_021691b0[];                /* "/national" */
extern char data_02169180[];                /* "/bmap" */
extern char data_021691bc[];                /* "0ba", the text's palette */
extern u8 data_0214a568[];                  /* sprite animation data */
extern u8 data_0214a618[];
extern u8 data_0214a670[];
extern s16 gSineTable[];

void PropertyPicture_Load(int tile, int gfxTile, int palette);
void CaptureArmyGfx_Load(int owner, int gfxTile, int palette);
void FS_ChangeDir(const char *dir);
void File_UnpackTo(void *dest, const char *fileName);
void Palette_LoadFromFile(const char *fileName, int fileOffset, int paletteOffset, int size);
Unk_02022404 *SpriteAnim_Create(void *data, int a, int b);
void SpriteAnim_SetAnimation(Unk_02022404 *sprite, int animation);
s32 HwDiv32(s32 numer, s32 denom);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);

/* Attribute 2 of a sprite: OBJ palette and first tile. */
#define SPRITE_ATTR2(palette, tile) (((palette) << 12) | (tile))

/* First step of the capture picture task: loads the property picture (tile
 * 500, palette 5), the capturing army's graphics (tile 532, palette 4) and
 * the capture text for the game language (tile 404, palette 3), makes one
 * sprite for each, and sets up affine matrices 1 and 2 unscaled and 3
 * squashed to (20 - oldPoints) / 20, as CapturePicture_Draw does each frame.
 * Then shows oldPoints. */
void CapturePicture_Init(CapturePictureTask *task)
{
    u32 cosValue; /* needed to match: unsigned */
    s16 sinValue;
    int d1;
    int d0;

    task->armySprite = NULL;
    task->propertySprite = NULL;
    task->textSprite = NULL;
    PropertyPicture_Load(task->tile, 500, 5);
    CaptureArmyGfx_Load(task->owner, 532, 4);
    FS_ChangeDir(data_021691b0);
    File_UnpackTo(data_027c0280->objVram + 404 * 32, data_ov000_022f66c4[data_ov000_023503a0]);
    FS_ChangeDir(data_02169180);
    Palette_LoadFromFile(data_021691bc, 0, (3 + 16) << 5, 32);

    task->armySprite = SpriteAnim_Create(data_0214a568, 0, 2);
    SpriteAnim_SetAnimation(task->armySprite, 0);
    task->armySprite->unk_24 = SPRITE_ATTR2(4, 532);
    task->propertySprite = SpriteAnim_Create(data_0214a618, 0, 2);
    SpriteAnim_SetAnimation(task->propertySprite, 0);
    task->propertySprite->unk_24 = SPRITE_ATTR2(5, 500);
    task->textSprite = SpriteAnim_Create(data_0214a670, 0, 2);
    SpriteAnim_SetAnimation(task->textSprite, 0);
    task->textSprite->unk_24 = SPRITE_ATTR2(3, 404);

    cosValue = gSineTable[64] * 16;
    sinValue = gSineTable[0];
    Oam_SetAffineParams(1, HwDiv32(cosValue, 256), HwDiv32(-sinValue * 16, 256),
                        HwDiv32(sinValue * 16, 256), HwDiv32(cosValue, 256));
    /* a scale of 0 would divide by 0; use 2 then */
    d1 = HwDiv32((20 - task->oldPoints) << 8, 20) ? HwDiv32((20 - task->oldPoints) << 8, 20) : 2;
    d0 = HwDiv32((20 - task->oldPoints) << 8, 20) ? HwDiv32((20 - task->oldPoints) << 8, 20) : 2;
    Oam_SetAffineParams(3, HwDiv32(cosValue, 256), HwDiv32(-sinValue * 16, d0),
                        HwDiv32(sinValue * 16, 256), HwDiv32(cosValue, d1));
    Oam_SetAffineParams(2, HwDiv32(cosValue, 256), HwDiv32(-sinValue * 16, 256),
                        HwDiv32(sinValue * 16, 256), HwDiv32(cosValue, 256));
    task->points = task->oldPoints;
    task->unk_42 = 0;
}

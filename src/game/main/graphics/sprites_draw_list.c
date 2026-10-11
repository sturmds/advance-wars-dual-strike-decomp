#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280 *data_027c0280;

/* x and y hold OAM attribute bits and are read as signed halfwords. */
#define SPRITE_X(sprite) ((s16)(sprite)->x)
#define SPRITE_Y(sprite) ((s16)(sprite)->y)

void func_01ff817c(int x, int y, const void *data, int unk08, int unk0a);
void func_01ff8368(int x, int y, const void *data, int unk08);
void Oam_PutCells(int attr1, int attr0, const void *cells, int attr2, int tile);
void Oam_PutSprite(int x, int y, const void *shape, int tileAndPalette);

/* Puts the sprites of queue list into the OAM buffer, newest first. The
 * first entry of a list is its head and holds no sprite. A sprite with
 * bit 12 of x set and bit 8 clear in both y and the first halfword of its
 * data goes to func_01ff817c (with unk_0a) or func_01ff8368 (without), which
 * look like affine variants (a guess); the others to Oam_PutCells (with
 * unk_0a) or Oam_PutSprite (without). */
void SpriteList_Draw(int list)
{
    SpriteEntry *sprite = &data_027c0280->spriteLists[list];

    if (sprite == NULL) {
        return;
    }
    do {
        if (sprite->unk_0c == NULL) {
            sprite = sprite->next;
            continue;
        }
        if ((SPRITE_X(sprite) & 0x1000) && !((((u16 *)sprite->unk_0c)[0] | SPRITE_Y(sprite)) & 0x100)) {
            if (sprite->unk_0a != 0) {
                func_01ff817c(SPRITE_X(sprite), SPRITE_Y(sprite), sprite->unk_0c, sprite->unk_08, sprite->unk_0a);
            } else {
                func_01ff8368(SPRITE_X(sprite), SPRITE_Y(sprite), sprite->unk_0c, sprite->unk_08);
            }
        } else if (sprite->unk_0a != 0) {
            Oam_PutCells(SPRITE_X(sprite), SPRITE_Y(sprite), sprite->unk_0c, sprite->unk_08, sprite->unk_0a);
        } else {
            Oam_PutSprite(SPRITE_X(sprite), SPRITE_Y(sprite), sprite->unk_0c, sprite->unk_08);
        }
        sprite = sprite->next;
    } while (sprite != NULL);
}

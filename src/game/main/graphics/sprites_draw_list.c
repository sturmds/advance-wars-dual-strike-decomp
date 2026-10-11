#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280 *data_027c0280;

void Oam_PutFlippedCells(int x, int y, const void *data, int unk08, int unk0a);
void func_01ff8368(int x, int y, const void *data, int unk08);
void Oam_PutCells(int attr1, int attr0, const void *cells, int attr2, int tile);
void Oam_PutSprite(int x, int y, const void *shape, int tileAndPalette);

/* Puts the sprites of queue list into the OAM buffer, newest first. The
 * first entry of a list is its head and holds no sprite. A sprite with
 * bit 12 of x set (horizontal flip) and bit 8 clear (not affine) in both y
 * and the first halfword of its data goes to Oam_PutFlippedCells (with
 * unk_0a) or func_01ff8368 (without); the others to Oam_PutCells (with
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
        if ((sprite->x & 0x1000) && !((((u16 *)sprite->unk_0c)[0] | sprite->y) & 0x100)) {
            if (sprite->unk_0a != 0) {
                Oam_PutFlippedCells(sprite->x, sprite->y, sprite->unk_0c, sprite->unk_08, sprite->unk_0a);
            } else {
                func_01ff8368(sprite->x, sprite->y, sprite->unk_0c, sprite->unk_08);
            }
        } else if (sprite->unk_0a != 0) {
            Oam_PutCells(sprite->x, sprite->y, sprite->unk_0c, sprite->unk_08, sprite->unk_0a);
        } else {
            Oam_PutSprite(sprite->x, sprite->y, sprite->unk_0c, sprite->unk_08);
        }
        sprite = sprite->next;
    } while (sprite != NULL);
}

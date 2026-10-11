/* The original was built without optimization: no value is reused from a
 * register, every global is read again. */
#pragma optimization_level 0


#include "types.h"
#include "game/name_entry_work.h"
#include "game/sprite_anim.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;

void NameEntry_PlaceButtonSprites(NameEntry *entry)
{
    Unk_027c0280 *saved0280 = data_027c0280;
    Unk_02080440 *saved027c;

    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    entry->buttonSprites[0]->tileBase = 0xc200;
    entry->buttonSprites[1]->tileBase = 0xc200;
    entry->buttonSprites[2]->tileBase = 0xc200;
    SpriteAnim_Draw(entry->buttonSprites[0], 42, 128);
    SpriteAnim_Draw(entry->buttonSprites[1], 122, 128);
    SpriteAnim_Draw(entry->buttonSprites[2], 210, 128);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
}

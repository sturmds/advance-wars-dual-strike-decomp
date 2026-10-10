#include "types.h"
#include "game/ov003_object.h"
#include "game/ov003_unit.h"
#include "game/Unk_ov003_02369bb8.h"

extern Ov003Unit data_ov003_02369538[16];
extern u8 data_ov003_02365d10[];       /* per tile code: bits 5-7 owner, bits 0-4 terrain */

u32 func_020279c4(int x1, int y1, int x2, int y2);
void func_ov003_0235b64c(Ov003Object *object, Ov003Unit *unit);
void Ov003Object_HitTile(Ov003Object *object, int tileX, int tileY);
Unk_ov003_02369bb8 *Ov003_FindEntryAtTile(int x, int y);
void func_ov003_0235e9b8(void);
void Ov003_CreateSpriteVariant0(int x, int y);

/* Runs when an object of kind 5 or 7 ends its trip. Every live unit except
 * the one named by object->unk_0a, within 35 units (pixels / 8) of the object,
 * goes to func_ov003_0235b64c. Then every tile within three of the object's
 * tile and 35 units from it goes to Ov003Object_HitTile (terrain 9, 21 and 26
 * once per Unk_ov003_02369bb8 entry, and not when object->variant equals
 * the current spawn row). */
void Ov003Object_Impact(Ov003Object *object)
{
    int tileY;
    Ov003Unit *unit;
    Unk_ov003_02369bb8 *entry;
    int col;
    int i;
    int row;
    int tileX;

    unit = data_ov003_02369538;
    for (i = 0; i < 16; i++, unit++) {
        if (unit->hp != 0 && !unit->unk_17_6 && i != object->unk_0a
            && func_020279c4(object->x >> 3, object->y >> 3, unit->x >> 3, unit->y >> 3) < 35) {
            func_ov003_0235b64c(object, unit);
        }
    }
    tileX = object->x >> 7;
    tileY = object->y >> 7;
    func_ov003_0235e9b8();
    for (row = tileY - 3; row <= tileY + 3; row++) {
        if (row < 0 || row >= data_ov003_02364a88->height) {
            continue;
        }
        for (col = tileX - 3; col <= tileX + 3; col++) {
            if (col < 0 || col >= data_ov003_02364a88->width) {
                continue;
            }
            if (func_020279c4(object->x >> 3, object->y >> 3, ((col << 7) + 64) >> 3, ((row << 7) + 64) >> 3) >= 35) {
                continue;
            }
            switch (data_ov003_02365d10[data_ov003_02364a88->cells[row * data_ov003_02364a88->width + col]] & 31) {
            case 9:
            case 21:
            case 26:
                if (object->variant == data_ov003_02364928->row) {
                    break;
                }
                entry = Ov003_FindEntryAtTile(col, row);
                if (!entry->unk_09_0) {
                    Ov003Object_HitTile(object, entry->x, entry->y);
                    entry->unk_09_0 = 1;
                }
                break;
            default:
                Ov003Object_HitTile(object, col, row);
                break;
            }
        }
    }
    Ov003_CreateSpriteVariant0(object->x >> 3, object->y >> 3);
}

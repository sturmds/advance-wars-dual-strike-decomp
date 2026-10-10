#include "types.h"
#include "game/ov003_object.h"
#include "game/ov003_unit.h"
#include "game/Unk_ov003_02369bb8.h"
#include "game/terrain.h"

extern u8 data_ov003_02365d10[];       /* per tile code: bits 5-7 row, bits 0-4 terrain */
/* Bytes 0x11 and 0x12 of the 20-byte Ov003TileInfo entries in
 * data_ov003_02364d88: index with type * 20. */
extern u8 data_ov003_02364d99[];
extern u8 data_ov003_02364d9a[];

int func_ov003_0235d5d4(int row, int kind, int arg2, int targetType);
void Ov003Row_AddGauge(int row, int arg1);
int Ov003_SpawnItem(int x, int y);
Unk_ov003_02369bb8 *Ov003_FindEntryAtTile(int x, int y);
void func_ov003_0236179c(int team);
void func_ov003_02364524(int x, int y);
void func_ov003_02364618(int x, int y);
void func_ov003_0236470c(int x, int y);
int Ov003_IsRowDone(void);
u32 Ov003_RandNext(void);

#define STATE data_ov003_02364928
#define MAP data_ov003_02364a88
#define TILE_AT(x, y) ((y) * MAP->width + (x))

/* A shot (object) has hit map tile (x, y), and stops. A Minicannon, Black
 * Cannon (terrain 26, or 9 through its main tile), pipe seam or terrain 3
 * tile takes damage from func_ov003_0235d5d4 plus -3 to 4 at random; the
 * shooter's own row cannot hurt its cannons. Target types 6-9 index the
 * same table as Ov003Unit.tile. When the tile's hp runs out it turns into
 * its broken tile code, the shooter's row counts it, and a freed
 * cannon entry can make Ov003_IsRowDone return 1, which goes on to
 * func_ov003_0236179c. A shot that breaks nothing puffs where it is, except
 * kinds 5 and 7. */
void Ov003Object_HitTile(Ov003Object *object, int x, int y)
{
    int targetType;
    BOOL rowDone;
    int damage;
    int py;
    int px;
    Unk_ov003_02369bb8 *structure;

    rowDone = FALSE;
    object->active = 0;
    switch (data_ov003_02365d10[MAP->cells[TILE_AT(x, y)]] & 31) {
    case 3:
        targetType = 8;
        break;
    case TERRAIN_MINICANNON:
        if (object->variant == STATE->row) {
            if (object->kind == 5 || object->kind == 7) {
                return;
            }
            func_ov003_0236470c(object->x >> 3, object->y >> 3);
            return;
        }
        targetType = 6;
        break;
    case 9:
        /* a structure over several tiles: damage goes to its main tile */
        structure = Ov003_FindEntryAtTile(x, y);
        x = structure->x;
        y = structure->y;
    case TERRAIN_BLACK_CANNON:
        if (object->variant == STATE->row) {
            if (object->kind == 5 || object->kind == 7) {
                return;
            }
            func_ov003_0236470c(object->x >> 3, object->y >> 3);
            return;
        }
        targetType = 7;
        break;
    case TERRAIN_PIPE_SEAM:
        targetType = 9;
        break;
    default:
        if (object->kind == 5 || object->kind == 7) {
            return;
        }
        func_ov003_0236470c(object->x >> 3, object->y >> 3);
        return;
    }

    damage = func_ov003_0235d5d4(object->variant, object->kind, 0, targetType);
    damage += (Ov003_RandNext() & 7) - 3;
    if (MAP->unk_f1a[TILE_AT(x, y)] <= damage) {
        MAP->unk_f1a[TILE_AT(x, y)] = 0;
        switch (MAP->cells[TILE_AT(x, y)]) {
        case 0x182:
        case 0x183:
        case 0x184:
        case 0x185:
            Ov003_FindEntryAtTile(x, y)->busy = 0;
            if (Ov003_IsRowDone() == 1) {
                rowDone = TRUE;
            }
            /* fall through */
        case 0x20:
        case 0x146:
        case 0x147:
            MAP->cells[TILE_AT(x, y)] = 4;
            break;
        case 0x163:
            MAP->cells[TILE_AT(x, y)] = 0x123;
            break;
        case 0x162:
            MAP->cells[TILE_AT(x, y)] = 0x122;
            break;
        case 0x187:
            Ov003_FindEntryAtTile(x, y)->busy = 0;
            if (Ov003_IsRowDone() == 1) {
                rowDone = TRUE;
            }
            /* the 3x3 block around it turns into its wreck */
            MAP->cells[TILE_AT(x - 1, y - 1)] += 32;
            MAP->cells[TILE_AT(x - 1, y)] += 32;
            MAP->cells[TILE_AT(x - 1, y + 1)] += 32;
            MAP->cells[TILE_AT(x, y - 1)] += 32;
            MAP->cells[TILE_AT(x, y)] += 32;
            MAP->cells[TILE_AT(x, y + 1)] += 32;
            MAP->cells[TILE_AT(x + 1, y - 1)] += 32;
            MAP->cells[TILE_AT(x + 1, y)] += 32;
            MAP->cells[TILE_AT(x + 1, y + 1)] += 32;
            break;
        default:
            return;
        }
        if (object->kind < 6) {
            Ov003Row_AddGauge(object->variant, data_ov003_02364d9a[targetType * 20]);
            /* targets broken by the shooter's row, per type */
            STATE->rows[object->variant - 1].unk_28[targetType]++;
        }
        if (targetType == 7) {
            py = (y << 7) + 64;
            px = (x << 7) + 64;
            func_ov003_02364618(px >> 3, py >> 3);
        } else {
            py = (y << 7) + 64;
            px = (x << 7) + 64;
            func_ov003_02364524(px >> 3, py >> 3);
        }
        if (targetType == 8 && object->variant != STATE->row) {
            STATE->unk_1d9++;
            if (STATE->unk_1d9 >= 4 && Ov003_SpawnItem(px, py) == 1) {
                STATE->unk_1d9 = 0;
            }
        }
        if (rowDone == TRUE) {
            func_ov003_0236179c(STATE->row);
        }
    } else {
        if (targetType == 6 || targetType == 7) {
            Ov003_FindEntryAtTile(x, y)->unk_09_6 = 2;
        }
        MAP->unk_f1a[TILE_AT(x, y)] -= damage;
        if (object->kind < 6) {
            Ov003Row_AddGauge(object->variant, data_ov003_02364d99[targetType * 20]);
        }
        if (object->kind == 5 || object->kind == 7) {
            return;
        }
        func_ov003_0236470c(object->x >> 3, object->y >> 3);
    }
}

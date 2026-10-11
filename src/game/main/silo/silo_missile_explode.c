#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"
#include "game/silo_missile_task.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern const u8 data_0213dd38[];

BOOL Map_AddTileOffset(int *x, int *y, int list, int index);
int DamageFromSilos(int playerIndex, int coSlot);
void Unit_DamageAtTile(int x, int y, int damage);
void func_020c4018(void);
struct SpriteAnimTask *SpriteAnimTask_Start(const void *file, int x, int y, u32 tileBase, int animation, int layer);
void Sound_PlaySe(u32 soundId);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Script step after the missile lands: damages the units on every tile of
 * offset list 1 around the target by unk_44, less the owner's silo damage
 * reduction (DamageFromSilos). With bit 0 of unk_60 set, Black Hole units
 * take no damage. Then plays the explosion animation and sound. */
void SiloMissile_Explode(SiloMissileTask *task)
{
    int pos[2];
    int index;
    MapUnit *unit;
    int damage;
    int reduction;

    index = 0;
    while (1) {
        pos[0] = task->x;
        pos[1] = task->y;
        if (!Map_AddTileOffset(&pos[0], &pos[1], 1, index++)) {
            break;
        }
        if (pos[0] < 0 || pos[0] >= STATE->unk_1c4->mapWidth) {
            continue;
        }
        if (pos[1] < 0 || pos[1] >= STATE->unk_1c4->mapHeight) {
            continue;
        }
        unit = &STATE->units[STATE->mapTiles->unk_508[pos[0] + STATE->mapTiles->rowStart[pos[1]]]];
        damage = task->unk_44;
        /* needed to match: the original adds the call to a zero */
        reduction = 0;
        reduction += DamageFromSilos(MAP_UNIT_OWNER(STATE, unit), 0);
        damage -= reduction;
        if ((task->unk_60 & 1) && STATE->players[MAP_UNIT_OWNER(STATE, unit)].armyColour == ARMY_BLACK_HOLE) {
            continue;
        }
        Unit_DamageAtTile(pos[0], pos[1], damage);
    }
    func_020c4018();
    SpriteAnimTask_Start(data_0213dd38, task->pixelX - STATE->unk_1c4->scrollX,
                         task->pixelY - STATE->unk_1c4->scrollY, task->spriteAttr, 3, 1);
    Sound_PlaySe(0xac);
}

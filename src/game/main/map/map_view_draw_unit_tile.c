#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"
#include "game/skills.h"

#define STATE (data_027c027c->unk_48[data_027c0284])

extern int data_027c0284;
extern Unk_02080440 *data_027c027c;

void MapView_ClearUnitTile(u16 x, u16 y);
BOOL UnitSlot_IsVisible(int slot);
u16 *Bg_GetMapEntry(int layer, int x, int y);
s32 HwDiv32(s32 numerator, s32 denominator);
void UnitIcon_DrawEntries(u16 *entry, int type, int look, u8 carrying, u8 capture, u16 shownHp, u16 dived, u8 hpHidden,
                   BOOL lowFuel, BOOL lowAmmo, BOOL paralysed);

/* Draws the unit shown on map tile (x, y) into BG layer 1, or clears the
 * tile when there is none to show. look is 0x100 for a unit that has
 * waited (grey), else the slot's owner bits. Not matched yet: the original
 * keeps units and unit in one register (r7). */
void MapView_DrawUnitTile(int x, int y)
{
    int idx;
    MapUnit *unit;
    BOOL hpHidden;
    u8 slot;
    int bgY;
    BOOL flag;
    BOOL paralysed;
    int bgX;
    MapUnit *units;
    u16 half;
    MapTiles *tiles;
    BOOL lowFuel;
    u16 *entry;
    int shownHp;
    int look;
    BOOL lowAmmo;

    if (x < 0 || x >= STATE->unk_1c4->mapWidth || y < 0 || y >= STATE->unk_1c4->mapHeight) {
        MapView_ClearUnitTile(x, y);
        return;
    }
    tiles = STATE_IDX_FIRST->mapTiles;
    idx = x + tiles->rowStart[y];
    slot = tiles->unk_508[idx];
    if (slot == 0 || tiles->unk_2338[idx] == 0 || !UnitSlot_IsVisible(slot)
        || ((units = STATE->units)[slot].flags & MAP_UNIT_LOCKED)) {
        MapView_ClearUnitTile(x, y);
        return;
    }
    if (units[slot].flags & MAP_UNIT_WAITED) {
        look = 0x100;
    } else {
        look = (u16)(slot & 0xc0);
    }
    lowFuel = FALSE;

    bgX = (s16)(((x - STATE->unk_1c4->unk_26) & 15) * 2);
    bgY = (s16)(((y - STATE->unk_1c4->unk_28) & 15) * 2);
    lowAmmo = lowFuel;
    paralysed = lowFuel;
    unit = &units[slot];

    flag = FALSE;
    if (Ov000_gUnitStats[units[slot].type].fuel != 0 && unit->fuel <= Ov000_gUnitStats[units[slot].type].fuel >> 1) {
        flag = TRUE;
    }
    if (flag) {
        lowFuel = TRUE;
    }
    flag = FALSE;
    if (Ov000_gUnitStats[units[slot].type].ammo != 0 && unit->ammo <= Ov000_gUnitStats[units[slot].type].ammo >> 1) {
        flag = TRUE;
    }
    if (flag) {
        lowAmmo = TRUE;
    }
    if (unit->flags & MAP_UNIT_PARALYSED) {
        paralysed = TRUE;
    }
    half = ((x - STATE->unk_1c4->unk_26) & 16) ? 0x400 : 0;
    entry = Bg_GetMapEntry(1, bgX, bgY) + half;
    if (unit->hp == 0) {
        shownHp = 0;
    } else {
        shownHp = HwDiv32(unit->hp - 1, 10) + 1;
    }
    hpHidden = CheckSkillsEquipped(STATE->players[(slot >> 6) + 1].skills[0], SKILL_HIDDEN_HP);
    UnitIcon_DrawEntries(entry, STATE->units[slot].type, look, unit->cargo[0] | unit->cargo[1], unit->captureProgress,
                  shownHp, unit->flags & MAP_UNIT_DIVED, hpHidden, lowFuel, lowAmmo, paralysed);
}

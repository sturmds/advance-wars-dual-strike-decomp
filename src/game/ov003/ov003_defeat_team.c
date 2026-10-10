#include "types.h"
#include "game/ov003_unit.h"
#include "game/Unk_ov003_02369bb8.h"
#include "game/Unk_02160ac0.h"

extern Ov003Unit data_ov003_02369538[16];
extern Unk_ov003_02369bb8 data_ov003_02369bb8[4];

u16 func_0207a1dc(void);
void Ov003_CreateSprite(int x, int y);
void Ov003Row_StopCountdown(int team);
void Ov003_ReplaceRowSpecialTiles(int team);
void func_ov003_02361c7c(int winner);
void func_ov003_02362c44(void);

#define STATE data_ov003_02364928

/* Knocks team (1 to 5) out: its HQ was captured, its row is done, or time
 * ran out. Removes all its units (with a sprite where each one stood), frees
 * its row of cells and counts the teams left. With two or more left, the
 * team's slots only scroll the map and Ov003_ReplaceRowSpecialTiles(team) runs; with
 * one left, func_ov003_02361c7c gets the winner. When unk_1da_0 is 7, it only
 * clears unk_01_4 of the slots in the func_0207a1dc mask. */
void Ov003_DefeatTeam(int team)
{
    Ov003Unit *unit;
    Unk_ov003_02369bb8 *entry;
    int alive;
    int winner;
    int count;
    int i;
    int j;

    unit = data_ov003_02369538;
    if (STATE->unk_1da_0 == 7) {
        for (i = 0; i < 8; i++) {
            if ((func_0207a1dc() | 1) & (1 << i)) {
                STATE->slots[i].unk_01_4 = 0;
            }
        }
        return;
    }

    STATE->rows[team - 1].active = 0;
    Ov003Row_StopCountdown(team);

    for (i = 0; i < 16; i++, unit++) {
        if (unit->hp == 0 && !unit->unk_17_7) {
            continue;
        }
        if (team != unit->unk_12_3) {
            continue;
        }
        unit->hp = 0;
        /* 128 units per tile, 16 pixels per tile */
        Ov003_CreateSprite(unit->x >> 3, unit->y >> 3);
        if (team == STATE->row) {
            continue;
        }
        /* drop the unit from the busy entries that point at it */
        entry = data_ov003_02369bb8;
        for (j = 0; j < 4; j++, entry++) {
            if (entry->busy && entry->target == unit) {
                entry->target = NULL;
            }
        }
    }

    for (i = 0; i < STATE->rows[team - 1].spawnCount; i++) {
        STATE->rows[team - 1].cells[i].unit = -1;
        if (STATE->rows[team - 1].cells[i].state != 0) {
            STATE->rows[team - 1].cells[i].state = 3;
        }
    }

    alive = 0;
    winner = 0;
    count = STATE->rowCount + 1;
    for (i = 1; i < count; i++) {
        if (STATE->rows[i - 1].active) {
            winner = i;
            alive++;
        }
    }

    if (data_02160ac0->unk_7c == 0) {
        STATE->slots[STATE->currentSlot].unk_01_4 = 0;
    } else {
        for (i = 0; i < 8; i++) {
            if ((func_0207a1dc() | 1) & (1 << i)) {
                if (alive >= 2) {
                    if (team == STATE->slots[i].row) {
                        STATE->slots[i].unk_01_4 = OV003_SLOT_SCROLLING;
                    }
                } else {
                    STATE->slots[i].unk_01_4 = 0;
                }
            }
        }
    }

    if (team == STATE->slots[STATE->currentSlot].row) {
        func_ov003_02362c44();
    }
    if (alive >= 2) {
        Ov003_ReplaceRowSpecialTiles(team);
    } else {
        func_ov003_02361c7c(winner);
    }
}

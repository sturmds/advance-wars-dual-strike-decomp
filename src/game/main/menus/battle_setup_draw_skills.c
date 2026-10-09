#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/menu_state.h"
#include "game/skills.h"

void func_0201e6b4(s16 x, s16 y, int w, int h);       /* window frame */
void func_02026d54(int x, int y, int tile, int size);  /* skill icon */
void func_020277a4(u16 x, u16 y, u16 *map, int a);     /* empty skill slot */
void func_02027804(u16 *map, u16 x, u16 y, int w, int h);
int func_02069920(void);                               /* 1: the cursor is on a CO (setup.unk_2e4 8-15) */
void func_020e6cc8(u8 *out, int co, int set);          /* equipped skill ids of a set, ended by 0xff */
int Co_GetRank(u32 co);

extern Unk_027c0280 *data_027c0280;
extern MenuState gMenuState;

/* Draws the skill window at (x, y) for the CO under the battle setup cursor:
 * one empty slot per rank, at most 4, then the icons of the Force Ranks
 * equipped in set `set`. */
void BattleSetup_DrawSkills(int x, int y, int set)
{
    u8 skills[4];
    int i;
    int slots;
    int cursor;
    MenuState *shifted;

    func_0201e6b4(x, y, 10, 4);
    func_02027804(data_027c0280->layer0Map, x + 1, y + 1, 8, 2);
    if (func_02069920() != 1) {
        return;
    }

    /* Cursor items 8-15 are the CO slots, setup.cos[player][front or back].
     * Needed to match: the original adds the slot's offset to gMenuState
     * first and the offset of cos last, so shifted->setup.cos[0][0] is
     * setup.cos[cursor >> 1][cursor & 1]. */
    cursor = gMenuState.setup.unk_2e4 - 8;
    shifted = (MenuState *)((u8 *)((u16 *)&gMenuState + (cursor >> 1)) + (cursor & 1));
    if (shifted->setup.cos[0][0] == 0) {
        slots = 0;
    } else {
        slots = Co_GetRank(shifted->setup.cos[0][0]);
        if (slots > 4) {
            slots = 4;
        }
    }
    for (i = 0; i < slots; i++) {
        func_020277a4(x + 1 + i * 2, y + 1, data_027c0280->layer1Map, 0);
    }

    if (shifted->setup.cos[0][0] == 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        skills[i] = 0;
    }
    func_020e6cc8(skills, shifted->setup.cos[0][0], set);
    for (i = 0; i < 4; i++) {
        if (skills[i] == 0 || skills[i] == 0xff) {
            return;
        }
        /* Force Rank icons: 4 tiles each from tile 0x100 */
        func_02026d54(x + 1 + i * 2, y + 1, (skills[i] - SKILL_RANK_BRUISER) * 4 + 0x100, 2);
    }
}

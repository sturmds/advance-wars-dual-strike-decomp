#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void func_01ff8d60(int a);
void Screen_SetCurrent(void *state);
void func_0200acac(void *state);
void func_02014d88(int x, int y, void *target, u32 text, int a, int b);
void func_02015258(int x, int y, void *target, u32 text, int a, int b);
Unk_02023724 *func_02023724(void *key);
void func_02027804(void *target, int x, int y, int w, int h);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;
extern u8 data_021613a0[];             /* CO ids: Andy, Max, Sami, Flak, Lash, Adder, Hawke */
extern u8 data_021613d8[];

/* Draws the CO Power name and a second text line for the CO selected in
 * a seven-CO list. */
void DrawSelectedCoPowerText(void)
{
    Unk_02023724 *list;
    u32 text;
    Unk_027c0280 *saved0280 = data_027c0280;
    Unk_02080440 *saved027c;

    func_0200acac(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    list = func_02023724(data_021613d8);
    switch (data_021613a0[list->unk_32]) {
    case CO_ANDY:
        text = 0x44000003;
        break;
    case CO_MAX:
        text = 0x44000004;
        break;
    case CO_SAMI:
        text = 0x44000005;
        break;
    case CO_FLAK:
        text = 0x44000006;
        break;
    case CO_LASH:
        text = 0x44000007;
        break;
    case CO_ADDER:
        text = 0x44000008;
        break;
    case CO_HAWKE:
        text = 0x44000009;
        break;
    }
    func_02027804(data_027c0280->layer0Map, 17, 13, 15, 8);
    /* Needed to match: indexing from &gCoTable[0].blocks[POWER_CO] keeps
     * the 0x120 offset in the literal pool instead of the load. TextRefs
     * are passed as one u32. */
    func_02014d88(19, 13, data_027c0280->layer0Map,
                  *(u32 *)&((CoRecord *)&gCoTable[0].blocks[POWER_CO])[data_021613a0[list->unk_32] & 0x7f].name,
                  0, 0);
    func_02015258(17, 15, data_027c0280->layer0Map, text, 0, 0x340);
    func_01ff8d60(0);
    Screen_SetCurrent(saved027c);
    func_0200acac(saved0280);
}

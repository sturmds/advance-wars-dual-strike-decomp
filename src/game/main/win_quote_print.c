#include "types.h"
#include "game/script_slot.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7a9c.h"

s32 HwMod32(s32 numerator, s32 denominator);
u32 Rng_Next(void);
ScriptSlot *Text_PrintByIdOnLayer0(int a, int b, u16 *map, u32 d, u16 e, u16 f);
u32 func_020e1840(int coId, int partnerId, int speaker, int pick);
u32 func_020e18dc(int coId);

extern Unk_027c0280 *data_027c0280;

/* Prints the win quote of result record `record`. A tag pair gets two
 * lines, one per CO, both from the same random pick of two; a single CO
 * gets one of its three win quotes (func_020e18dc). */
void WinQuote_Print(int record)
{
    int pick = HwMod32(Rng_Next(), 2);
    ScriptSlot *slot;

    if (data_022a7aba[record].unk_00 & 0x7f) {
        slot = Text_PrintByIdOnLayer0(1, 13, data_027c0280->layer0Map,
                                      func_020e1840(data_022a7ab8[record].unk_00 & 0x7f,
                                                    data_022a7aba[record].unk_00 & 0x7f, 0, pick),
                                      0, 0x41);
        slot->unk_3e = 2;
        slot = Text_PrintByIdOnLayer0(1, 18, data_027c0280->layer0Map,
                                      func_020e1840(data_022a7ab8[record].unk_00 & 0x7f,
                                                    data_022a7aba[record].unk_00 & 0x7f, 1, pick),
                                      0, 0x81);
        slot->unk_3e = 2;
    } else {
        slot = Text_PrintByIdOnLayer0(1, 18, data_027c0280->layer0Map,
                                      func_020e18dc(data_022a7ab8[record].unk_00 & 0x7f), 0, 0x41);
        slot->unk_3e = 2;
    }
}

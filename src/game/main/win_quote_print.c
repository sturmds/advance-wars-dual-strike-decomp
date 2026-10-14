#include "types.h"
#include "game/text_printer.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7a9c.h"
#include "game/co_data.h"

s32 HwMod32(s32 numerator, s32 denominator);
u32 Rng_Next(void);
TextPrinter* Text_PrintByIdOnLayer0(int x, int y, u16* map, u32 textId,
                                    u16 unk_30, u16 unk_38);
u32 Co_GetPairWinQuote(int coId, int partnerId, int speaker, int pick);
u32 Co_PickWinQuote(int coId);

extern Unk_027c0280* data_027c0280;

/* Prints the win quote of result record `record`. A tag pair gets two
 * lines, one per CO, both from the same random pick of two; a single CO
 * gets one of its three win quotes (Co_PickWinQuote). */
void WinQuote_Print(int record) {
  int pick = HwMod32(Rng_Next(), 2);
  TextPrinter* printer;

  if (data_022a7aba[record].unk_00 & CO_ID_MASK) {
    printer = Text_PrintByIdOnLayer0(
        1, 13, data_027c0280->layer0Map,
        Co_GetPairWinQuote(data_022a7ab8[record].unk_00 & CO_ID_MASK,
                           data_022a7aba[record].unk_00 & CO_ID_MASK, 0, pick),
        0, 0x41);
    printer->charDelay = 2;
    printer = Text_PrintByIdOnLayer0(
        1, 18, data_027c0280->layer0Map,
        Co_GetPairWinQuote(data_022a7ab8[record].unk_00 & CO_ID_MASK,
                           data_022a7aba[record].unk_00 & CO_ID_MASK, 1, pick),
        0, 0x81);
    printer->charDelay = 2;
  } else {
    printer = Text_PrintByIdOnLayer0(
        1, 18, data_027c0280->layer0Map,
        Co_PickWinQuote(data_022a7ab8[record].unk_00 & CO_ID_MASK), 0, 0x41);
    printer->charDelay = 2;
  }
}

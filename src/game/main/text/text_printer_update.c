#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"
#include "game/text_printer.h"

#define KEY_CONFIRM 0x10               /* A, or a touch in the text window */
#define KEY_SKIP    0x400              /* Start */

extern Unk_02080440 *data_027c027c;
extern u8 gEventSkipState;
extern ScriptCommand data_02158364[];
extern ScriptCommand data_021584f8[];
extern ScriptCommand data_02158520[];

u32 func_02024a40(void);
int Touch_TestPressInScreen(void);
s8 ScriptSlot_Find(const ScriptCommand *script);
void ScriptSlot_Stop(int slot);
BOOL func_02014b28(void);
int func_02015ec8(TextPrinter *printer);
void func_0201d730(int a);
void func_02015da8(TextPrinter *printer);
u16 *TextPrinter_GetCursorEntry(TextPrinter *printer);
int TextPrinter_ReadControlCode(TextPrinter *printer, u16 *entry);
BOOL TextPrinter_AdvancePixels(TextPrinter *printer, int pixels);
void TextPrinter_WriteTallEntries(TextPrinter *printer, u16 *entry);
int func_0202083c(int c, u8 *tiles, int x, int colour);
void Sound_PlaySe(u32 soundId);

#define ADVANCE(n) (*(const u8 **)&printer->text += (n))

/* Per-frame update of a text printer slot. Waits while one of the three
 * text effect scripts runs. Start skips the event (gEventSkipState = 1) and
 * stops the printer, if func_02014b28 allows it. A or a touch makes a slow
 * printer show the rest at once (func_02015da8). Otherwise unk_3d counts
 * frames up to unk_3e, and each time it wraps one character is drawn; while
 * unk_3d is negative, characters are drawn until a code that waits. unk_3c
 * set plays a sound per character. */
void TextPrinter_Update(TextPrinter *printer)
{
    u16 keys = func_02024a40() | Touch_TestPressInScreen();
    u16 *entry;
    int width;

    if (ScriptSlot_Find(data_02158364) != -1) {
        return;
    }
    if (ScriptSlot_Find(data_021584f8) != -1) {
        return;
    }
    if (ScriptSlot_Find(data_02158520) != -1) {
        return;
    }
    if (printer->unk_3e >= 1 && func_02014b28() && (keys & KEY_SKIP) && gEventSkipState == 0) {
        gEventSkipState = 1;
        func_0201d730(func_02015ec8(printer));
        ((void (*)(void))printer->unk_40)();
        ScriptSlot_Stop(*data_027c027c->scriptSlotCurrent);
        return;
    }
    if (printer->unk_3e > 1 && (keys & KEY_CONFIRM)) {
        func_02015da8(printer);
    }
    printer->unk_3d++;
    if (printer->unk_3d >= 0 && printer->unk_3d < printer->unk_3e) {
        return;
    }
    if (printer->unk_3d >= 0) {
        printer->unk_3d = 0;
    }
    ((void (*)(void))printer->unk_40)();
    do {
        entry = TextPrinter_GetCursorEntry(printer);
        switch (TextPrinter_ReadControlCode(printer, entry)) {
        case 0:
            ScriptSlot_Stop(*data_027c027c->scriptSlotCurrent);
            return;
        case 1:
            break;
        case 3:
            if (printer->unk_3d < 0) {
                continue;
            }
            /* fall through */
        case 2:
            return;
        }
        if (printer->unk_46 == 1 || printer->cursorX != printer->startX || printer->unk_44 != 0) {
            TextPrinter_AdvancePixels(printer, 1);
        }
        width = func_0202083c(*(const u8 *)printer->text, printer->tiles + printer->unk_38 * 32, printer->unk_44, printer->unk_32);
        printer->unk_46++;
        if (printer->unk_44 <= 1) {
            TextPrinter_WriteTallEntries(printer, TextPrinter_GetCursorEntry(printer));
        }
        if (TextPrinter_AdvancePixels(printer, width)) {
            TextPrinter_WriteTallEntries(printer, TextPrinter_GetCursorEntry(printer));
        }
        if (printer->unk_3c != 0) {
            Sound_PlaySe(0x10003);
        }
        ADVANCE(1);
    } while (printer->unk_3d < 0);
}

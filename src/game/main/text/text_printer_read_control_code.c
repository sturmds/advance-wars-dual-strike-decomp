#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"
#include "game/text_printer.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u8 data_02158364[];
extern u8 data_021584f8[];
extern u8 data_02158520[];

s32 HwDiv32(s32 numer, s32 denom);
ScriptSlot *ScriptSlot_Start(const void *descriptor, int arg);
int TextPrinter_ReadControlCode(TextPrinter *printer, u16 *entry);
int func_02015ec8(TextPrinter *printer);
void func_02015da8(TextPrinter *printer);
void func_0201d730(void);
void PopupMenu_SetItemIcon(int arg);
void DialogueCursor_Animate(void);
void UnitIcon_DrawEntries(u16 *dest, int unitType, int base, int cargo, int a4, int hp, int a6, int a7, int a8, int a9, int a10);

#define TEXT ((const u8 *)printer->text) /* TextPrinter.text is a bare pointer */
#define ADVANCE(n) (*(const u8 **)&printer->text += (n))
#define STATE (data_027c027c->unk_48[data_027c0284])

/* Handles the control code at the printer's text position; entry is the
 * screen map cell at the cursor. Bytes 16 to 19 set TextPrinter.unk_32 and
 * are skipped first. Returns 0 when the text ends, 1 when the byte is a
 * character the caller draws, 2 or 3 when a code was handled (2: the
 * caller goes on with the next cell after the code's effect). The codes
 * seen: 9 plays a sound (PopupMenu_SetItemIcon), 10 draws the HP and icon of a
 * unit carried by the acting unit, 11 sets unk_3e, 12 and 15 return to the
 * line start (15 also marks the next cell), 13 is a line break, 14 starts
 * the effect in data_02158364 while unk_3e is set, 20 to 23 start the effect
 * in data_02158520 and move the cursor 8 tiles right. */
int TextPrinter_ReadControlCode(TextPrinter *printer, u16 *entry)
{
    const u8 *text;
    ScriptSlot *slot;
    MapUnit *unit;
    int hp;

    for (;;) {
        text = TEXT;
        switch (text[0]) {
        case 16:
        case 17:
        case 18:
        case 19:
            printer->unk_32 = text[0] - 16;
            ADVANCE(1);
            continue;
        }
        break;
    }

    switch (text[0]) {
    case 0:
        if (printer->resumeText == NULL) {
            return 0;
        }
        printer->text = printer->resumeText;
        printer->resumeText = NULL;
        return TextPrinter_ReadControlCode(printer, entry);
    case 21:
        return 1; /* needed to match: separate from default */
    case 20:
    case 22:
    case 23:
        slot = ScriptSlot_Start(data_02158520, 0);
        slot->textEntry = entry + 1;
        slot->unk_28 = func_02015ec8(printer);
        slot->unk_2c = printer->unk_40;
        slot->unk_30 = printer->unk_30;
        if (TEXT[0] == 23 || TEXT[0] == 23) /* needed to match: the original tests twice */ {
            slot->unk_1e = 1;
            ADVANCE(1);
        } else {
            slot->unk_1e = 0;
            ADVANCE(1);
        }
        printer->cursorX += 8;
        return 2;
    case 13:
        printer->cursorY += 2;
        printer->cursorX = printer->startX;
        printer->unk_38 += 2;
        printer->unk_44 = 0;
        ADVANCE(1);
        return 3;
    case 15:
        entry[1] = 0;
        entry[33] = printer->unk_30 | 0x1b0;
        ADVANCE(1);
        printer->cursorX = printer->startX;
        printer->cursorY = printer->startY;
        printer->unk_38 = printer->unk_3a;
        printer->unk_44 = 0;
        slot = ScriptSlot_Start(data_021584f8, 0);
        slot->textEntry = entry + 1;
        slot->unk_28 = func_02015ec8(printer);
        slot->unk_2c = printer->unk_40;
        DialogueCursor_Animate();
        return 2;
    case 12:
        ADVANCE(1);
        printer->cursorX = printer->startX;
        printer->cursorY = printer->startY;
        printer->unk_38 = printer->unk_3a;
        printer->unk_44 = 0;
        func_02015ec8(printer);
        func_0201d730();
        ((void (*)(void))printer->unk_40)();
        return 2;
    case 14:
        ADVANCE(1);
        if (printer->unk_3e == 0) {
            return 3;
        }
        slot = ScriptSlot_Start(data_02158364, 0);
        slot->unk_1e = 10;
        return 2;
    case 11:
        ADVANCE(1);
        switch (TEXT[0]) {
        case 16:
            func_02015da8(printer);
            ADVANCE(1);
            break;
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
            printer->unk_3e = TEXT[0] - 16;
            printer->unk_3d = 0;
            ADVANCE(1);
            break;
        }
        return 3;
    case 10:
        if (text[1] == 17) {
            unit = &STATE->units[STATE->actingUnit->cargo[0]];
            hp = unit->hp == 0 ? 0 : HwDiv32(unit->hp - 1, 10) + 1;
            UnitIcon_DrawEntries(printer->screen + printer->cursorX + printer->cursorY * 32, unit->type,
                          STATE->currentUnitBase, unit->cargo[0], 0, hp, 0, 0, 0, 0, 0);
            printer->cursorX += 2;
            ADVANCE(2);
            return 3;
        } else { /* needed to match: the original has both copies in full */
            unit = &STATE->units[STATE->actingUnit->cargo[1]];
            hp = unit->hp == 0 ? 0 : HwDiv32(unit->hp - 1, 10) + 1;
            UnitIcon_DrawEntries(printer->screen + printer->cursorX + printer->cursorY * 32, unit->type,
                          STATE->currentUnitBase, unit->cargo[0], 0, hp, 0, 0, 0, 0, 0);
            printer->cursorX += 2;
            ADVANCE(2);
            return 3;
        }
    case 9:
        if (text[1] >= 128 && text[1] <= 153) {
            PopupMenu_SetItemIcon(text[1] - 128);
        }
        if (TEXT[1] >= 154 && TEXT[1] <= 179) {
            PopupMenu_SetItemIcon(TEXT[1] - 128);
        }
        printer->cursorX += 2;
        ADVANCE(2);
        return 3;
    default:
        return 1;
    }
}

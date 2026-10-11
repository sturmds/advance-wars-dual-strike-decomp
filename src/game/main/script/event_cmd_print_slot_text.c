#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/Unk_022977a4.h"
#include "game/event_script.h"
#include "game/text_printer.h"

extern Unk_0201d320 data_022977f4[];
extern EventSlotHold data_022977f8[];
extern EventSlotText data_02297804[];
extern u8 *data_02297790;
extern u16 gEventTextMap[];

TextPrinter *TextPrinter_StartById(int x, int y, u8 *tiles, u16 *map, u32 textId, u16 unk_30, u16 unk_38);
void EventTextMap_QueueCopy(void);
void EventHold_ReleaseOnTextDone(EventScriptSlot *slot);

/* Event script command: prints the slot's text id at tile (7, 1) of the text map buffer
 * gEventTextMap with the current palette slot. When the printer ends it runs
 * EventTextMap_QueueCopy to upload the buffer. The script is held until printing is done
 * (EventHold_ReleaseOnTextDone), and the slot's run ends for this frame. */
BOOL EventCmd_PrintSlotText(s16 index)
{
    TextPrinter *printer = TextPrinter_StartById(7, 1, data_02297790, gEventTextMap, data_02297804[index].textId,
                                                 data_022977a4.unk_00 << 12, data_022977a4.unk_04);

    printer->copyCallback = (void *)EventTextMap_QueueCopy;
    printer->charSound = 1;
    data_022977f8[index].hold = (u32)EventHold_ReleaseOnTextDone;
    data_022977f4[index].unk_00++;
    return 0;
}

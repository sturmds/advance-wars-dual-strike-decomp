#include "types.h"
#include "game/event_script.h"
#include "game/Unk_022977a4.h"

/* Fields of the 16-byte records MainMenu_ShowRecordHelp also shows. */
extern u8 data_02290a62[][16]; /* 1: draws an extra mark */
extern u8 data_02290a64[][16]; /* a number; 0xff: none */
extern u16 data_02290a66[][8]; /* a second number; 0: not shown */

void EventText_ClearLines(u16* tile, int value);
BOOL Profile_IsInUse(int index);
s8 Text_PrintId(s16 x, s16 y, u16* map, u32 textId, u16 unk_30, u16 unk_32);
void Number_Draw(s16 x, s16 y, u16* map, int value, u16 a, u16 b);

/* Clears the choice line at tile (x, y) of the event text window with
 * EventText_ClearLines, then, unless index is the last choice (3), prints the line
 * of record index: a text picked by which of its numbers are set, then the
 * numbers. When Profile_IsInUse says the record is not there, prints text
 * 0xc00002df instead. Texts use the event script palette slot. */
void EventChoice_DrawRecord(int index, int x, int y) {
  u32 textId;
  u8 number;
  u8 mark;
  int second;

  EventText_ClearLines(&gEventTextMap[y][x], 1);
  if (index == 3) {
    return;
  }
  if (Profile_IsInUse(index)) {
    number = data_02290a64[index][0];
    mark = data_02290a62[index][0];
    second = data_02290a66[index][0];
    if (number == 0xff) {
      textId = (mark == 1) ? 0xc00002fa : 0xc00002f9;
    } else if (mark == 1) {
      textId = (second == 0) ? 0xc00002e3 : 0xc00002e1;
    } else {
      textId = (second == 0) ? 0xc00002e2 : 0xc00002e0;
    }
    Text_PrintId(x, y, gEventTextMap[0], textId, data_022977a4.unk_00 << 12,
                 data_022977a4.unk_04 + 50);
    if (number == 0xff) {
      return;
    }
    Number_Draw(x + 12, y, gEventTextMap[0], number, data_022977a4.unk_00 << 12,
                0);
    if (second <= 0) {
      return;
    }
    Number_Draw(x + 19, y, gEventTextMap[0], second, data_022977a4.unk_00 << 12,
                0);
  } else {
    Text_PrintId(x, y, gEventTextMap[0], 0xc00002df, data_022977a4.unk_00 << 12,
                 data_022977a4.unk_04 + 50);
  }
}

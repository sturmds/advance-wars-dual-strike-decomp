#include "types.h"
#include "game/Unk_02160abc.h"
#include "game/Unk_02160ac0.h"
#include "game/ov003_unit.h"
#include "game/play_records.h"

void func_ov003_0236249c(void);

#define STATE data_ov003_02364928
#define LINK_ROW(i) (((Unk_02160ac0_Row*)data_02160ac0)[i])
#define RECORD_MAX 999999

/* Byte 1 of an Ov003Slot. The last loop reads the slot row through
 * STATE + 0x139 kept in a register, as the original does. */
typedef struct RowView {
  u8 row : 4;
  u8 hi : 4;
  u8 pad[3];
} RowView;

/* Copies what overlay 3 knows about a row (1 to 5) into data_02160abc: the
 * row number, the co and color of the row and of the other of rows 1 and
 * 2, and Ov003Row.unk_28. For link play (data_02160ac0->unk_7c clear) and row 1
 * it also copies the cells of every row, with units (state 2) as state 1.
 * Then it updates two play counters. Not matched yet (97.2%): in the last
 * loop the original keeps the slot base in r0 and the total in r6. */
void func_ov003_02361c7c(int row) {
  u8 other;
  int i;
  int j;
  RowView* slots;

  if (row != 1) {
    other = 1;
  } else {
    other = 2;
  }
  data_02160abc->unk_0a = row;
  data_02160abc->unk_0b = STATE->rows[row - 1].co;
  data_02160abc->unk_0d = STATE->rows[row - 1].color;
  data_02160abc->unk_0c = STATE->rows[other - 1].co;
  data_02160abc->unk_0e = STATE->rows[other - 1].color;
  for (i = 0; i < 10; i++) {
    data_02160abc->unk_00[i] = STATE->rows[row - 1].unk_28[i];
  }
  if (data_02160ac0->unk_7c == 0 && data_02160abc->unk_0a == 1) {
    for (i = 1; i < STATE->rowCount + 1; i++) {
      LINK_ROW(i - 1).spawnCount = STATE->rows[i - 1].spawnCount;
      for (j = 0; j < STATE->rows[i - 1].spawnCount; j++) {
        LINK_ROW(i - 1).cells[j].tile = STATE->rows[i - 1].cells[j].kind;
        LINK_ROW(i - 1).cells[j].state = STATE->rows[i - 1].cells[j].state;
        if (LINK_ROW(i - 1).cells[j].state == 2) {
          LINK_ROW(i - 1).cells[j].state = 1;
        }
      }
    }
  }
  if (row != STATE->slots[STATE->currentSlot].row) {
    if (gPlayRecords.unk_440 < RECORD_MAX) {
      gPlayRecords.unk_440++;
    }
  }
  slots = (RowView*)((u8*)STATE + 0x139);
  for (i = 0; i < 6; i++) {
    u32 total;
    int slotRow = slots[STATE->currentSlot].row;

    total = gPlayRecords.unk_43c + STATE->rows[slotRow - 1].unk_28[i];
    if (total < RECORD_MAX) {
      gPlayRecords.unk_43c = total;
    } else {
      gPlayRecords.unk_43c = RECORD_MAX;
    }
  }
  func_ov003_0236249c();
}

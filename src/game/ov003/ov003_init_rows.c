#include "types.h"
#include "game/ov003_unit.h"
#include "game/Unk_02160ac0.h"
#include "game/Unk_02160abc.h"

extern Ov003Preset data_ov003_02364e50[];

int func_0207a260(void);
u16 Wireless_GetChildAids(void);

#define STATE data_ov003_02364928
#define SETUP data_02160ac0

/* Ov003Row.co values the preset uses for "decide at start". */
#define ROW_UNK_3B_FROM_SETUP 8  /* take Unk_02160ac0_Row.unk_19 of the row */
#define ROW_UNK_3B_FROM_ROW0_A 9 /* derived from row 0's value */
#define ROW_UNK_3B_FROM_ROW0_B 10

/* Sets up the rows and the 8 slots at the start of overlay 3. The preset
 * (entry data_02160ac0->unk_7d of data_ov003_02364e50) gives the row count and each row's colour and
 * co. In link play (unk_7c set) the slot rows come from
 * data_02160ac0->unk_70, the rows of the connected slots (Wireless_GetChildAids, plus
 * slot 0) are active instead, and row 5 is always active. Active rows among
 * the first four get unk_3b_4 = 0, 2, 4, 6 in order, shifted so the group
 * sits in the middle. Settings at 0xff in data_02160ac0 fall back to the
 * preset. Needed to match: n is both the inner loop counter and the count of
 * active rows. */
void Ov003_InitRows(void) {
  int i;
  int n;

  STATE->preset = &data_ov003_02364e50[SETUP->unk_7d];
  STATE->unk_1df_1 = 1;
  STATE->currentSlot = SETUP->unk_7c ? func_0207a260() : 0;
  STATE->activeRows = 0;
  data_02160abc->unk_0a = 0;
  for (i = 0; i < 5; i++) {
    STATE->rows[i].charge = 0;
    STATE->rows[i].powerTimer = 0;
    STATE->rows[i].co = 0;
    STATE->rows[i].color = i + 1;
    STATE->rows[i].active = 0;
    STATE->rows[i].unk_3b_4 = 0;
    STATE->rows[i].unk_38 = 0xff;
    STATE->rows[i].unk_39 = 0xff;
    for (n = 0; n < 10; n++) {
      STATE->rows[i].unk_28[n] = 0;
    }
  }
  for (i = 0; i < 8; i++) {
    STATE->slots[i].cursor = 0;
    if (SETUP->unk_7c) {
      STATE->slots[i].row = SETUP->unk_70[i];
    } else {
      STATE->slots[i].row = 1;
    }
    STATE->slots[i].state = 0;
  }

  STATE->rowCount = STATE->preset->rowCount;
  STATE->row = (u8)(int)STATE->preset
                   ->rowCount; /* needed to match: the cast adds `and 0xff` */
  for (i = 0; i < STATE->rowCount; i++) {
    STATE->rows[i].co = STATE->preset->unk_0c[i];
    STATE->rows[i].color = STATE->preset->colors[i];
    STATE->rows[i].active = 1;
    STATE->activeRows |= 1 << i;
  }

  if (SETUP->unk_7c == 0) {
    STATE->slots[STATE->currentSlot].state = 2;
  } else {
    STATE->activeRows = 0;
    for (i = 0; i < 5; i++) {
      STATE->rows[i].active = 0;
    }
    for (i = 0; i < 8; i++) {
      if ((Wireless_GetChildAids() | 1) & (1 << i)) {
        STATE->slots[i].state = 2;
        STATE->rows[STATE->slots[i].row - 1].active = 1;
        STATE->activeRows |= 1 << (STATE->slots[i].row - 1);
      }
    }
    STATE->row = 5;
    STATE->rows[4].active = 1;
    STATE->rows[4].color = 5;
  }

  n = 0;
  for (i = 0; i < 4; i++) {
    if (STATE->rows[i].active) {
      STATE->rows[i].unk_3b_4 = n * 2;
      n++;
    }
  }
  switch (n) {
    case 2:
      n = 2;
      break;
    case 3:
      n = 1;
      break;
    case 4:
      n = 0;
      break;
  }
  for (i = 0; i < 4; i++) {
    if (STATE->rows[i].active) {
      STATE->rows[i].unk_3b_4 += n;
    }
  }

  if (SETUP->unk_7f == 0xff) {
    STATE->unk_1df_0 = STATE->preset->unk_10_3;
  } else {
    STATE->unk_1df_0 = SETUP->unk_7f;
  }
  if (SETUP->unk_82 == 0xff) {
    STATE->unk_1df_2 = STATE->preset->unk_10_4;
  } else {
    STATE->unk_1df_2 = SETUP->unk_82;
  }
  if (SETUP->unk_81 == 0xff) {
    STATE->unk_1dc = STATE->preset->unk_11;
  } else {
    STATE->unk_1dc = SETUP->unk_81;
  }
  switch (SETUP->unk_84) {
    case 0:
      STATE->unk_1df_5 = 0;
      break;
    case 1:
      STATE->unk_1df_5 = 1;
      break;
    case 2:
      STATE->unk_1df_5 = 1;
      STATE->unk_1df_0 = 1;
      STATE->unk_1df_2 = 2;
      break;
  }
  STATE->unk_1df_6 = SETUP->unk_80;
  STATE->nextSpawn = 0;
  STATE->unk_1d9 = 0;
  STATE->unk_1db_7 = 0;
  STATE->phases = (STATE->phases & ~0x0f) | 3;
  STATE->phases = (STATE->phases & ~0xf0) | 0x30;
  STATE->frameCount = 0;
  STATE->unk_1cc = 0;
  for (i = 0; i < 8; i++) {
    STATE->unk_1c0[i] = 0;
  }

  for (i = 0; i < STATE->rowCount; i++) {
    switch (STATE->rows[i].co) {
      case ROW_UNK_3B_FROM_SETUP:
        STATE->rows[i].co = LINK_ROWS[i].unk_19;
        break;
      case ROW_UNK_3B_FROM_ROW0_A:
        switch (STATE->rows[0].co) {
          case 1:
          case 6:
            STATE->rows[i].co = 3;
            break;
          case 2:
          case 5:
          case 7:
            STATE->rows[i].co = 1;
            break;
          case 3:
          case 4:
            STATE->rows[i].co = 2;
            break;
          default:
            STATE->rows[i].co = 1;
            break;
        }
        break;
      case ROW_UNK_3B_FROM_ROW0_B:
        switch (STATE->rows[0].co) {
          case 1:
          case 5:
            STATE->rows[i].co = 2;
            break;
          case 2:
          case 4:
          case 7:
            STATE->rows[i].co = 3;
            break;
          case 3:
          case 6:
            STATE->rows[i].co = 1;
            break;
          default:
            STATE->rows[i].co = 2;
            break;
        }
        break;
    }
  }
}

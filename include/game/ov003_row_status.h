#ifndef GAME_OV003_ROW_STATUS_H
#define GAME_OV003_ROW_STATUS_H

#include "types.h"
#include "game/ov003_unit.h"

/* The same 62 bytes as Ov003Row, seen from the fields after the cells.
 * Rows are numbered 1 to 5. */
typedef struct Ov003RowStatus {
    u8 pad_00[0x32];
    u16 charge;                      /* filled by Ov003Row_AddGauge, up to a limit per kind */
    u16 powerTimer;                  /* not 0 while the power is active */
    u16 unk_36;
    u8 pad_38[3];
    u8 kind : 4;                       /* indexes the tables at data_ov003_02364cdc and data_ov003_02364cde */
    u8 unk_3b_4 : 4;
    u8 pad_3c[2];
} Ov003RowStatus;

typedef char Ov003RowStatus_size_check[sizeof(Ov003RowStatus) == 62 ? 1 : -1];

#define OV003_ROW_STATUS(row) (((Ov003RowStatus *)data_ov003_02364928->rows)[(row) - 1])

#endif

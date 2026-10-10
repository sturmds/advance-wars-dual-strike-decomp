#ifndef GAME_UNK_OV005_0236BBA8_H
#define GAME_UNK_OV005_0236BBA8_H

#include "types.h"

struct Ov005Task;

/* One cell of the CO picture screen's grid, 16 bytes. The grid is 4
 * columns by 7 rows (data_ov005_0236ac90). */
typedef struct Ov005CoGridEntry {
    s16 index;
    s16 coId;                          /* -1: empty cell */
    s16 x;                             /* pixels */
    s16 y;
    s16 column;
    s16 row;
    s16 available;                     /* 1 when func_02045830 accepts coId */
    s16 pad_0e;
} Ov005CoGridEntry;

#define OV005_CO_GRID_ENTRIES 28

/* CO picture screen work block (0x2f4 bytes) that func_ov005_02362618
 * allocates into Ov005Task.work and data_ov005_0236bba8. */
typedef struct Unk_ov005_0236bba8 {
    u32 unk_00;
    u8 pad_04[0x20 - 0x04];
    u32 coId;                          /* low 7 bits: CO shown */
    u32 cursorCoId;                    /* coId of the grid cell at cursor */
    int unk_28;                        /* palette variant of coId */
    int unk_2c;
    u32 cursor;                        /* grid cell index */
    u32 unk_34;
    u32 unk_38;
    Ov005CoGridEntry grid[OV005_CO_GRID_ENTRIES]; /* copy of data_ov005_0236ac90 */
    fx32 unk_1fc;
    fx32 unk_200;
    fx32 unk_204;
    struct Ov005Task *childTask;       /* runs data_ov005_0236ab6c with the same work block */
    int unk_20c;                       /* palette variant of the CO */
    int unk_210;                       /* same value */
    int unk_214;                       /* palette variant, 0 when it is 8 */
    void *spriteFile;                  /* rikiishi file 042; source of the unk_21c sprites */
    void *unk_21c[9];
    u8 pad_240[0x288 - 0x240];
    u8 unk_288[0x6c];                  /* copy of data_ov005_0236ac24 */
} Unk_ov005_0236bba8;

#endif

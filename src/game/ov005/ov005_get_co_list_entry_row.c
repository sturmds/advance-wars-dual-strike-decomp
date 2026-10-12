#include "types.h"

/* Four bytes per entry; this reads the byte at offset 3 of entry
 * `index` (the table starts at data_ov005_0236a5a0). */
extern u8 data_ov005_0236a5a3[][4];

u8 Ov005_GetCoListEntryRow(int index) { return data_ov005_0236a5a3[index][0]; }

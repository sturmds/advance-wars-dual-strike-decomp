#include "types.h"

void __read_console(void);
void __write_console(void);
extern u8 data_022ad224[];
extern u8 data_022ad324[];
extern u8 data_022ad424[];
void func_021295f8(void);

/* declared `FILE gMslFiles[FILE_COUNT]` in msl_file.h: FileModes.openMode is a bit field */
u32 gMslFiles[57] = {
#include "data/main/gMslFiles.inc.c"
};

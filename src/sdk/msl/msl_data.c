#include "types.h"
#include "sdk/msl_file.h"

void __read_console(void);
void __write_console(void);
extern u8 data_022ad224[];
extern u8 data_022ad324[];
extern u8 data_022ad424[];
void func_021295f8(void);

FILE gMslFiles[] = {
#include "data/main/gMslFiles.inc.c"
};

#include "types.h"

extern char data_ov000_02350470[];
extern char data_ov000_02313370[];

char data_ov000_02313358[] = {
#include "data/ov000/data_ov000_02313358.inc.c"
};
/* declared `u8 data_ov000_02313364[]` in ov000_data_022f6bd8.c: relocation at data_ov000_02313364+0x0 is not on a 4-byte field */
void* data_ov000_02313364[] = {&data_ov000_02313358, &data_ov000_02350470,
                               &data_ov000_02313370};
char data_ov000_02313370[] = {
#include "data/ov000/data_ov000_02313370.inc.c"
};

#include "types.h"

extern u8 data_ov000_02350470[];
extern char data_ov000_02313370[];

char data_ov000_02313358[9] = {
#include "data/ov000/data_ov000_02313358.inc.c"
};
/* declared `u8 data_ov000_02313364[]` in ov000_data_022f6bd8.c: relocation at data_ov000_02313364+0x0 is not on a 4-byte field */
void* data_ov000_02313364[3] = {&data_ov000_02313358, &data_ov000_02350470,
                                &data_ov000_02313370};
char data_ov000_02313370[30] = {
#include "data/ov000/data_ov000_02313370.inc.c"
};

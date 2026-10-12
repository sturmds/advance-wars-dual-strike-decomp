#include "sdk/wmsp.h"

void WMSP_VAlarmLine200(void);
void WMSP_VAlarmLine208(void);
void WMSP_VAlarmLine209(void);
void WMSP_VAlarmLine240(void);
void WMSP_VAlarmLine260(void);

/* The V-count alarm handler: each step's alarm passes its line as the argument. */
void WMSP_VAlarmHandler(void *line)
{
    switch ((u32)line) {
    case 200:
        WMSP_VAlarmLine200();
        break;
    case 208:
        WMSP_VAlarmLine208();
        break;
    case 240:
        WMSP_VAlarmLine240();
        break;
    case 209:
        WMSP_VAlarmLine209();
        break;
    case 260:
        WMSP_VAlarmLine260();
        break;
    }
}

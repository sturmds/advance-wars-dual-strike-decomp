/* Needed to match: the library was built optimizing for size. */
#pragma optimize_for_size on
#include "types.h"
#include "sdk/isd.h"

void Isd_VFormat(IsdPrintTarget *target, const char *format, char **args);

/* Formats to the debugger when debugger output is on. The va_list is built by hand: a pointer to the word after format. */
void Isd_Printf(const char *format, ...)
{
    IsdPrintTarget target;
    char *args;

    if (Isd_TestFlags(ISD_FLAG_OUTPUT)) {
        target.unk_00 = 0;
        target.stored = 0;
        target.limit = -1;
        target.write = Isd_PrintTargetWrite;
        args = (char *)(((u32)&format & ~3) + 4);
        Isd_VFormat(&target, format, &args);
    }
}

/* Needed to match: the library was built optimizing for size. */
#pragma optimize_for_size on
#include "types.h"
#include "sdk/isd.h"

extern char gIsdAssertionFormat[];
extern char gIsdWarningFormat[];

void func_02121bc8(void);

/* Prints the assertion or warning line for a failed check; a fatal one then traps. */
void Isd_ReportFailure(const char *file, int line, const char *expression, BOOL isFatal)
{
    if (isFatal) {
        Isd_Printf(gIsdAssertionFormat, file, line, expression);
        func_02121bc8();
    } else {
        Isd_Printf(gIsdWarningFormat, file, line, expression);
    }
}

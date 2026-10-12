#include "types.h"
#include "sdk/mb.h"

extern DlPlayWorkerArea *data_022acf00;

/* TRUE once the task thread has started. */
BOOL MbTask_IsAvailable(void)
{
    return (data_022acf00 != NULL) ? 1 : 0;
}

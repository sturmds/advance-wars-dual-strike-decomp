#include "types.h"
#include "game/object_heal_task.h"

/* Step of script data_02167fb8: starts the scan from the first map object. */
void ObjectHealScan_Reset(ObjectHealScanTask* task) { task->nextObject = 0; }

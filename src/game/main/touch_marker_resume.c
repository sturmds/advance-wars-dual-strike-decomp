#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd gTouchMarkerScript[];

void Task_ResumeByScript(const TaskScriptCmd* script);

void TouchMarker_Resume(void) { Task_ResumeByScript(gTouchMarkerScript); }

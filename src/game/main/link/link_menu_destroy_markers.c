#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd gLinkMarkerScript0[];
extern const TaskScriptCmd gLinkMarkerScript1[];
extern const TaskScriptCmd gLinkMarkerScript2[];
void Task_DestroyByScript(const TaskScriptCmd* script);

/* Ends the three marker tasks LinkMenu_CreateMarker starts. */
void LinkMenu_DestroyMarkers(void) {
  Task_DestroyByScript(gLinkMarkerScript0);
  Task_DestroyByScript(gLinkMarkerScript1);
  Task_DestroyByScript(gLinkMarkerScript2);
}

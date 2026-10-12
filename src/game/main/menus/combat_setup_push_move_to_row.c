#include "types.h"
#include "game/event_queue.h"

void CombatSetup_PushMoveToRow(int arg) { EventQueue_Push(11, arg); }

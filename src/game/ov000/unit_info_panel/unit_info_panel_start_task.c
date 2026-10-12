#include "types.h"
#include "game/task.h"
#include "game/unit_info_panel.h"

void Map_ScrollViewToTile(int arg);
void UnitInfoPanel_AddButtons(void);

extern const TaskScriptCmd data_ov000_0234ef28[];

/* Resets the panel state, scrolls the map view (argument 2) and starts the panel task under parent. */
void UnitInfoPanel_StartTask(void* parent) {
  data_ov000_0234ec8c->page = 0;
  data_ov000_0234ec8c->shownPage = -1;
  Map_ScrollViewToTile(2);
  Task_Create(data_ov000_0234ef28, parent);
  UnitInfoPanel_AddButtons();
}

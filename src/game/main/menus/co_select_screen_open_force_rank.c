#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_select_screen.h"

void ForceRankScreen_Start(int co, int gameMode, int unused, int taskList);

/* Opens the Force Rank screen for the CO of the cursor cell. */
void CoSelectScreen_OpenForceRank(int taskList) {
  u8* column = &data_0229c73c->coGrid[(data_0229c73c->cursorColumn - 1) * 2];

  ForceRankScreen_Start(column[data_0229c73c->cursorRow], MAP_STATE->gameMode,
                        0, taskList);
}

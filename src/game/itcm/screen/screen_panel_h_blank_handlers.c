#include "types.h"
#include "text_data.h"

void Screen_HBlankTopPanel(void);
void Screen_HBlankBottomPanel(void);

/* The values come from the ROM at build time (config/AWRE/data_inc.txt). */
#pragma section TEXT_DATA begin

/* H-blank handler of the panel on each screen, indexed by the screen
 * (EventScript_ShowPicture). */
void* gScreenPanelHBlankHandlers[2] = {
#include "data/itcm/gScreenPanelHBlankHandlers.inc.c"
};

#pragma section TEXT_DATA end

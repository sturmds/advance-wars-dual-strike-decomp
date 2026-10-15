#include "types.h"
#include "game/event_script.h"
#include "game/Unk_ov006_0235f690.h"

int DesignMap_CopyName(int id, u8* buf);
void Ov006_ClearLayer1Box(void);
void Ov006_ClearLayer0Box(void);
void Ov006_ClearWindowsAndBlend(void);
int Ov006_HasNonSpace(u8* buf);
void Ov006_LoadIdText(int id);

extern EventScriptCommand data_ov006_0236bb60[];
extern EventScriptCommand data_ov006_0236bc00[];
extern EventScriptCommand data_ov006_0236bca0[];
extern EventScriptCommand data_ov006_0236bd50[];

void Ov006_StartIdScript(int id) {
  Ov006_ClearLayer1Box();
  Ov006_ClearLayer0Box();
  Ov006_ClearWindowsAndBlend();
  data_ov006_0236d640->unk_00 &= ~0x100;
  data_ov006_0236d640->chosenSlot = id;
  if (DesignMap_CopyName(data_ov006_0236d640->chosenSlot,
                         &data_ov006_0236d640->unk_eb) == 0) {
    if (Ov006_HasNonSpace(data_ov006_0236d640->unk_d8) == 0) {
      Ov006_LoadIdText(id);
      data_ov006_0236d640->unk_00 |= 0x100;
      EventScript_Start(data_ov006_0236bca0);
    } else {
      EventScript_Start(data_ov006_0236bb60);
    }
  } else {
    if (Ov006_HasNonSpace(data_ov006_0236d640->unk_d8) == 0) {
      Ov006_LoadIdText(id);
      data_ov006_0236d640->unk_00 |= 0x100;
      EventScript_Start(data_ov006_0236bd50);
    } else {
      EventScript_Start(data_ov006_0236bc00);
    }
  }
}

#include "types.h"
#include "game/Unk_ov006_0235f690.h"

/* Stores a request code and sets flag 0x8000 to tell the screen about it. */
void Ov006_PostRequest(int request) {
  data_ov006_0236d640->request = request;
  data_ov006_0236d640->unk_00 |= 0x8000;
}

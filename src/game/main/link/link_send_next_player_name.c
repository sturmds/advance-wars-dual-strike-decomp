#include "types.h"
#include "game/Unk_0215fe8c.h"
#include "game/link_buffers.h"
#include "game/link_player_name.h"

extern Unk_0215fe8c* data_0215fe8c;

int Link_QueuePacket(LinkPacket* packet);
void Mem_CopyUnits(void* src, void* dest, int size, int unitSize);
void* Wireless_GetPlayerName(u8 index);
u16 Wireless_GetChildAids(void); /* connected child aid bits */

/* Parent side: moves on to the next connected child (aid 1 to 7, wrapping)
 * and sends its name, with the child aid bits, as a name packet. */
void Link_SendNextPlayerName(void) {
  LinkPacket packet;

  if (Wireless_GetChildAids() == 0) {
    data_0215fe8c->nameSendAid = 0;
  } else {
    do {
      data_0215fe8c->nameSendAid++;
      if (data_0215fe8c->nameSendAid >= LINK_AID_COUNT) {
        data_0215fe8c->nameSendAid = 1;
      }
    } while (Wireless_GetChildAids() != 0 &&
             !(Wireless_GetChildAids() & (1 << data_0215fe8c->nameSendAid)));
  }
  packet.header = data_0215fe8c->unk_0e & 0x7f;
  packet.header |= 0x80;
  packet.type = LINK_PACKET_PLAYER_NAME - 1 + data_0215fe8c->nameSendAid;
  packet.payload[0] = Wireless_GetChildAids();
  packet.payload[1] = Wireless_GetChildAids() >> 8;
  Mem_CopyUnits(Wireless_GetPlayerName(data_0215fe8c->nameSendAid),
                &packet.payload[2], sizeof(LinkPlayerName), 8);
  data_0215fe8c->unk_0e++;
  Link_QueuePacket(&packet);
}

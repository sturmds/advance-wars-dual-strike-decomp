#include "types.h"
#include "game/Unk_0215fe8c.h"
#include "game/link_buffers.h"

extern Unk_0215fe8c* data_0215fe8c;

int Wireless_GetState(void);     /* wireless state: 4 parent, 5 child */
u16 Wireless_GetChildAids(void); /* connected child aid bits */
int Wireless_TakeChildrenChanged(
    void);               /* true once after the children changed */
int func_02077f20(void); /* Unk_0215fe84.unk_18 */
void func_0207ab14(void);
void Link_SendNextPlayerName(void);
void Link_SendChildAids(void);
int MainLoop_GetFrameCount(void);
int Link_PopFlaggedPacket(u8 player, void* dest);
void Link_SendCommand(int type);
void Link_SendFlaggedPacket(void* data);
BOOL Wireless_StartDataSharing(void);
void Wireless_EndDataSharing(void);
void LinkActionQueue_Push(void* action);
void LinkReactionQueue_Push(void* data);

/* Parent side: handles the flagged packets of each child (aid 1 to 7). Stop
 * and the sharing acknowledgements are collected as bits per aid in stopAcks and
 * sharingAcks; once every connected child has answered, the parent acts and tells
 * the children. Unit actions and reactions are queued here and relayed to all
 * children. Then, when the children changed (and every 16th frame while
 * func_02077f20 is set), sends the child aid bits: with the next child's
 * name (Link_SendNextPlayerName) while func_02077f20 is set, else alone
 * (Link_SendChildAids). */
void Link_ParentHandlePackets(void) {
  LinkPacket packet;
  int aid;
  int bit;
  int i;

  if (Wireless_GetState() != 4) {
    return;
  }
  for (aid = 1; aid <= LINK_PEER_COUNT; aid++) {
    while (Link_PopFlaggedPacket(aid, &packet) == 1) {
      bit = 1 << aid;
      switch (packet.type) {
        case LINK_PACKET_STOP:
          data_0215fe8c->stopAcks |= bit;
          for (i = 1; i <= LINK_PEER_COUNT; i++) {
            if ((Wireless_GetChildAids() & (1 << i)) &&
                !(data_0215fe8c->stopAcks & (1 << i))) {
              break;
            }
          }
          if (i == 8) {
            data_0215fe8c->unk_0c = 0;
            Link_SendCommand(LINK_PACKET_STOP);
            func_0207ab14();
          }
          break;
        case LINK_PACKET_SHARING_STARTED:
          data_0215fe8c->sharingAcks |= bit;
          for (i = 1; i <= LINK_PEER_COUNT; i++) {
            if ((Wireless_GetChildAids() & (1 << i)) &&
                !(data_0215fe8c->sharingAcks & (1 << i))) {
              break;
            }
          }
          if (i == 8) {
            Wireless_StartDataSharing();
            data_0215fe8c->linkState = 2;
            Link_SendCommand(LINK_PACKET_SHARING_STARTED);
          }
          break;
        case LINK_PACKET_SHARING_ENDED:
          data_0215fe8c->sharingAcks |= bit;
          for (i = 1; i <= LINK_PEER_COUNT; i++) {
            if ((Wireless_GetChildAids() & (1 << i)) &&
                !(data_0215fe8c->sharingAcks & (1 << i))) {
              break;
            }
          }
          if (i == 8) {
            Wireless_EndDataSharing();
            data_0215fe8c->linkState = 0;
            Link_SendCommand(LINK_PACKET_SHARING_ENDED);
          }
          break;
        case LINK_PACKET_SHARING_START:
        case LINK_PACKET_SHARING_END:
        case LINK_PACKET_ACTION:
          /* needed to match: these cases make the jump table start at 0 */
          break;
        case LINK_PACKET_ACTION + 1:
        case LINK_PACKET_ACTION + 2:
        case LINK_PACKET_ACTION + 3:
          LinkActionQueue_Push(packet.payload);
          Link_SendFlaggedPacket(&packet);
          break;
        case LINK_PACKET_REACTION:
          LinkReactionQueue_Push(packet.payload);
          Link_SendFlaggedPacket(&packet);
          break;
      }
    }
  }
  if (Wireless_GetChildAids() == 0) {
    return;
  }
  if (Wireless_TakeChildrenChanged() != 1) {
    if (func_02077f20() != 1) {
      return;
    }
    if (MainLoop_GetFrameCount() & 15) {
      return;
    }
  }
  if (func_02077f20() == 1) {
    Link_SendNextPlayerName();
  } else {
    Link_SendChildAids();
  }
}

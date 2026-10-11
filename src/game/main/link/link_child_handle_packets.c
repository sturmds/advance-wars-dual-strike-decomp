#include "types.h"
#include "game/Unk_0215fe8c.h"
#include "game/link_buffers.h"

extern Unk_0215fe8c *data_0215fe8c;

int func_0207a27c(void);             /* wireless state: 4 parent, 5 child */
int func_0207a260(void);             /* this player's aid, it seems */
void func_0207a1c0(int childAids);
int Link_PopFlaggedPacket(int player, void *dest);
void Link_SendCommand(int type);
void Link_Stop(void);
BOOL Wireless_StartDataSharing(void);
void Wireless_EndDataSharing(void);
void Wireless_SetPlayerName(u8 aid, u8 *name);
void LinkActionQueue_Push(void *action);
void LinkReactionQueue_Push(void *data);

/* Child side: handles every flagged packet the parent sent. The child
 * acknowledges sharing start and end; unk_0b goes 1 (start acknowledged), 2,
 * 3 (end acknowledged), 0. */
void Link_ChildHandlePackets(void)
{
    LinkPacket packet;

    if (func_0207a27c() != 5) {
        return;
    }
    if (Link_PopFlaggedPacket(0, &packet) != 1) {
        return;
    }
    do {
        switch (packet.type) {
        case LINK_PACKET_STOP:
            Link_Stop();
            break;
        case LINK_PACKET_PLAYER_NAME + 0:
        case LINK_PACKET_PLAYER_NAME + 1:
        case LINK_PACKET_PLAYER_NAME + 2:
        case LINK_PACKET_PLAYER_NAME + 3:
        case LINK_PACKET_PLAYER_NAME + 4:
        case LINK_PACKET_PLAYER_NAME + 5:
        case LINK_PACKET_PLAYER_NAME + 6:
            /* a name packet also carries the child aid bits */
            Wireless_SetPlayerName(packet.type - LINK_PACKET_PLAYER_NAME + 1, &packet.payload[2]);
        case LINK_PACKET_CHILD_AIDS:
            func_0207a1c0((u16)(packet.payload[0] | (packet.payload[1] << 8)));
            break;
        case LINK_PACKET_SHARING_START:
            Wireless_StartDataSharing();
            data_0215fe8c->linkState = 1;
            Link_SendCommand(LINK_PACKET_SHARING_STARTED);
            break;
        case LINK_PACKET_SHARING_STARTED:
            data_0215fe8c->linkState = 2;
            break;
        case LINK_PACKET_SHARING_END:
            Wireless_EndDataSharing();
            data_0215fe8c->linkState = 3;
            Link_SendCommand(LINK_PACKET_SHARING_ENDED);
            break;
        case LINK_PACKET_SHARING_ENDED:
            data_0215fe8c->linkState = 0;
            break;
        case LINK_PACKET_ACTION + 0:
        case LINK_PACKET_ACTION + 1:
        case LINK_PACKET_ACTION + 2:
        case LINK_PACKET_ACTION + 3:
            /* skip the echo of this player's own action */
            if (packet.type - LINK_PACKET_ACTION != func_0207a260()) {
                LinkActionQueue_Push(packet.payload);
            }
            break;
        case LINK_PACKET_REACTION:
            LinkReactionQueue_Push(packet.payload);
            break;
        }
    } while (Link_PopFlaggedPacket(0, &packet) == 1);
}

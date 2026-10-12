#include "types.h"
#include "sdk/wl.h"

u8 WL_ReadByte(const void *addr);
BOOL WifiScan_SsidPassesFilter(u32 length, const u8 *ssid);
void WifiRates_DecodeElement(const u8 *element, WlRateSet *rates);

/*
 * Walks the information elements of a received beacon, probe or association
 * frame. Records where the SSID, DS parameter set, rates, TIM, CF parameter
 * set and Nintendo vendor element are, and sets a matched bit for each value
 * that agrees with our own settings (the channel with the join request's).
 */
void WifiRx_ScanElements(WlRxElements *info)
{
    WlLink *link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);
    const u8 *p = info->elements;
    int remaining;
    u32 id;
    u32 length;

    info->channel = link->channel;
    if (info->found & WIFI_IE_BLANK_SSID_PASSES) {
        info->matched |= WIFI_IE_AGREES_SSID;
    }
    for (remaining = info->length; remaining > 0; remaining -= length + 2) {
        id = WL_ReadByte(p++);
        length = WL_ReadByte(p++);
        switch (id) {
        case 0: /* SSID */
            if (length <= WIFI_SSID_LONGEST) {
                info->found |= WIFI_IE_SPOTTED_NETWORK_NAME;
                info->ssid = p - 2;
                if (length == 0 && (info->found & WIFI_IE_BLANK_SSID_PASSES)) {
                    info->matched |= WIFI_IE_AGREES_SSID;
                    break;
                }
                info->matched &= ~WIFI_IE_AGREES_SSID;
                /* needed to match: WifiScan_SsidPassesFilter probably takes a u16 length */
                if (WifiScan_SsidPassesFilter((u16)length, p)) {
                    info->matched |= WIFI_IE_AGREES_SSID;
                }
            } else {
                info->found &= ~WIFI_IE_SPOTTED_NETWORK_NAME;
                info->ssid = NULL;
            }
            break;
        case 3: /* DS parameter set */
            if (length >= 1) {
                info->found |= WIFI_IE_SPOTTED_DS_SET;
                info->channel = WL_ReadByte(p);
                if (info->channel == ((WlJoinRequest *)WIFI_ACTIVE_JOB_OF(WIFI_DRIVER_STATE)->any.request)->channel) {
                    info->matched |= WIFI_IE_AGREES_CHANNEL;
                } else {
                    info->matched &= ~WIFI_IE_AGREES_CHANNEL;
                }
            }
            break;
        case 1: /* supported rates */
            if (length >= 1) {
                info->found |= WIFI_IE_SPOTTED_RATE_LIST;
                WifiRates_DecodeElement(p - 2, &info->rates);
                if ((info->rates.basic & ~(link->basicRates | link->supportedRates)) == 0
                    && (link->basicRates & (info->rates.basic | info->rates.supported)) == link->basicRates) {
                    info->matched |= WIFI_IE_AGREES_RATES;
                } else {
                    info->matched &= ~WIFI_IE_AGREES_RATES;
                }
            }
            break;
        case 5: /* TIM */
            if (length >= 3) {
                info->found |= WIFI_IE_SPOTTED_TRAFFIC_MAP;
                info->tim = p - 2;
            }
            break;
        case 4: /* CF parameter set */
            if (length >= 6) {
                info->found |= WIFI_IE_SPOTTED_CF_SET;
                info->cfParams = p - 2;
            }
            break;
        case 221: /* vendor specific */
            if (length >= 8 && WL_ReadByte(p) == 0x00 && WL_ReadByte(p + 1) == 0x09
                && WL_ReadByte(p + 2) == 0xbf && WL_ReadByte(p + 3) == 0) {
                info->found |= WIFI_IE_SPOTTED_VENDOR;
                info->nintendo = p - 2;
            }
            break;
        }
        p += length;
    }
    if (info->found & WIFI_IE_ASK_BSSID_TEST) {
        if ((link->bssid[0] & 1) || (info->unk_04 & 0x8000)) {
            info->matched |= WIFI_IE_AGREES_BSSID;
        }
    }
    if (info->found & WIFI_IE_ASK_CAPS_TEST) {
        if ((info->capability & 3) == (link->flags_7c & 3)) {
            info->matched |= WIFI_IE_AGREES_NET_KIND;
        }
        if ((info->capability & 0x10) == (link->flags_7c & 0x10)) {
            info->matched |= WIFI_IE_AGREES_PRIVACY;
        }
    }
}

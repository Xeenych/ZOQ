#pragma once
#include "debug_print.hpp"

namespace w5500 {

template <class driver_t>
class w5500_itf {
  public:
    struct settings_t {
        uint8_t shar[6];  // mac
        uint8_t sipr[4];  // ip
        uint8_t subr[4];  // subnet
        uint8_t gar[4];   // gateway
    };
    enum class state_t : uint8_t { W5500_NoChip, W5500_NoCable, W5500_Configured, W5500_Init };

    struct status_t {
        uint8_t HasChip : 1;
        uint8_t HasPhyLink : 1;
        uint8_t Is100Mbps : 1;
        uint8_t IsFullDuplex : 1;
    };

    inline w5500_itf(driver_t& d, const settings_t& set);
    void SetSettings(const settings_t& settings);
    void WriteSettings();
    settings_t ReadSettings();
    void run();

    uint16_t socket_send(uint8_t sock, const uint8_t* buf, uint16_t len);
    uint16_t socket_recv(uint8_t sock, uint8_t* buf, uint16_t buflen);
    void socket_open_tcp(uint8_t sockn, uint16_t port);
    void socket_listen(uint8_t sockn);
    void socket_discon(uint8_t sockn);
    uint16_t socket_rxlen(uint8_t sockn);

  private:
    driver_t& drv;
    settings_t settings;
};

template <class driver_t>
w5500_itf<driver_t>::w5500_itf(driver_t& d, const settings_t& set) : drv(d), settings(set)
{
}

template <class driver_t>
w5500_itf<driver_t>::settings_t w5500_itf<driver_t>::ReadSettings()
{
    settings_t ret;
    drv.ReadBuf(W5500_ADR_SHAR, W5500_BSB_COMMON, ret.shar, 6);
    drv.ReadBuf(W5500_ADR_SUBR, W5500_BSB_COMMON, ret.subr, 4);
    drv.ReadBuf(W5500_ADR_GAR, W5500_BSB_COMMON, ret.gar, 4);
    drv.ReadBuf(W5500_ADR_SIPR, W5500_BSB_COMMON, ret.sipr, 4);
    return ret;
}

template <class driver_t>
void w5500_itf<driver_t>::SetSettings(const w5500_itf<driver_t>::settings_t& set)
{
    settings = set;
}

template <class driver_t>
void w5500_itf<driver_t>::WriteSettings()
{
    drv.WriteBuf(W5500_ADR_SHAR, W5500_BSB_COMMON, settings.shar, 6);
    drv.WriteBuf(W5500_ADR_SUBR, W5500_BSB_COMMON, settings.subr, 4);
    drv.WriteBuf(W5500_ADR_GAR, W5500_BSB_COMMON, settings.gar, 4);
    drv.WriteBuf(W5500_ADR_SIPR, W5500_BSB_COMMON, settings.sipr, 4);
}

template <class driver_t>
uint16_t w5500_itf<driver_t>::socket_send(uint8_t sock, const uint8_t* buf, uint16_t len)
{
    if (!len)
        return 0;

    uint16_t txfree = drv.ReadWord(W5500_ADR_Sn_TX_FSR, W5500_BSB_SOCKET(sock));
    if (!txfree)
        return 0;

    uint16_t txaddr = drv.ReadWord(W5500_ADR_Sn_TX_WR, W5500_BSB_SOCKET(sock));
    uint16_t tosend = (txfree > len) ? len : txfree;

    drv.WriteBuf(txaddr, W5500_BSB_SOCKET_TX(sock), buf, tosend);
    drv.WriteWord(W5500_ADR_Sn_TX_WR, W5500_BSB_SOCKET(sock), tosend + txaddr);
    drv.WriteByte(W5500_ADR_Sn_CR, W5500_BSB_SOCKET(sock), W5500_Sn_CR_SEND);
    return tosend;
}

template <class driver_t>
uint16_t w5500_itf<driver_t>::socket_recv(uint8_t sock, uint8_t* buf, uint16_t buflen)
{
    uint16_t rxsize = drv.ReadWord(W5500_ADR_Sn_RX_RSR, W5500_BSB_SOCKET(sock));
    uint16_t torecv = (rxsize > buflen) ? buflen : rxsize;
    if (!torecv)
        return 0;

    uint16_t rxaddr = drv.ReadWord(W5500_ADR_Sn_RX_RD, W5500_BSB_SOCKET(sock));
    drv.ReadBuf(rxaddr, W5500_BSB_SOCKET_RX(sock), buf, torecv);
    drv.WriteWord(W5500_ADR_Sn_RX_RD, W5500_BSB_SOCKET(sock), torecv + rxaddr);
    drv.WriteByte(W5500_ADR_Sn_CR, W5500_BSB_SOCKET(sock), W5500_Sn_CR_RECV);

    return torecv;
}

template <class driver_t>
void w5500_itf<driver_t>::socket_open_tcp(uint8_t sockn, uint16_t port)
{
    drv.WriteWord(W5500_ADR_Sn_PORT, W5500_BSB_SOCKET(sockn), port);
    drv.WriteByte(W5500_ADR_Sn_RXBUF_SIZE, W5500_BSB_SOCKET(sockn), W5500_SOCK_SIZE_2K);
    drv.WriteByte(W5500_ADR_Sn_TXBUF_SIZE, W5500_BSB_SOCKET(sockn), W5500_SOCK_SIZE_2K);
    drv.WriteByte(W5500_ADR_Sn_MR, W5500_BSB_SOCKET(sockn), W5500_PROTO_TCP);
    drv.WriteByte(W5500_ADR_Sn_CR, W5500_BSB_SOCKET(sockn), W5500_Sn_CR_OPEN);
}

template <class driver_t>
void w5500_itf<driver_t>::socket_listen(uint8_t sockn)
{
    drv.WriteByte(W5500_ADR_Sn_CR, W5500_BSB_SOCKET(sockn), W5500_Sn_CR_LISTEN);
}

template <class driver_t>
void w5500_itf<driver_t>::socket_discon(uint8_t sockn)
{
    drv.WriteByte(W5500_ADR_Sn_CR, W5500_BSB_SOCKET(sockn), W5500_Sn_CR_DISCON);
}

template <class driver_t>
uint16_t w5500_itf<driver_t>::socket_rxlen(uint8_t sockn)
{
    return drv.ReadWord(W5500_ADR_Sn_RX_RSR, W5500_BSB_SOCKET(sockn));
}

template <class driver_t>
void w5500_itf<driver_t>::run()
{
    for (int sockn = 0; sockn < 8; sockn++) {
        auto snsr = drv.ReadByte(W5500_ADR_Sn_SR, W5500_BSB_SOCKET(sockn));
        switch (snsr) {
            case W5500_Sn_SR_SOCK_CLOSED:
                handler.on_closed(*this);
                debug_printf("sock %d closed\n", sockn);
                break;
            case W5500_Sn_SR_SOCK_INIT:
                handler.on_init(*this);
                debug_printf("sock %d init\n", sockn);
                break;
            case W5500_Sn_SR_SOCK_LISTEN:
                handler.on_listen(*this);
                //			debug_printf("sock %d listen\n", sockn);
                break;
            case W5500_Sn_SR_SOCK_ESTABLISHED:
                handler.on_established(*this);
                // debug_printf("sock %d established\n", sockn);
                break;
            case W5500_Sn_SR_SOCK_CLOSE_WAIT:
                handler.on_close_wait(*this);
                debug_printf("sock %d close_wait\n", sockn);
                break;
            default:
                debug_printf("sock %d close_other\n", sockn);
                //			assert(0);
                break;
        }
    }
}

/*
template <class driver_t>
void w5500_itf<driver_t>::Run()
{
    uint8_t version;
    // проверка наличия чипа
    version = drv.ReadByte(W5500_ADR_VERSIONR, W5500_BSB_COMMON);

    status_t status;
    status.HasChip = (version == 0x04) ? 1 : 0;

    WriteSettings();
    auto set = ReadSettings();

    if (set.sipr[0] != settings.sipr[0])
        debug_printf("bad");

    // проверка наличия провода
    uint8_t phycfgr = drv.ReadByte(W5500_ADR_PHYCFGR, W5500_BSB_COMMON);
    status.HasPhyLink = ((phycfgr & 0x01) > 0) ? 1 : 0;
    status.Is100Mbps = ((phycfgr & 0x02) > 0) ? 1 : 0;
    status.IsFullDuplex = ((phycfgr & 0x04) > 0) ? 1 : 0;
    // return state_t::W5500_Configured;
}
*/

}  // namespace w5500
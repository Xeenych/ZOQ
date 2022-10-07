#pragma once

#include <cstdint>
#include <algorithm>

#include "w5500/w5500_defs.h"

namespace w5500 {

template <typename spi_T>
class w5500driver_T {
  public:
	inline constexpr w5500driver_T(const spi_T& s):spi(s){};
    uint8_t ReadByte(uint16_t addr, uint8_t block);
    uint16_t ReadWord(uint16_t addr, uint8_t block);
    uint32_t ReadDWord(uint16_t addr, uint8_t block);
    void WriteByte(uint16_t addr, uint8_t block, uint8_t b);
    void WriteWord(uint16_t addr, uint8_t block, uint16_t b);
    void WriteDWord(uint16_t addr, uint8_t block, uint32_t b);
    uint16_t SendBuf(uint8_t sock, const uint8_t* buf, uint16_t len);
    uint16_t RecvBuf(uint8_t sock, uint8_t* buf, uint16_t buflen);
    void ReadBuf(uint16_t addr, uint8_t block, uint8_t* buf, uint16_t toread);
    void WriteBuf(uint16_t addr, uint8_t block, const uint8_t* buf, uint16_t len);
    void set_nss();
    void clr_nss();
    inline void run() {}; 

  private:
    const spi_T& spi;
};

template <typename spi_T>
uint8_t w5500driver_T<spi_T>::ReadByte(uint16_t addr, uint8_t block)
{
    uint8_t txd[4], rxd[4];
    txd[0] = addr >> 8;
    txd[1] = addr;
    txd[2] = ((block & 0x1f) << 3) | OPMODE_R | OPMODE_FIX8;

	spi.set_nss();
    spi.TransmitReceive(txd, rxd, 4);
	spi.clr_nss();
    uint8_t ret = rxd[3];
    return ret;
}

template <typename spi_T>
uint16_t w5500driver_T<spi_T>::ReadWord(uint16_t addr, uint8_t block)
{
    uint8_t txd[5], rxd[5];
    txd[0] = addr >> 8;
    txd[1] = addr;
    txd[2] = ((block & 0x1f) << 3) | OPMODE_R | OPMODE_FIX16;

    spi.set_nss();
    spi.TransmitReceive(txd, rxd, sizeof(txd));
    spi.clr_nss();

    uint16_t ret = (rxd[3] << 8) | (rxd[4]);
    return ret;
}

template <typename spi_T>
uint32_t w5500driver_T<spi_T>::ReadDWord(uint16_t addr, uint8_t block)
{
    uint8_t txd[7], rxd[7];
    txd[0] = addr >> 8;
    txd[1] = addr;
    txd[2] = ((block & 0x1f) << 3) | OPMODE_R | OPMODE_FIX32;

    spi.set_nss();
    spi.TransmitReceive(txd, rxd, sizeof(txd));
    spi.clr_nss();

    uint32_t ret = (rxd[3] << 24) | (rxd[4] << 16) | (rxd[5] << 8) | rxd[6];
    return ret;
}

template <typename spi_T>
void w5500driver_T<spi_T>::WriteByte(uint16_t addr, uint8_t block, uint8_t b)
{
    uint8_t txd[4], rxd[4];
    txd[0] = addr >> 8;
    txd[1] = addr;
    txd[2] = ((block & 0x1f) << 3) | OPMODE_W | OPMODE_FIX8;
    txd[3] = b;

    spi.set_nss();
    spi.Transmit(txd, sizeof(txd));
    spi.clr_nss();
}

template <typename spi_T>
void w5500driver_T<spi_T>::WriteWord(uint16_t addr, uint8_t block, uint16_t b)
{
    uint8_t txd[5], rxd[5];
    txd[0] = addr >> 8;
    txd[1] = addr;
    txd[2] = ((block & 0x1f) << 3) | OPMODE_W | OPMODE_FIX16;
    txd[3] = b >> 8;
    txd[4] = b;

    spi.set_nss();
    spi.Transmit(txd, sizeof(txd));
    spi.clr_nss();
}

template <typename spi_T>
void w5500driver_T<spi_T>::WriteDWord(uint16_t addr, uint8_t block, uint32_t b)
{
    uint8_t txd[7], rxd[7];
    txd[0] = addr >> 8;
    txd[1] = addr;
    txd[2] = ((block & 0x1f) << 3) | OPMODE_W | OPMODE_FIX32;
    txd[3] = b >> 24;
    txd[4] = b >> 16;
    txd[5] = b >> 8;
    txd[6] = b;

    spi.set_nss();
    spi.Transmit(txd, rxd, sizeof(txd));
    spi.clr_nss();
}

template <typename spi_T>
void w5500driver_T<spi_T>::ReadBuf(uint16_t addr, uint8_t block, uint8_t* buf, uint16_t toread)
{
    if (toread == 0)
        return;
    uint8_t txd[3];
    txd[0] = addr >> 8;
    txd[1] = addr;
    txd[2] = ((block & 0x1f) << 3) | OPMODE_R | OPMODE_VDM;

    spi.set_nss();
    spi.Transmit(txd, sizeof(txd));
    spi.Receive(buf, toread);
    spi.clr_nss();
}

template <typename spi_T>
void w5500driver_T<spi_T>::WriteBuf(uint16_t addr, uint8_t block, const uint8_t* buf, uint16_t len)
{
    if (len == 0)
        return;
    uint8_t txd[3];
    txd[0] = addr >> 8;
    txd[1] = addr;
    txd[2] = ((block & 0x1f) << 3) | OPMODE_W | OPMODE_VDM;

    spi.set_nss();
    spi.Transmit(txd, sizeof(txd));
    spi.Transmit(buf, len);
    spi.clr_nss();
}

template <typename spi_T>
uint16_t w5500driver_T<spi_T>::SendBuf(uint8_t sock, const uint8_t* buf, uint16_t len)
{
    if (!len)
        return 0;

    uint16_t txfree = ReadWord(W5500_ADR_Sn_TX_FSR, W5500_BSB_SOCKET(sock));
    if (!txfree)
        return 0;

    uint16_t txaddr = ReadWord(W5500_ADR_Sn_TX_WR, W5500_BSB_SOCKET(sock));
    uint16_t tosend = (txfree > len) ? len : txfree;

    WriteBuf(txaddr, W5500_BSB_SOCKET_TX(sock), buf, tosend);
    WriteWord(W5500_ADR_Sn_TX_WR, W5500_BSB_SOCKET(sock), tosend + txaddr);
    WriteByte(W5500_ADR_Sn_CR, W5500_BSB_SOCKET(sock), W5500_Sn_CR_SEND);
    return tosend;
}

template <typename spi_T>
uint16_t w5500driver_T<spi_T>::RecvBuf(uint8_t sock, uint8_t* buf, uint16_t buflen)
{
    uint16_t rxsize = ReadWord(W5500_ADR_Sn_RX_RSR, W5500_BSB_SOCKET(sock));
    uint16_t torecv = (rxsize > buflen) ? buflen : rxsize;
    if (!torecv)
        return 0;
    
    uint16_t rxaddr = ReadWord(W5500_ADR_Sn_RX_RD, W5500_BSB_SOCKET(sock));
    ReadBuf(rxaddr, W5500_BSB_SOCKET_RX(sock), buf, torecv);
    WriteWord(W5500_ADR_Sn_RX_RD, W5500_BSB_SOCKET(sock), torecv + rxaddr);
    WriteByte(W5500_ADR_Sn_CR, W5500_BSB_SOCKET(sock), W5500_Sn_CR_RECV);

    return torecv;
}

}
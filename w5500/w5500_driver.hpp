#pragma once

#include <cstdint>
#include <algorithm>

#include "w5500/w5500_defs.h"

namespace w5500 {

template <typename spi_T>
class w5500driver_T {
  public:
    inline constexpr w5500driver_T(const spi_T& s) : spi(s){};
    uint8_t ReadByte(uint16_t addr, uint8_t block) const;
    uint16_t ReadWord(uint16_t addr, uint8_t block) const;
    uint32_t ReadDWord(uint16_t addr, uint8_t block) const;
    void WriteByte(uint16_t addr, uint8_t block, uint8_t b) const;
    void WriteWord(uint16_t addr, uint8_t block, uint16_t b) const;
    void WriteDWord(uint16_t addr, uint8_t block, uint32_t b) const;
    void ReadBuf(uint16_t addr, uint8_t block, uint8_t* buf, uint16_t toread) const;
    void WriteBuf(uint16_t addr, uint8_t block, const uint8_t* buf, uint16_t len) const;
    void set_nss() const;
    void clr_nss() const;
    inline void run() const {};

  private:
    const spi_T& spi;
};

template <typename spi_T>
uint8_t w5500driver_T<spi_T>::ReadByte(uint16_t addr, uint8_t block) const
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
uint16_t w5500driver_T<spi_T>::ReadWord(uint16_t addr, uint8_t block) const
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
uint32_t w5500driver_T<spi_T>::ReadDWord(uint16_t addr, uint8_t block) const
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
void w5500driver_T<spi_T>::WriteByte(uint16_t addr, uint8_t block, uint8_t b) const
{
    uint8_t txd[4], rxd[4];
    txd[0] = addr >> 8;
    txd[1] = addr;
    txd[2] = ((block & 0x1f) << 3) | OPMODE_W | OPMODE_FIX8;
    txd[3] = b;

    spi.set_nss();
    spi.TransmitReceive(txd, rxd, sizeof(txd));
    spi.clr_nss();
}

template <typename spi_T>
void w5500driver_T<spi_T>::WriteWord(uint16_t addr, uint8_t block, uint16_t b) const
{
    uint8_t txd[5], rxd[5];
    txd[0] = addr >> 8;
    txd[1] = addr;
    txd[2] = ((block & 0x1f) << 3) | OPMODE_W | OPMODE_FIX16;
    txd[3] = b >> 8;
    txd[4] = b;

    spi.set_nss();
    spi.TransmitReceive(txd, rxd, sizeof(txd));
    spi.clr_nss();
}

template <typename spi_T>
void w5500driver_T<spi_T>::WriteDWord(uint16_t addr, uint8_t block, uint32_t b) const
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
    spi.TransmitReceive(txd, rxd, sizeof(txd));
    spi.clr_nss();
}

template <typename spi_T>
void w5500driver_T<spi_T>::ReadBuf(uint16_t addr, uint8_t block, uint8_t* buf, uint16_t toread) const
{
    if (toread == 0)
        return;
    uint8_t txd[3], rxd[3];
    txd[0] = addr >> 8;
    txd[1] = addr;
    txd[2] = ((block & 0x1f) << 3) | OPMODE_R | OPMODE_VDM;

    spi.set_nss();
    spi.TransmitReceive(txd, rxd, sizeof(txd));
    spi.Receive(buf, toread);
    spi.clr_nss();
}

template <typename spi_T>
void w5500driver_T<spi_T>::WriteBuf(uint16_t addr, uint8_t block, const uint8_t* buf, uint16_t len) const
{
    if (len == 0)
        return;
    uint8_t txd[3], rxd[3];
    txd[0] = addr >> 8;
    txd[1] = addr;
    txd[2] = ((block & 0x1f) << 3) | OPMODE_W | OPMODE_VDM;

    spi.set_nss();
    spi.TransmitReceive(txd, rxd, sizeof(txd));
    spi.Transmit(buf, len);
    spi.clr_nss();
}

}  // namespace w5500
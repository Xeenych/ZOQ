#pragma once
#include "../bitband.h"
#include "stm32f4xx_hal.h"

namespace ZOQ::Stm32_HAL {

class spi_t {
  public:
    inline constexpr spi_t(SPI_TypeDef* s) : spi(s){};
    inline void set_nss() const;
    inline void clr_nss() const;
    void TransmitReceive(const uint8_t* pTxData, uint8_t* pRxData, uint16_t Size) const;
    void Transmit(const uint8_t* pTxData, uint16_t Size) const;
    void Receive( uint8_t* pRxData, uint16_t Size) const;

  private:
    SPI_TypeDef* const spi;
};

void spi_t::set_nss() const {
	// SPI enable  
  	BIT_BAND_PER(spi->CR1, SPI_CR1_SPE) = 1;
}

void spi_t::clr_nss() const {
    // SPI disable
    BIT_BAND_PER(spi->CR1, SPI_CR1_SPE) = 0;
}

void spi_t::Transmit(const uint8_t* pTxData, uint16_t Size) const
{
	// clear DR
	//spi->DR;
    const auto end = pTxData + Size;
    while (pTxData != end) {
        while (!BIT_BAND_PER(spi->SR, SPI_FLAG_TXE));
        spi->DR = *pTxData++;
		
		while (!BIT_BAND_PER(spi->SR, SPI_FLAG_RXNE));
		spi->DR;
    }
}

void spi_t::TransmitReceive(const uint8_t* pTxData, uint8_t* pRxData, uint16_t Size) const
{
	// clear DR
	//spi->DR;
	const auto txend = pTxData + Size;
	while (pTxData != txend) {
		while (!BIT_BAND_PER(spi->SR, SPI_FLAG_TXE));	
		spi->DR = *pTxData++;
		
		while (!BIT_BAND_PER(spi->SR, SPI_FLAG_RXNE));
		*pRxData++ = spi->DR;
	}
}

void spi_t::Receive( uint8_t* pRxData, uint16_t Size) const
{
	// clear DR
	//spi->DR;
	const auto rxend = pRxData + Size;
	while (pRxData != rxend) {
		while (!BIT_BAND_PER(spi->SR, SPI_FLAG_TXE));	
		spi->DR = 0;
		
		while (!BIT_BAND_PER(spi->SR, SPI_FLAG_RXNE));
		*pRxData++ = spi->DR;
	}
}


}
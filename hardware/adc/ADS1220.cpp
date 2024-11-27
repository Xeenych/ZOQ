#include "ADS1220.hpp"

#include "stm32f0xx.h"
#include "sys/dassert.h"

// ADS1120 SPI commands
static constexpr uint8_t ADS1220_RESET = 0x06;
// Send the START/SYNC command (08h) to start converting in continuous conversion mode
static constexpr uint8_t ADS1220_START = 0x08;
static constexpr uint8_t ADS1220_PWRDOWN = 0x02;
static constexpr uint8_t ADS1220_RDATA = 0x10;
static constexpr uint8_t ADS1220_WREG = 0x40;  // write register
static constexpr uint8_t ADS1220_RREG = 0x20;  // read register

/* registers */
static constexpr uint8_t ADS1220_CONF_REG_0 = 0x00;
static constexpr uint8_t ADS1220_CONF_REG_1 = 0x01 << 2;
static constexpr uint8_t ADS1220_CONF_REG_2 = 0x02 << 2;
static constexpr uint8_t ADS1220_CONF_REG_3 = 0x03 << 2;

namespace common {

void ads1220_t::set_config_0(Mux mux, Gain gain, PgaBypass pga_bypass)
{
    uint8_t value = (uint8_t)mux | (uint8_t)gain | (uint8_t)pga_bypass;
    writeRegister(ADS1220_CONF_REG_0, value);
}

void ads1220_t::set_config_1(DataRate dr, OpMode opmode, ConvMode cm, TsMode ts, Burnout burnout)
{
    uint8_t value = (uint8_t)dr | (uint8_t)opmode | (uint8_t)cm | (uint8_t)ts | (uint8_t)burnout;
    writeRegister(ADS1220_CONF_REG_1, value);
}

void ads1220_t::set_config_2(VRef vref, FIR fir, PSW psw, IdacCurrent idac)
{
    uint8_t value = (uint8_t)vref | (uint8_t)fir | (uint8_t)psw | (uint8_t)idac;
    writeRegister(ADS1220_CONF_REG_2, value);
}

void ads1220_t::set_config_3(IdacRouting idac1, IdacRouting idac2, DrdyMode mode)
{
    uint8_t value = (uint8_t)idac1 << 5 | (uint8_t)idac2 << 2 | (uint8_t)mode;
    writeRegister(ADS1220_CONF_REG_3, value);
}

// Every write access to any configuration register also starts a conversion!
void ads1220_t::setup(const config_t& c)
{
    set_config_0(c.mux, c.gain, c.pga_bypass);
    set_config_1(DataRate::DR_LVL_0, OpMode::NORMAL_MODE, ConvMode::SINGLE_SHOT, c.ts, Burnout::BCS_DISABLED);
    set_config_2(c.ref, FIR::HZ50, PSW::ALWAYS_OPEN, c.current);
    set_config_3(c.routing, IdacRouting::IDAC_NONE, DrdyMode::DRDY);
}

ads1220_t::ads1220_t(spi_itf& spi, o_pin_itf& cs, i_pin_it_itf& rdy) : spi_{spi}, cs_{cs}, drdy_{rdy}
{
    NVIC_DisableIRQ(EXTI0_1_IRQn);
    drdy_.set_callback(+[](void* c) { static_cast<ads1220_t*>(c)->on_ready_irq(); }, this);
}

uint8_t ads1220_t::init()
{
    cs_.set();
    reset();
    // start();
    uint8_t ctrlVal = readRegister(ADS1220_CONF_REG_0);
    // bypassPGA(false);
    return ctrlVal & 0x01;
}

// Single conversion takes ~50ms
void ads1220_t::start()
{
    // HAL_GPIO_WritePin(LED_S_GPIO_Port, LED_S_Pin, GPIO_PIN_RESET);
    NVIC_ClearPendingIRQ(EXTI0_1_IRQn);
    NVIC_EnableIRQ(EXTI0_1_IRQn);
    command(ADS1220_START);
}

void ads1220_t::reset()
{
    command(ADS1220_RESET);
}

void ads1220_t::powerDown()
{
    command(ADS1220_PWRDOWN);
}

void ads1220_t::on_ready_irq()
{
    NVIC_DisableIRQ(EXTI0_1_IRQn);

    // В отличие от ADS1120 тут три байта результата
    uint8_t buf[3];
    cs_.reset();
    spi_.read({buf, 3});
    cs_.set();

    int32_t r = ((int8_t)buf[0] << 16) | buf[1] << 8 | buf[2];
    last_result = r;
    if (cb_)
        cb_(ctx_, last_result);
}

uint16_t ads1220_t::getLastResult()
{
    return last_result;
}

uint8_t ads1220_t::readRegister(uint8_t reg)
{
    uint8_t regValue = 0;
    uint8_t buf = (uint8_t)(ADS1220_RREG | reg);

    cs_.reset();
    spi_.write({&buf, 1});
    spi_.read({&regValue, 1});
    cs_.set();
    return regValue;
}

void ads1220_t::writeRegister(uint8_t reg, uint8_t val)
{
    uint8_t buf = (uint8_t)(ADS1220_WREG | reg);
    cs_.reset();
    spi_.write({&buf, 1});
    spi_.read({&val, 1});
    cs_.set();
}

void ads1220_t::command(uint8_t cmd)
{
    cs_.reset();
    spi_.write({&cmd, 1});
    cs_.set();
}

}  // namespace common

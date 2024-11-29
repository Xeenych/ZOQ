#include "ADS1120.hpp"

//#include "stm32f0xx.h"
#include "ZOQ/sys/dassert.h"

namespace common {

// ADS1120 SPI commands
static const uint8_t RESET = 0x06;
static const uint8_t START =
    0x08;  // Send the START/SYNC command (08h) to start converting in continuous conversion mode
static const uint8_t PWRDOWN = 0x02;
static const uint8_t RDATA = 0x10;
static const uint8_t WREG = 0x40;  // write register
static const uint8_t RREG = 0x20;  // read register

/* registers */
static const uint8_t CONF_REG_0 = 0x00;
static const uint8_t CONF_REG_1 = 0x01 << 2;
static const uint8_t CONF_REG_2 = 0x02 << 2;
static const uint8_t CONF_REG_3 = 0x03 << 2;

void ads1120_t::set_config_0(Mux mux, Gain gain, PgaBypass pga_bypass)
{
    uint8_t value = (uint8_t)mux | (uint8_t)gain | (uint8_t)pga_bypass;
    writeRegister(CONF_REG_0, value);
}

void ads1120_t::set_config_1(DataRate dr, OpMode opmode, ConvMode cm, TsMode ts, Burnout burnout)
{
    uint8_t value = (uint8_t)dr | (uint8_t)opmode | (uint8_t)cm | (uint8_t)ts | (uint8_t)burnout;
    writeRegister(CONF_REG_1, value);
}

void ads1120_t::set_config_2(VRef vref, FIR fir, PSW psw, IdacCurrent idac)
{
    uint8_t value = (uint8_t)vref | (uint8_t)fir | (uint8_t)psw | (uint8_t)idac;
    writeRegister(CONF_REG_2, value);
}

void ads1120_t::set_config_3(IdacRouting idac1, IdacRouting idac2, DrdyMode mode)
{
    uint8_t value = (uint8_t)idac1 << 5 | (uint8_t)idac2 << 2 | (uint8_t)mode;
    writeRegister(CONF_REG_3, value);
}

// Every write access to any configuration register also starts a conversion!
void ads1120_t::setup(const config_t& c)
{
    set_config_0(c.mux, c.gain, c.pga_bypass);
    set_config_1(DataRate::DR_LVL_0, OpMode::NORMAL_MODE, ConvMode::SINGLE_SHOT, c.ts, Burnout::BCS_DISABLED);
    set_config_2(c.ref, FIR::HZ50, PSW::ALWAYS_OPEN, c.current);
    set_config_3(c.routing, IdacRouting::IDAC_NONE, DrdyMode::DRDY);
}

ads1120_t::ads1120_t(spi_itf& spi, o_pin_itf& cs, i_pin_it_itf& rdy) : spi_{spi}, cs_{cs}, drdy_{rdy}
{
    //NVIC_DisableIRQ(EXTI0_1_IRQn);
    drdy_.set_callback(+[](void* c) { static_cast<ads1120_t*>(c)->on_ready_irq(); }, this);
}

uint8_t ads1120_t::init()
{
    cs_.set();
    reset();
    // start();
    uint8_t ctrlVal = readRegister(CONF_REG_0);
    // bypassPGA(false);
    return ctrlVal & 0x01;
}

// Single conversion takes ~50ms
void ads1120_t::start()
{
    // HAL_GPIO_WritePin(LED_S_GPIO_Port, LED_S_Pin, GPIO_PIN_RESET);
    //NVIC_ClearPendingIRQ(EXTI0_1_IRQn);
    //NVIC_EnableIRQ(EXTI0_1_IRQn);
    command(START);
}

void ads1120_t::reset()
{
    command(RESET);
}

void ads1120_t::powerDown()
{
    command(PWRDOWN);
}

/* Configuration Register 0 settings */
void ads1120_t::setCompareChannels(Mux mux)
{
    if ((mux == Mux::MUX_REFPX_REFNX_4) || (mux == Mux::MUX_AVDD_M_AVSS_4)) {
        params.gain = 1;  // under these conditions gain is one by definition
        params.refMeasurement = true;
    } else {  // otherwise read gain from register
        uint8_t regValue = readRegister(CONF_REG_0);
        regValue = regValue & 0x0E;
        regValue = regValue >> 1;
        params.gain = 1 << regValue;
        params.refMeasurement = false;
    }
    uint8_t regValue = readRegister(CONF_REG_0);
    regValue &= ~0xF1;
    regValue |= static_cast<uint8_t>(mux);
    regValue |= !(params.doNotBypassPgaIfPossible & 0x01);
    writeRegister(CONF_REG_0, regValue);
    if ((mux >= Mux::MUX_0_AVSS) && (mux <= Mux::MUX_AVDD_M_AVSS_4)) {
        if (params.gain > 4) {
            params.gain = 4;  // max gain is 4 if single-ended input is chosen or PGA is bypassed
        }
        forcedBypassPGA();
    }
}

void ads1120_t::setGain(Gain enumGain)
{
    auto regValue = readRegister(CONF_REG_0);
    Mux mux = (Mux)(regValue & 0xF0);
    regValue &= ~0x0E;
    regValue |= static_cast<uint8_t>(enumGain);
    writeRegister(CONF_REG_0, regValue);

    params.gain = 1 << (static_cast<uint8_t>(enumGain) >> 1);
    if ((mux >= Mux::MUX_0_AVSS) && (mux <= Mux::MUX_AVDD_M_AVSS_4)) {
        if (params.gain > 4) {
            params.gain = 4;  // max gain is 4 if single-ended input is chosen or PGA is bypassed
        }
        forcedBypassPGA();
    }
}

void ads1120_t::bypassPGA(bool bypass)
{
    uint8_t regValue = readRegister(CONF_REG_0);
    regValue &= ~0x01;
    regValue |= bypass;
    params.doNotBypassPgaIfPossible = !(bypass & 0x01);
    writeRegister(CONF_REG_0, regValue);
}

bool ads1120_t::isPGABypassed()
{
    uint8_t regValue = readRegister(CONF_REG_0);
    return regValue & 0x01;
}

/* Configuration Register 1 settings */
void ads1120_t::setDataRate(DataRate rate)
{
    uint8_t regValue = readRegister(CONF_REG_1);
    regValue &= ~0xE0;
    regValue |= static_cast<uint8_t>(rate);
    writeRegister(CONF_REG_1, regValue);
}

void ads1120_t::setOperatingMode(OpMode mode)
{
    uint8_t regValue = readRegister(CONF_REG_1);
    regValue &= ~0x18;
    regValue |= static_cast<uint8_t>(mode);
    writeRegister(CONF_REG_1, regValue);
}

void ads1120_t::setConversionMode(ConvMode mode)
{
    params.convMode = mode;
    uint8_t regValue = readRegister(CONF_REG_1);
    regValue &= ~0x04;
    regValue |= static_cast<uint8_t>(mode);
    writeRegister(CONF_REG_1, regValue);
}

void ads1120_t::enableTemperatureSensor(bool enable)
{
    uint8_t regValue = readRegister(CONF_REG_1);
    if (enable) {
        regValue |= 0x02;
    } else {
        regValue &= ~0x02;
    }
    writeRegister(CONF_REG_1, regValue);
}

void ads1120_t::enableBurnOutCurrentSources(bool enable)
{
    uint8_t regValue = readRegister(CONF_REG_1);
    if (enable) {
        regValue |= 0x01;
    } else {
        regValue &= ~0x01;
    }
    writeRegister(CONF_REG_1, regValue);
}

/* Configuration Register 2 settings */

void ads1120_t::setVRefSource(VRef vRefSource)
{
    uint8_t regValue = readRegister(CONF_REG_2);
    regValue &= ~0xC0;
    regValue |= static_cast<uint8_t>(vRefSource);
    writeRegister(CONF_REG_2, regValue);
}

void ads1120_t::setFIRFilter(FIR fir)
{
    auto regValue = readRegister(CONF_REG_2);
    regValue &= ~0x30;
    regValue |= static_cast<uint8_t>(fir);
    writeRegister(CONF_REG_2, regValue);
}

void ads1120_t::setLowSidePowerSwitch(PSW psw)
{
    auto regValue = readRegister(CONF_REG_2);
    regValue &= ~0x08;
    regValue |= static_cast<uint8_t>(psw);
    writeRegister(CONF_REG_2, regValue);
}

void ads1120_t::setIdacCurrent(IdacCurrent current)
{
    auto regValue = readRegister(CONF_REG_2);
    regValue &= ~0x07;
    regValue |= static_cast<uint8_t>(current);
    writeRegister(CONF_REG_2, regValue);
}

/* Configuration Register 3 settings */

void ads1120_t::setIdac1Routing(IdacRouting route)
{
    auto regValue = readRegister(CONF_REG_3);
    regValue &= ~0xE0;
    regValue |= (static_cast<uint8_t>(route) << 5);
    writeRegister(CONF_REG_3, regValue);
}

void ads1120_t::setIdac2Routing(IdacRouting route)
{
    auto regValue = readRegister(CONF_REG_3);
    regValue &= ~0x1C;
    regValue |= (static_cast<uint8_t>(route) << 2);
    writeRegister(CONF_REG_3, regValue);
}

void ads1120_t::setDrdyMode(DrdyMode mode)
{
    auto regValue = readRegister(CONF_REG_3);
    regValue &= ~0x02;
    regValue |= static_cast<uint8_t>(mode);
    writeRegister(CONF_REG_3, regValue);
}

/* Other settings */
void ads1120_t::setAvddAvssAsVrefAndCalibrate()
{
    setVRefSource(VRef::VREF_AVDD_AVSS);
    setCompareChannels(Mux::MUX_AVDD_M_AVSS_4);

    float avssVoltage = 0.0f;
    for (int i = 0; i < 10; i++) {
        avssVoltage += getVoltage_mV();
    }
    params.vRef = avssVoltage * 4.0f / 10000.0f;
}

void ads1120_t::setRefp0Refn0AsVefAndCalibrate()
{
    setVRefSource(VRef::VREF_REFP0_REFN0);
    setCompareChannels(Mux::MUX_REFPX_REFNX_4);

    float ref0Voltage = 0.0f;
    for (int i = 0; i < 10; i++) {
        ref0Voltage += getVoltage_mV();
    }
    params.vRef = ref0Voltage * 4.0f / 10000.0f;
}

void ads1120_t::setRefp1Refn1AsVefAndCalibrate()
{
    setVRefSource(VRef::VREF_REFP1_REFN1);
    setCompareChannels(Mux::MUX_REFPX_REFNX_4);

    float ref1Voltage = 0.0;
    for (int i = 0; i < 10; i++) {
        ref1Voltage += getVoltage_mV();
    }
    params.vRef = ref1Voltage * 4.0f / 10000.0f;
}

void ads1120_t::setIntVRef()
{
    setVRefSource(VRef::VREF_INT);
    params.vRef = 2.048f;
}

/* Results */
float ads1120_t::getVoltage_mV()
{
    int32_t rawData = getData();
    float resultInMV = 0.0f;
    if (params.refMeasurement) {
        resultInMV = (rawData / RANGE) * 2.048f * 1000.0f / (params.gain * 1.0f);
    } else {
        resultInMV = (rawData / RANGE) * params.vRef * 1000.0f / (params.gain * 1.0f);
    }
    return resultInMV;
}

int32_t ads1120_t::getVoltage_mV_int(uint16_t vref_mv, uint8_t gain)
{
    int32_t rawData = getData();
    // 16 bit * 16 bit max - overflow not possible?
    // Check ADC datasheet
    int32_t ret = rawData * vref_mv / RANGE_UINT / gain;
    return ret;
}

float ads1120_t::getVoltage_uV()
{
    return getVoltage_mV() * 1000.0f;
}

int64_t ads1120_t::getVoltage_uV_int(uint16_t vref_mv, uint8_t gain)
{
    return getVoltage_mV_int(vref_mv, gain) * 1000;
}

int16_t ads1120_t::getRawData()
{
    return getData();
}

float ads1120_t::getTemperature()
{
    enableTemperatureSensor(true);
    int16_t rawResult = getLastResult();
    enableTemperatureSensor(false);

    uint16_t result = (rawResult >> 2);
    if (result >> 13) {
        result = ~(result - 1) & 0x3777;
        return result * (-0.03125f);
    }
    return result * 0.03125f;
}

// TODO check this with negative temperature
int32_t ads1120_t::getTemperature_int()
{
    enableTemperatureSensor(true);
    int16_t rawResult = getLastResult();
    enableTemperatureSensor(false);

    uint16_t result = (rawResult >> 2);
    // check sign
    if (result >> 13) {
        result = ~(result - 1) & 0x3777;
        return -result * 3125 / 10000;
    }
    return result * 3125 / 100000;
}

/************************************************
    private functions
*************************************************/
void ads1120_t::forcedBypassPGA()
{
    auto regValue = readRegister(CONF_REG_0);
    regValue |= 0x01;
    writeRegister(CONF_REG_0, regValue);
}

// TODO: check strict aliasing
int16_t ads1120_t::getData()
{
    union Data {
        uint16_t rawResult;
        int16_t result;
    };

    union Data data;
    data.rawResult = getLastResult();
    return data.result;
}

void ads1120_t::on_ready_irq()
{
    //NVIC_DisableIRQ(EXTI0_1_IRQn);
    // HAL_GPIO_WritePin(LED_S_GPIO_Port, LED_S_Pin, GPIO_PIN_SET);

    uint8_t buf[2];
    cs_.reset();
    spi_.read({buf, 2});
    cs_.set();

    int32_t r = ((int8_t)buf[0] << 8) | buf[1];
    last_result = r;
    if (cb_)
        cb_(ctx_, last_result);
}

uint16_t ads1120_t::getLastResult()
{
    return last_result;
}

uint8_t ads1120_t::readRegister(uint8_t reg)
{
    uint8_t regValue = 0;
    uint8_t buf = (uint8_t)(RREG | reg);

    cs_.reset();
    spi_.write({&buf, 1});
    spi_.read({&regValue, 1});
    cs_.set();
    return regValue;
}

void ads1120_t::writeRegister(uint8_t reg, uint8_t val)
{
    uint8_t buf = (uint8_t)(WREG | reg);
    cs_.reset();
    spi_.write({&buf, 1});
    spi_.read({&val, 1});
    cs_.set();
}

void ads1120_t::command(uint8_t cmd)
{
    cs_.reset();
    spi_.write({&cmd, 1});
    cs_.set();
}

}  // namespace common

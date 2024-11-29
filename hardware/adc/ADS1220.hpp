#pragma once

#include "ZOQ/itf/gpio/i_pin_it_itf.hpp"
#include "ZOQ/itf/gpio/o_pin_itf.hpp"
#include "ZOQ/itf/spi/spi_itf.hpp"

namespace common {

using namespace ZOQ::itf;

// ADS1120 class
// Minimum SPI SCK period must be 150 ns
class ads1220_t {
  public:
    // Forward decalrations
    enum class DrdyMode : uint8_t;
    enum class IdacRouting : uint8_t;
    enum class IdacCurrent : uint8_t;
    enum class Mux : uint8_t;
    enum class Gain : uint8_t;
    enum class PgaBypass : uint8_t;
    enum class DataRate : uint8_t;
    enum class OpMode : uint8_t;
    enum class ConvMode : uint8_t;
    enum class TsMode : uint8_t;
    enum class Burnout : uint8_t;
    enum class VRef : uint8_t;
    enum class FIR : uint8_t;
    enum class PSW : uint8_t;

    // Callback function
    using on_sampling_complete_cb = void (*)(void* ctx, int32_t raw_result);

    static constexpr uint32_t RANGE_UINT = 0x7fffff;
    static constexpr uint16_t REF_MV_UINT = 2048;

    ads1220_t(spi_itf& spi, o_pin_itf& cs, i_pin_it_itf& rdy);
    void register_callback(const on_sampling_complete_cb& cb, void* ctx)
    {
        cb_ = cb;
        ctx_ = ctx;
    }

    struct config_t {
        Gain gain;
        IdacRouting routing;
        IdacCurrent current;
        Mux mux;
        PgaBypass pga_bypass;
        VRef ref;
        TsMode ts;
    };

    void setup(const config_t& config);

    /* Commands */
    uint8_t init();
    void start();
    void reset();
    void powerDown();

    void set_config_0(Mux mux, Gain, PgaBypass pga_bypass);
    void set_config_1(DataRate dr, OpMode opmode, ConvMode cm, TsMode ts, Burnout burnout);
    void set_config_2(VRef vfref, FIR fir, PSW psw, IdacCurrent idac_cur);
    void set_config_3(IdacRouting idac1, IdacRouting idac2, DrdyMode mode);

    uint16_t getLastResult();

    void on_ready_irq();

    ads1220_t(const ads1220_t&) = delete;
    ads1220_t& operator=(const ads1220_t&) = delete;

  private:
    spi_itf& spi_;
    o_pin_itf& cs_;
    i_pin_it_itf& drdy_;

    on_sampling_complete_cb cb_ = nullptr;
    void* ctx_ = nullptr;

    int32_t last_result = 0;

    uint8_t readRegister(uint8_t reg);
    void writeRegister(uint8_t reg, uint8_t val);
    void command(uint8_t cmd);
};

enum class ads1220_t::Mux : uint8_t {
    MUX_0_1 = 0x00,  // default
    MUX_0_2 = 0x10,
    MUX_0_3 = 0x20,
    MUX_1_2 = 0x30,
    MUX_1_3 = 0x40,
    MUX_2_3 = 0x50,
    MUX_1_0 = 0x60,
    MUX_3_2 = 0x70,
    MUX_0_AVSS = 0x80,
    MUX_1_AVSS = 0x90,
    MUX_2_AVSS = 0xA0,
    MUX_3_AVSS = 0xB0,
    MUX_REFPX_REFNX_4 = 0xC0,
    MUX_AVDD_M_AVSS_4 = 0xD0,
    MUX_AVDD_P_AVSS_2 = 0xE0
};

enum class ads1220_t::Gain : uint8_t {
    GAIN_1 = 0x00,  // default
    GAIN_2 = 0x02,
    GAIN_4 = 0x04,
    GAIN_8 = 0x06,
    GAIN_16 = 0x08,
    GAIN_32 = 0x0A,
    GAIN_64 = 0x0C,
    GAIN_128 = 0x0E
};

enum class ads1220_t::PgaBypass : uint8_t {
    pga_enabled = 0x00,  // default
    pga_disabled = 0x01
};

enum class ads1220_t::DataRate : uint8_t {
    DR_LVL_0 = 0x00,  // default
    DR_LVL_1 = 0x20,
    DR_LVL_2 = 0x40,
    DR_LVL_3 = 0x60,
    DR_LVL_4 = 0x80,
    DR_LVL_5 = 0xA0,
    DR_LVL_6 = 0xC0
};

enum class ads1220_t::OpMode : uint8_t {
    NORMAL_MODE = 0x00,  // default
    DUTY_CYCLE_MODE = 0x08,
    TURBO_MODE = 0x10
};

enum class ads1220_t::ConvMode : uint8_t {
    SINGLE_SHOT = 0x00,  // default
    CONTINUOUS = 0x04
};

enum class ads1220_t::TsMode : uint8_t {
    TS_DISABLED = 0x00,  // default
    TS_ENABLED = 0x02
};

enum class ads1220_t::Burnout : uint8_t {
    BCS_DISABLED = 0x00,  // default
    BCS_ENABLED = 0x01
};

enum class ads1220_t::VRef : uint8_t {
    VREF_INT = 0x00,  // default
    VREF_REFP0_REFN0 = 0x40,
    VREF_REFP1_REFN1 = 0x80,
    VREF_AVDD_AVSS = 0xC0
};

enum class ads1220_t::FIR : uint8_t {
    NONE = 0x00,  // default
    HZ50_HZ60 = 0x10,
    HZ50 = 0x20,
    HZ60 = 0x30
};

enum class ads1220_t::PSW : uint8_t {
    ALWAYS_OPEN = 0x00,  // default
    SWITCH = 0x08
};

enum class ads1220_t::DrdyMode : uint8_t {
    DRDY = 0x00,  // default
    DOUT_DRDY = 0x02
};

enum class ads1220_t::IdacRouting : uint8_t {
    IDAC_NONE = 0x00,  // default
    IDAC_AIN0_REFP1 = 0x01,
    IDAC_AIN1 = 0x02,
    IDAC_AIN2 = 0x03,
    IDAC_AIN3_REFN1 = 0x04,
    IDAC_REFP0 = 0x05,
    IDAC_REFN0 = 0x06,
};

enum class ads1220_t::IdacCurrent : uint8_t {
    IDAC_OFF = 0x00,  // default
    IDAC_10_MU_A = 0x01,
    IDAC_50_MU_A = 0x02,
    IDAC_100_MU_A = 0x03,
    IDAC_250_MU_A = 0x04,
    IDAC_500_MU_A = 0x05,
    IDAC_1000_MU_A = 0x06,
    IDAC_1500_MU_A = 0x07
};

inline constexpr uint8_t gain_to_divider(ads1220_t::Gain g)
{
    switch (g) {
        case ads1220_t::Gain::GAIN_1:
            return 1;
        case ads1220_t::Gain::GAIN_2:
            return 2;
        case ads1220_t::Gain::GAIN_4:
            return 4;
        case ads1220_t::Gain::GAIN_8:
            return 8;
        case ads1220_t::Gain::GAIN_16:
            return 16;
        case ads1220_t::Gain::GAIN_32:
            return 32;
        case ads1220_t::Gain::GAIN_64:
            return 64;
        case ads1220_t::Gain::GAIN_128:
            return 128;
        default:
            return 1;
    }
};

}  // namespace common

#pragma once

#include "ZOQ/callback.hpp"
#include "stm32f4xx_hal.h"

namespace ZOQ::stm32_hal::stm32f4xx::adc {

using namespace ZOQ;

class adc_channel_it_t {
  public:
    using fn_t = void (*)(uint32_t value, void* ctx);
    adc_channel_it_t(ADC_HandleTypeDef& handle, uint32_t channel, uint32_t sample_time);

    constexpr void register_handler(fn_t measure_end_cb, void* measure_end_ctx)
    {
        _measure_end_cb = measure_end_cb;
        _measure_end_ctx = measure_end_ctx;
    }

    void measure();
    void on_measure_end() const;
    static inline adc_channel_it_t* _instance;

  private:
    ADC_HandleTypeDef& _handle;
    const uint32_t _channel;
    const uint32_t _sample_time;

    fn_t _measure_end_cb = nullptr;
    void* _measure_end_ctx = nullptr;
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::adc

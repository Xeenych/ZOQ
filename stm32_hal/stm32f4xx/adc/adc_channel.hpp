#pragma once

#include "stm32f4xx_hal.h"

namespace ZOQ::stm32_hal::stm32f4xx::adc {

class adc_channel_t {
  public:
    constexpr adc_channel_t(ADC_HandleTypeDef& handle, uint32_t channel, uint32_t sample_time)
        : _handle{handle}, _channel{channel}, _sample_time{sample_time}
    {
    }
    uint32_t measure() const;

  private:
    ADC_HandleTypeDef& _handle;
    const uint32_t _channel;
    const uint32_t _sample_time;
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::adc

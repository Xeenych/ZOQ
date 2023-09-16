#include "adc_channel.hpp"

#include <cassert>

namespace ZOQ::stm32_hal::stm32f4xx::adc {

uint32_t adc_channel_t::measure() const
{
    ADC_ChannelConfTypeDef sConfig = {.Channel = _channel, .Rank = 1, .SamplingTime = _sample_time, .Offset = 0};
    assert(HAL_OK == HAL_ADC_ConfigChannel(&_handle, &sConfig));
    {
        auto status = HAL_ADC_Start(&_handle);
        assert(HAL_OK == status);
    }

    {
        auto status = HAL_ADC_PollForConversion(&_handle, HAL_MAX_DELAY);
        assert(HAL_OK == status);
    }

    return HAL_ADC_GetValue(&_handle);
}

}  // namespace ZOQ::stm32_hal::stm32f4xx::adc

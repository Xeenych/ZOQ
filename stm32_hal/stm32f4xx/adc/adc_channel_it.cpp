#define LOG_ENABLE 0

#include "adc_channel_it.hpp"

#include <cassert>

#include "logging.h"

using namespace ZOQ::stm32_hal::stm32f4xx::adc;

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    LOG_DBG("HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)");
    adc_channel_it_t::_instance->on_measure_end();
}

namespace ZOQ::stm32_hal::stm32f4xx::adc {

void adc_channel_it_t::on_measure_end() const
{
    LOG_DBG("adc_channel_it_t::on_measure_end()");
    auto value = HAL_ADC_GetValue(&_handle);
    _measure_end_cb(value, _measure_end_ctx);
}

void adc_channel_it_t::measure() const
{
    LOG_DBG("adc_channel_it_t::measure()");
    ADC_ChannelConfTypeDef sConfig = {.Channel = _channel, .Rank = 1, .SamplingTime = _sample_time, .Offset = 0};
    {
        auto status = HAL_ADC_ConfigChannel(&_handle, &sConfig);
        assert(HAL_OK == status);
    }

    {
        auto status = HAL_ADC_Start_IT(&_handle);
        assert(HAL_OK == status);
    }
    LOG_DBG("adc_channel_it_t::measure() END");
}

}  // namespace ZOQ::stm32_hal::stm32f4xx::adc

#define LOG_ENABLE 0

#include "adc_channel_it.hpp"

#include <cassert>

#include "logging.h"

namespace ZOQ::stm32_hal::stm32f4xx::adc {

void adc_channel_it_t::register_interrupt_handler(ADC_HandleTypeDef* hadc, adc_channel_it_t* instance)
{
    adc_handler.hadc = hadc;
    adc_handler.instance = instance;
}

void adc_channel_it_t::HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (adc_channel_it_t::adc_handler.hadc == hadc)
        adc_channel_it_t::adc_handler.instance->on_measure_end();
}

void adc_channel_it_t::on_measure_end() const
{
    auto value = HAL_ADC_GetValue(&_handle);
    LOG_DBG("adc_channel_it_t::on_measure_end(), value: %d", value);
    if (_measure_end_cb)
        _measure_end_cb(value, _measure_end_ctx);
}

void adc_channel_it_t::measure()
{
    LOG_DBG("adc_channel_it_t::measure()");
    ADC_ChannelConfTypeDef sConfig = {.Channel = _channel, .Rank = 1, .SamplingTime = _sample_time, .Offset = 0};
    {
        auto status = HAL_ADC_ConfigChannel(&_handle, &sConfig);
        assert(HAL_OK == status);
    }

    register_interrupt_handler(&_handle, this);

    {
        auto status = HAL_ADC_Start_IT(&_handle);
        assert(HAL_OK == status);
    }
}

}  // namespace ZOQ::stm32_hal::stm32f4xx::adc

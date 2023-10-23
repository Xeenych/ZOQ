#define LOG_ENABLE 0

#include "adc_channel_it.hpp"

#include <cassert>

#include "logging.h"

using namespace ZOQ::stm32_hal::stm32f4xx::adc;

struct handler_t {
    ADC_HandleTypeDef* hadc;
    adc_channel_it_t* instance;
};

static handler_t adc_handlers[10]{};

static void register_interrupt_handler(ADC_HandleTypeDef* hadc, adc_channel_it_t* instance)
{
    for (auto& h : adc_handlers)
        if (!h.hadc) {
            h.hadc = hadc;
            h.instance = instance;
            return;
        }
    assert(false);
}

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    LOG_DBG("HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)");
    for (const auto& h : adc_handlers)
        if (h.hadc == hadc)
            h.instance->on_measure_end();
}

namespace ZOQ::stm32_hal::stm32f4xx::adc {

adc_channel_it_t::adc_channel_it_t(ADC_HandleTypeDef& handle, uint32_t channel, uint32_t sample_time)
    : _handle(handle), _channel(channel), _sample_time(sample_time)

{
    register_interrupt_handler(&handle, this);
};

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

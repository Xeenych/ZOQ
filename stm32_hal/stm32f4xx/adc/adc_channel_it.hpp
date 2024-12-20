#pragma once

#include "stm32f4xx_hal.h"

namespace ZOQ::stm32_hal::stm32f4xx::adc {

class adc_channel_it_t {
  public:
    using fn_t = void (*)(uint32_t value, void* ctx);
    constexpr adc_channel_it_t(ADC_HandleTypeDef& handle, uint32_t channel, uint32_t sample_time)
        : _handle(handle), _channel(channel), _sample_time(sample_time) {}

    constexpr void set_callback(fn_t measure_end_cb, void* measure_end_ctx) {
        _measure_end_cb = measure_end_cb;
        _measure_end_ctx = measure_end_ctx;
    }

    void measure();
    void on_measure_end() const;

    static inline adc_channel_it_t* _instance{};
    static void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);

  private:
    ADC_HandleTypeDef& _handle;
    const uint32_t _channel;
    const uint32_t _sample_time;

    fn_t _measure_end_cb = nullptr;
    void* _measure_end_ctx = nullptr;

    struct handler_t {
        ADC_HandleTypeDef* hadc = nullptr;
        adc_channel_it_t* instance = nullptr;
    };
    // There is only anoe ADC channel may be active at a time
    static inline handler_t adc_handler{nullptr, nullptr};
    void register_interrupt_handler(ADC_HandleTypeDef* hadc, adc_channel_it_t* instance);
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::adc

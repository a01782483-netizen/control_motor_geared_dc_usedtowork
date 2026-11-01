#include "RPM_CONTROLLER.h"

RPM_CONTROLLER *RPM_CONTROLLER::_instance = nullptr;

RPM_CONTROLLER::RPM_CONTROLLER()
{
}

void RPM_CONTROLLER::setup(gpio_num_t encoder_pin, uint32_t sample_time_ms)
{
    _encoder_pin = encoder_pin;
    _sample_time_ms = sample_time_ms;
    _Encoder.setup(_encoder_pin, GPIO_MODE_INPUT);

    _instance = this;

    _Encoder.addInterrupt(GPIO_INTR_POSEDGE, interr_encoder, DEBOUNCET);

    _timer = esp_timer_get_time();
}

void RPM_CONTROLLER::setPI(float kp, float ki)
{
    _kp = kp;
    _ki = ki;
}

float RPM_CONTROLLER::rpm(float desired_rpm)
{
    float _desired_rpm = desired_rpm;

    int64_t now = esp_timer_get_time();
    int64_t elapsed_us = now - _timer;
    float dt = elapsed_us / 1000000.0f;

    if (elapsed_us >= _sample_time_ms * 1000LL)
    {
        uint32_t pulsos = _pulse_count;
        _pulse_count = 0;
        _last_rpm = pulsos * 60000000.0f / (493.9f * elapsed_us);
        _timer = now;
        _error_CL = _desired_rpm - _last_rpm;
        _integral += _error_CL * dt;
        _adj_pwm = (_kp * _error_CL) + (_ki * _integral);
    }

    return _adj_pwm;
}

void IRAM_ATTR RPM_CONTROLLER::interr_encoder(void *arg)
{
    _instance->_pulse_count++;
}

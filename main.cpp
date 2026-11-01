#include "definitions.h"

extern "C" void app_main()
{
    SimplePWM pwm;
    pwm.setup(PWM_OUT, 0);
    pwm.setDuty(0);

    SimpleGPIO dir_1;
    dir_1.setup(DIR_1, GPIO_MODE_OUTPUT);
    dir_1.set(1);

    SimpleGPIO dir_2;
    dir_2.setup(DIR_2, GPIO_MODE_OUTPUT);
    dir_2.set(0);

    RPM_CONTROLLER controller;
    controller.setup(ENCODER_OUT, SAMPLING_TIME);
    controller.setPI(kp, ki);

    int64_t inicio = esp_timer_get_time();

    while (esp_timer_get_time() - inicio < 30000000LL)
    {
        out_pwm = controller.rpm(30.0f);

        // Aquí falta limitar al rango real de setDuty().
        pwm.setDuty(out_pwm);
    }

    pwm.setDuty(0);
}
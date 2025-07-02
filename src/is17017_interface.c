#include "is17017_interface.h"

#include "board.h"

extern uint16_t get_cp_adc_value();

bool set_pwm_duty_cycle(float duty_percent)
{
    USER_TIMER_PWM_SetDutyCycle(IS17017_PWM_TIMER, IS17017_PWM_TIMER_CHANNEL, IS17017_PWM_TIME_PERIOD, duty_percent);

    return true;
}

float get_cp_voltage(void)
{
    uint16_t cp_adc_value = 0;
    float cp_voltage = 0;

    cp_adc_value = get_cp_adc_value();

    cp_voltage = ((cp_adc_value*3.3)/4095);

    return cp_voltage;
}
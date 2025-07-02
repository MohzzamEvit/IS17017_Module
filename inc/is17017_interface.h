#ifndef IS17017_INTERFACE_H
#define IS17017_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Set the PWM duty cycle at the hardware level
 * @param duty_percent PWM duty cycle percentage
 * @return true if successful
 */
bool set_pwm_duty_cycle(float duty_percent);

/**
 * @brief Read the pilot line voltage via ADC
 * @return Voltage in volts
 */
float get_cp_voltage(void);

#endif // is17017_IF_H

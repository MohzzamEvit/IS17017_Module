#ifndef IS17017_DLL_H
#define IS17017_DLL_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief IEC 61851 pilot signal states
 */
typedef enum {
    IS17017_STATE_A = 0,
    IS17017_STATE_B,
    IS17017_STATE_C,
    IS17017_STATE_D,
    IS17017_STATE_E,
    IS17017_STATE_F
} is17017_state_t;

/**
 * @brief Set PWM duty cycle corresponding to charge current
 * @param duty_percent PWM duty cycle (e.g., 10 to 96)
 */
void is17017_set_pwm(float duty_percent);

// ----------------------------------------------------
// Wrapper to read CP voltage (pilot signal)
// ----------------------------------------------------
float is17017_get_cp_voltage(void);

/**
 * @brief Get the current vehicle state (State A/B/C/D/E)
 * @return IEC 61851 state code (0 = A, 1 = B, ...)
 */
is17017_state_t is17017_get_state(void);

#endif // is17017_H

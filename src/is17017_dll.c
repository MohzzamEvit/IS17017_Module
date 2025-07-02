#include "is17017_dll.h"

#include "is17017_interface.h"

void is17017_set_pwm(float duty_percent)
{
    set_pwm_duty_cycle(duty_percent);
}

is17017_state_t is17017_get_state(void)
{
    float pilot_voltage = 0;
    static is17017_state_t curr_state;

    pilot_voltage = get_cp_voltage();

    if ((pilot_voltage >= 3.22) && (pilot_voltage <= 3.3))
    {
      curr_state = IS17017_STATE_A;
    }
    else if ((pilot_voltage >= 2.69) && (pilot_voltage < 2.88))
    {
      curr_state = IS17017_STATE_B;
    }
    else if ((pilot_voltage >= 2.21) && (pilot_voltage < 2.45))
    {
      curr_state = IS17017_STATE_C;
    }
    else if ((pilot_voltage >= 1.8) && (pilot_voltage < 2))
    {
      curr_state = IS17017_STATE_D;
    }
    else if ((pilot_voltage >= 1.45) && (pilot_voltage < 1.65))
    {
      curr_state = IS17017_STATE_E;
    }
    else if ((pilot_voltage >= 0) && (pilot_voltage < 1))
    {
      curr_state = IS17017_STATE_F;
    }

    return curr_state;
}
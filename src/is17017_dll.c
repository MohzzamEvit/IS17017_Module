#include "is17017_dll.h"

#include "is17017_interface.h"

extern uint32_t get_ms_tick();

void is17017_set_pwm(float duty_percent)
{
    set_pwm_duty_cycle(duty_percent);
}

//is17017_state_t is17017_get_state(void)
//{
//    float pilot_voltage = 0;
//    static is17017_state_t curr_state;
//
//    pilot_voltage = get_cp_voltage();
//
//    if ((pilot_voltage >= 3.22) && (pilot_voltage <= 3.3))
//    {
//      curr_state = IS17017_STATE_A;
//    }
//    else if ((pilot_voltage >= 2.69) && (pilot_voltage < 2.88))
//    {
//      curr_state = IS17017_STATE_B;
//    }
//    else if ((pilot_voltage >= 2.21) && (pilot_voltage < 2.45))
//    {
//      curr_state = IS17017_STATE_C;
//    }
//    else if ((pilot_voltage >= 1.8) && (pilot_voltage < 2))
//    {
//      curr_state = IS17017_STATE_D;
//    }
//    else if ((pilot_voltage >= 1.45) && (pilot_voltage < 1.65))
//    {
//      curr_state = IS17017_STATE_E;
//    }
//    else if ((pilot_voltage >= 0) && (pilot_voltage < 1))
//    {
//      curr_state = IS17017_STATE_F;
//    }
//
//    return curr_state;
//}

is17017_state_t is17017_get_state(void)
{
  float pilot_voltage = get_cp_voltage();
  static is17017_state_t curr_state = IS17017_STATE_A;
  static is17017_state_t temp_state = IS17017_STATE_A;
  static uint32_t state_start_time = 0;
  
  is17017_state_t new_state = IS17017_STATE_A;
  
  // Determine new_state from pilot voltage
  if ((pilot_voltage >= 3.22) && (pilot_voltage <= 3.3))
    new_state = IS17017_STATE_A;
  else if ((pilot_voltage >= 2.69) && (pilot_voltage < 2.88))
    new_state = IS17017_STATE_B;
  else if ((pilot_voltage >= 2.21) && (pilot_voltage < 2.45))
    new_state = IS17017_STATE_C;
  else if ((pilot_voltage >= 1.8) && (pilot_voltage < 2))
    new_state = IS17017_STATE_D;
  else if ((pilot_voltage >= 1.45) && (pilot_voltage < 1.65))
    new_state = IS17017_STATE_E;
  else if ((pilot_voltage >= 0) && (pilot_voltage < 1))
    new_state = IS17017_STATE_F;
  
  // If new state is different from temp_state, reset timer and update temp_state
  if (new_state != temp_state)
  {
    temp_state = new_state;
    state_start_time = get_ms_tick(); // Start timing
  }
  
  // If temp_state remains stable for 2 seconds, accept it as current state
  if ((get_ms_tick() - state_start_time) >= 500)
  {
    if (curr_state != temp_state)
    {
      curr_state = temp_state;
    }
  }
  
  return curr_state;
}

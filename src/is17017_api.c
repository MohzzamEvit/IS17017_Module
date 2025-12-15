#include "is17017_api.h"

#include "cust_common.h"

static float calculate_pwm_duty_from_current(uint8_t current_limit_A);

void iec61851_prev_state_a(is17017_state_t curr_state);
void iec61851_prev_state_b(is17017_state_t curr_state);
void iec61851_prev_state_cd(is17017_state_t curr_state);
void iec61851_prev_state_ef(is17017_state_t curr_state);

static is17017_fault_status_t fault_status = IS17017_FAULT_NOT_PRESENT;
static is17017_ventilation_required_t ventilation_status = IS17017_VENTILATION_NOT_REQUIRED;
static is17017_gun_state_t gun_state = IS17017_GUN_DISCONNECTED;
static is17017_charging_state_t charging_state = IS17017_CHARGING_NOT_ACTIVE;
static is17017_pwm_state_t pwm_state = IS17017_PWM_DISABLED;

static is17017_state_t pilot_state = IS17017_STATE_A;

static uint8_t pilot_current_limit_A = 0;

void is17017_state_flow(void)
{
    static is17017_state_t curr_pilot_state = IS17017_STATE_A;
    static is17017_state_t prev_pilot_state = IS17017_STATE_F;

    curr_pilot_state = is17017_get_state();

    if (curr_pilot_state != prev_pilot_state)
    {
        pilot_state = curr_pilot_state;

        switch (prev_pilot_state)
        {
        case IS17017_STATE_A:
            iec61851_prev_state_a(curr_pilot_state);
            break;

        case IS17017_STATE_B:
            iec61851_prev_state_b(curr_pilot_state);
            break;

        case IS17017_STATE_C:
        case IS17017_STATE_D:
            iec61851_prev_state_cd(curr_pilot_state);
            break;

        case IS17017_STATE_E:
        case IS17017_STATE_F:
            iec61851_prev_state_ef(curr_pilot_state);
            break;
        }
        
        prev_pilot_state = curr_pilot_state;
    }
    
    
}

bool is17017_vehicle_initialize(bool *ventilation_required)
{
  is17017_state_flow();
  
  if (charging_state == IS17017_CHARGING_ACTIVE)
  {
    *ventilation_required = ventilation_status;
    return true;
  }
  
  return false;
}

bool is17017_check_gun_connection(void)
{
    return gun_state;
}

bool is17017_check_charger_avalaibiltys(void)
{
    return pwm_state;
}

bool is17017_is_vehicle_charging(void)
{
    return charging_state;
}

bool is17017_is_vehicle_in_fault_state(void)
{
    return fault_status;
}

is17017_state_t is17017_get_vehicle_state(void)
{
    return pilot_state;
}

void is17017_set_charging_current(uint8_t current_A)
{
    pilot_current_limit_A = current_A;
    
    if (pwm_state == IS17017_PWM_ENABLED)
    {
      float duty_cycle = 0;
      
      duty_cycle = calculate_pwm_duty_from_current(pilot_current_limit_A);
      is17017_set_pwm(duty_cycle);
    }
}

void is17017_set_charger_availability(bool available)
{
    if (available)
    {
        float duty_cycle = 0;

        duty_cycle = calculate_pwm_duty_from_current(pilot_current_limit_A);

        is17017_set_pwm(duty_cycle);
        pwm_state = IS17017_PWM_ENABLED;
    }
    else
    {
        is17017_set_pwm(100);
        pwm_state = IS17017_PWM_DISABLED;
    }
}

/**
 * @brief Called when pilot previous State A 
 */
void iec61851_prev_state_a(is17017_state_t curr_state)
{
    switch (curr_state)
    {
    case IS17017_STATE_B:
        gun_state = IS17017_GUN_CONNECTED; // seq 1.1
        break;

    case IS17017_STATE_C:
    case IS17017_STATE_D:
        gun_state = IS17017_GUN_CONNECTED; // seq 1.2
        charging_state = IS17017_CHARGING_ACTIVE; 
        
        if (curr_state == IS17017_STATE_D)
        {
          ventilation_status = IS17017_VENTILATION_REQUIRED;
        }
        break;

    case IS17017_STATE_E:
    case IS17017_STATE_F:
        fault_status = IS17017_FAULT_PRESENT; // seq 12
        break;
    }
}

/**
 * @brief Called when pilot previous State B
 */
void iec61851_prev_state_b(is17017_state_t curr_state)
{
    switch (curr_state)
    {
    case IS17017_STATE_A:
        gun_state = IS17017_GUN_DISCONNECTED; // seq 2.1
        break;

    case IS17017_STATE_C:
    case IS17017_STATE_D:
        charging_state = IS17017_CHARGING_ACTIVE; // seq 4

        if (curr_state == IS17017_STATE_D)
        {
            ventilation_status = IS17017_VENTILATION_REQUIRED;
        }
        break;

    case IS17017_STATE_E:
    case IS17017_STATE_F:
        fault_status = IS17017_FAULT_PRESENT; // seq 12
        break;
    }
}

/**
 * @brief Called when pilot previous State C or D
 */
void iec61851_prev_state_cd(is17017_state_t curr_state)
{
    switch (curr_state)
    {
    case IS17017_STATE_A:
        gun_state = IS17017_GUN_DISCONNECTED; // seq 2.2
        charging_state = IS17017_CHARGING_NOT_ACTIVE;
        break;

    case IS17017_STATE_B:
        if (pwm_state == IS17017_PWM_ENABLED) // seq 7
        {
            charging_state = IS17017_CHARGING_NOT_ACTIVE; // seq 8.1
        }
        else // seq 10.1
        {
            charging_state = IS17017_CHARGING_NOT_ACTIVE; // seq 8.2
        }
        break;

    case IS17017_STATE_E:
    case IS17017_STATE_F:
        charging_state = IS17017_CHARGING_NOT_ACTIVE; // seq 8.1
        fault_status = IS17017_FAULT_PRESENT; // seq 12
        break;
    }
}

/**
 * @brief Called when pilot previous State E or F
 */
void iec61851_prev_state_ef(is17017_state_t curr_state)
{
    switch (curr_state)
    {
    case IS17017_STATE_A:
        gun_state = IS17017_GUN_DISCONNECTED; // seq 2.1
        break;

    case IS17017_STATE_B:
        gun_state = IS17017_GUN_CONNECTED; // seq 1.1
        break;

    case IS17017_STATE_C:
        gun_state = IS17017_GUN_CONNECTED; // seq 1.1
        break;
    }
}

static float calculate_pwm_duty_from_current(uint8_t current_limit_A)
{
  float duty;
  
  /* ===============================
  * 0 A ? No current allowed
  * =============================== */
  if (current_limit_A == 0)
  {
    return IEC_PWM_NO_CURRENT;
  }
  
  /* ===============================
  * Enforce IEC current limits
  * =============================== */
  if (current_limit_A < IEC_MIN_CURRENT_A)
  {
    current_limit_A = IEC_MIN_CURRENT_A;
  }
  else if (current_limit_A > IEC_MAX_CURRENT_A)
  {
    current_limit_A = IEC_MAX_CURRENT_A;
  }
  
  /* ===============================
  * IEC linear region (10–85%)
  * I = Duty × 0.6
  * =============================== */
  if (current_limit_A <= (IEC_PWM_LINEAR_MAX * IEC_PWM_SLOPE_LINEAR))
  {
    duty = current_limit_A / IEC_PWM_SLOPE_LINEAR;
  }
  /* ===============================
  * IEC high current region (85–96%)
  * I = (Duty - 64) × 2.5
  * =============================== */
  else
  {
    duty = (current_limit_A / IEC_PWM_SLOPE_HIGH) + IEC_PWM_HIGH_OFFSET;
  }
  
  /* ===============================
  * Clamp to valid IEC range
  * =============================== */
  if (duty < IEC_PWM_MIN_ACTIVE)
  {
    duty = IEC_PWM_MIN_ACTIVE;
  }
  else if (duty > IEC_PWM_MAX_ACTIVE)
  {
    duty = IEC_PWM_MAX_ACTIVE;
  }
  
  return duty;
}


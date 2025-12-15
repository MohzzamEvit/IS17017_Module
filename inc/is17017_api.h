#ifndef IS17017_API_H
#define IS17017_API_H

#include "is17017_dll.h"

#include <stdint.h>
#include <stdbool.h>

/* ===============================
 * IEC 61851 – Current limits
 * =============================== */
#define IEC_MIN_CURRENT_A        6
#define IEC_MAX_CURRENT_A        80

/* ===============================
 * IEC 61851 – PWM duty limits (%)
 * =============================== */
#define IEC_PWM_NO_CURRENT       7.5f    /* 7–8% : No current allowed */
#define IEC_PWM_MIN_ACTIVE       10.0f   /* ≥10% : Current allowed */
#define IEC_PWM_MAX_ACTIVE       96.0f   /* ≤96% : Max valid */

/* ===============================
 * IEC 61851 – Region boundaries
 * =============================== */
#define IEC_PWM_LINEAR_MAX       85.0f   /* 10–85% : I = D × 0.6 */
#define IEC_PWM_HIGH_MIN         85.0f   /* >85% */
#define IEC_PWM_HIGH_MAX         96.0f   /* ≤96% */

/* ===============================
 * IEC 61851 – Slopes
 * =============================== */
#define IEC_PWM_SLOPE_LINEAR     0.6f    /* A per % */
#define IEC_PWM_SLOPE_HIGH       2.5f    /* A per % */
#define IEC_PWM_HIGH_OFFSET      64.0f   /* IEC defined offset */

typedef enum
{
    IS17017_PWM_DISABLED = 0,         // 12V DC → State A (No PWM, no vehicle)
    IS17017_PWM_ENABLED              // PWM is active (standard mode)
} is17017_pwm_state_t;

typedef enum
{
    IS17017_GUN_DISCONNECTED = 0,   // State A — no vehicle connected
    IS17017_GUN_CONNECTED          // State B/C/D — vehicle connected
} is17017_gun_state_t;

typedef enum
{
    IS17017_CHARGING_NOT_ACTIVE = 0,   // Vehicle connected, but not drawing current (State B)
    IS17017_CHARGING_ACTIVE           // Vehicle is actively charging (State C or D)
} is17017_charging_state_t;

typedef enum
{
    IS17017_FAULT_NOT_PRESENT = 0,   // No fault
    IS17017_FAULT_PRESENT     = 1    // Fault detected (State E or internal error)
} is17017_fault_status_t;

typedef enum
{
    IS17017_VENTILATION_NOT_REQUIRED = 0,
    IS17017_VENTILATION_REQUIRED     = 1
} is17017_ventilation_required_t;


/**
 * @brief Initialize vehicle detection and determine if ventilation is required (IEC 61851 / IS 17017)
 *
 * This function monitors the pilot signal and waits for a valid vehicle state (B, C, or D)
 * within the specified timeout. It returns true if a vehicle is detected. Additionally, it
 * sets the `ventilation_required` flag to true if the pilot state indicates State D 
 * (ventilation required).
 *
 * @param[out] ventilation_required  Pointer to a bool, will be set true if vehicle requires ventilation (State D)
 * @param[in]  timeout_ms            Maximum time in milliseconds to wait for vehicle detection
 *
 * @return true  if vehicle is detected (pilot in valid state B, C, or D)
 * @return false if no vehicle is detected within the timeout or pilot is in fault/invalid state
 */
bool is17017_vehicle_initialize(bool* ventilation_required);


/**
 * @brief Check whether the EV gun is connected
 * @return true if connected
 */
bool is17017_check_gun_connection(void);

/**
 * @brief Check whether the EV gun is connected
 * @return true if connected
 */
bool is17017_check_charger_avalaibiltys(void);

/**
 * @brief Set whether the charger is available for EV
 * @param available true to make charger available
 */
void is17017_set_charger_availability(bool available);

/**
 * @brief Set the charging current in mA
 * @param current_A Charging current in amps
 */
void is17017_set_charging_current(uint8_t current_A);

/**
 * @brief Check whether EV is actively charging
 * @return true if charging is ongoing
 */
bool is17017_is_vehicle_charging(void);

/**
 * @brief Move the charger to fault mode (set pilot to 12V DC)
 */
void is17017_set_fault_mode(void);

/**
 * @brief Check whether the vehicle is in fault condition (State E)
 * @return true if fault condition is detected
 */
bool is17017_is_vehicle_in_fault_state(void);

/**
 * @brief Get the current vehicle state (State A/B/C/D/E)
 * @return IEC 61851 state code (0 = A, 1 = B, ...)
 */
is17017_state_t is17017_get_vehicle_state(void);

void is17017_state_flow(void);

#endif // is17017_API_H

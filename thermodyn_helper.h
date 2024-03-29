#ifndef __THERMODYN_HELPER_H__
#define __THERMODYN_HELPER_H__
/**
 * Interface: Thermodynamic helper
 * Purpose: Handle resource (memory alloc + delete + init) and Computations
 */

// opaque pointer
typedef struct system_t * device;


/**
 * Interface declarations:
 */

// Function return type:
typedef enum {SUCCESS, FAILURE} status;

// Type 1: Resource management functions
status init(device *gas, double volume, double pressure, double temperature);
status deinit(device gas);

// Type 2: Display functions
status print_current_state(device gas);

// Type 3: Computation functions
status update_volume_isothermal(device gas, double new_volume);
status update_pressure_isometric(device gas, double new_pressure);

#endif /* __THERMODYN_HELPER_H__ */

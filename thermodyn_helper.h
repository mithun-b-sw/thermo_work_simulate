/**
 * Interface: Thermodynamic helper
 * Purpose: Handle resource (memory alloc + delete + init) and Computations
 */

// opaque pointer
typedef struct system_t * device;

// Interface declarations
void init(device *gas, double volume, double pressure, double temperature);
void print_current_state(device gas);
void uninit(device gas);

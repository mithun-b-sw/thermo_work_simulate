/**
 * Module: Thermodynamic helper
 * Purpose: Handle resource (memory alloc + delete + init) and Computations
 */

#include <stdio.h>
#include <stdlib.h> // For: malloc() & free()
#include "thermodyn_helper.h"

/**
 * Definition of opaque pointer 'device' declared in header file
 */
struct system_t {			/* SI units are used */
	double volume;			/* liter */
	double pressure;		/* pascal (or) newton/meter square */
	double temperature;		/* kelvin */
	/* moles */
};
typedef struct system_t * device;

/**
 * Definition of interfaces whose declaration from header file starts here:
 */

// Type 1: Resource management functions

/**
 * Arguments: volume in liter, pressure in Pascal, temperature in Kelvin
 */
status init(device *gas, double volume, double pressure, double temperature) {
	*gas = (device) malloc(sizeof(struct system_t));
	(*gas)->volume = volume;
	(*gas)->pressure = pressure;
	(*gas)->temperature = temperature;
	return SUCCESS;
}

status deinit(device gas) {
	free(gas);
	return SUCCESS;
}

// Type 2: Display functions
status print_current_state(device gas) {
	printf("volume(L)\t"	"pressure(Pa)\t"	"temperature(K)\n");
	printf("%6.3lf L\t"		"%8.0lf Pa\t"			"%11.0lf K\n",
			gas->volume,	gas->pressure,		gas->temperature);
	return SUCCESS;
}

// Type 3: Computation functions

status update_pressure_isometric(device gas, double new_pressure) {
	/**
	 * Guy-Lussac's Law: P ∝ T
	 * =>  P1/T1 = P2/T2
	 * =>  P1 = T1.(P2/T2)
	 */
	double new_temperature = gas->temperature * (new_pressure / gas->pressure);
	gas->pressure = new_pressure;
	gas->temperature = new_temperature;
	return SUCCESS;
}

status update_volume_isothermal(device gas, double new_volume) {
	/**
	 * For Isothermal process, the 'polytropic index' n = 1;
	 * So the actual relation:
	 *		P.Vⁿ = Constant
	 * reduces to:
	 *		P.V = Constant
	 *
	 * Boyle's Law: P ∝ (1/V)  =>  P1.V1 = P2.V2
	 */
	double new_pressure = (gas->volume * gas->pressure) / new_volume;
	gas->volume = new_volume;
	gas->pressure = new_pressure;
	return SUCCESS;
}

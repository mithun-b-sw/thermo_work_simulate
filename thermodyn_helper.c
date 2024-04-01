/**
 * Module: Thermodynamic helper
 * Purpose: Handle resource (memory alloc + delete + init) and Computations
 */

#include <stdio.h>
#include <stdlib.h> // For: malloc() & free()
#include <math.h>   // For: fabs()
#include "thermodyn_helper.h"

/**
 * Definition of opaque pointer 'device' declared in header file
 */
struct system_t {			/* SI units are used */
	double volume;			/* liter */
	double pressure;		/* pascal (or) newton/meter square */
	double temperature;		/* kelvin */
	/* moles */
	double time;			/* second */
	double surface_area;
	double piston_mass;
	double chamber_length;
};
typedef struct system_t * device;

/**
 * Static functions declations:
 */
static double compute_time(device gas, double new_pressure, double new_volume);

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
	(*gas)->time = 0.0;
	(*gas)->surface_area = 0.04;
	(*gas)->piston_mass = 1;
	(*gas)->chamber_length = 0.25;
	return SUCCESS;
}

status deinit(device gas) {
	free(gas);
	return SUCCESS;
}

// Type 2: Display functions
status print_current_state(device gas) {
	printf("volume(L)\t" "pressure(Pa)\t" "temperature(K)\t"  "time(s)\n");
	printf("%6.3lf L\t"    "%8.0lf Pa\t"      "%11.0lf K\t"  "%.3lf s\n",
			gas->volume, gas->pressure, gas->temperature, gas->time);
	return SUCCESS;
}

// Type 3: Computation functions

status update_pressure_isometric(device gas, double new_pressure) {
	/**
	 * Gay-Lussac's Law: P ∝ T
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
	gas->time += compute_time(gas, new_pressure, new_volume);
	gas->volume = new_volume;
	gas->pressure = new_pressure;
	return SUCCESS;
}

// Type 4: Local/Static functions
double compute_time(device gas, double final_pressure, double final_volume) {
	double time = 0;
	double time_step_size = 0.001;
	double pressure = gas->pressure;
	double volume = gas->volume;
	device piston = gas;
	double chamber_length = gas->chamber_length;
	const double pressure_volume_constant = pressure * volume;

	for(;fabs(pressure-final_pressure) > 1.0;
			time += time_step_size) {
		double acceleration = ((pressure - final_pressure) * piston->surface_area)
			/ piston->piston_mass;
		/**
		 * Assuming uniform acceleration over short interval 'time_step_size'
		 */
		chamber_length += (0.5 * acceleration * time_step_size * time_step_size);
		volume = chamber_length * piston->surface_area;
		pressure = pressure_volume_constant / volume;
	}
	gas->chamber_length = chamber_length;
	return time;
}

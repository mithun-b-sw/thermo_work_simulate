/**
 * Module: Thermodynamic helper
 * Purpose: Handle resource (memory alloc + delete + init) and Computations
 */

#include <stdio.h>
#include <stdlib.h> // For: malloc() & free()
#include <math.h>   // For: fabs() & NAN
#include "thermodyn_helper.h"

#define R_GAS_CONSTANT       8.3144598    /* J/(mol.K) ; where J -> Newton.meter */
#define C_AIR_SPECIFIC_HEAT  20.85        /* J/(mol.K) ; Molar specific heat 'C' */
#define EQUILIBRIUM_PRESSURE 101325       /* Pa or pascal */

/**
 * Definition of opaque pointer 'device' declared in header file
 */
struct system_t {			/* SI units are used */
	double volume;			/* meter cube */
	double pressure;		/* pascal (or) newton per meter square */
	double temperature;		/* kelvin */
	double moles;
	double time;			/* second */
	double surface_area;
	double piston_mass;
	double chamber_length;
	double workdone;
};
typedef struct system_t * device;

/**
 * Static functions declaration:
 */
static double compute_time(device gas, double *new_pressure, double *new_volume);

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
	(*gas)->moles = (pressure * volume)/(R_GAS_CONSTANT * temperature);
	(*gas)->time = 0.0;
	(*gas)->surface_area = 0.04;
	(*gas)->piston_mass = 1;
	(*gas)->chamber_length = volume/(*gas)->surface_area;
	(*gas)->workdone = 0.0;
	return SUCCESS;
}

status deinit(device gas) {
	free(gas);
	return SUCCESS;
}

// Type 2: Display functions
status print_current_state(device gas) {
	printf("time(s)\t" "volume(m³)\t" "pressure(Pa)\t" "temperature(K)\t" "workdone(J)\n");
	print_current_state_value(gas);
	return SUCCESS;
}

status print_current_state_value(device gas) {
	printf("%.3lf\t" "%6.4lf\t" "%8.0lf\t" "%11.0lf\t" "%8.3lf\n",
			gas->time, gas->volume, gas->pressure, gas->temperature, gas->workdone);
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

status update_volume_isothermal(device gas, double) {
	/**
	 * For Isothermal process, the 'polytropic index' n = 1;
	 * So the actual relation:
	 *		P.Vⁿ = Constant
	 * reduces to:
	 *		P.V = Constant
	 *
	 * Boyle's Law: P ∝ (1/V)  =>  P1.V1 = P2.V2
	 */
	double new_pressure = EQUILIBRIUM_PRESSURE;
	double new_volume = (new_pressure * gas->volume) / gas->pressure;

	gas->time += compute_time(gas, &new_pressure, &new_volume);
	if(new_volume > gas->volume)
		gas->workdone += R_GAS_CONSTANT * gas->moles * gas->temperature * (log(new_volume/gas->volume));
	else
		gas->workdone += R_GAS_CONSTANT * gas->moles * gas->temperature * (log(gas->volume/new_volume));

	gas->volume = new_volume;
	gas->pressure = new_pressure;
	return SUCCESS;
}

// Type 4: Get device characteristics functions

double get_pressure(const device gas) {
	return gas->pressure;
}

double get_time(const device piston) {
	return piston->time;
}

double get_displacement(const device piston) {
	return piston->chamber_length;
}

// Type 5: Local/Static functions
double compute_time(device gas, double *final_pressure, double *final_volume) {
	double time_step_size = 0.0001;
	double time = time_step_size;
	double pressure = gas->pressure;
	double volume = gas->volume;
	device piston = gas;
	double chamber_length = gas->chamber_length;
	const double pressure_volume_constant = pressure * volume;
	double velocity = 0.0; // initial velocity assumed zero
	int previous_direction = 0;
	int direction = 0;
	double pressure_steps = 0.01;

	do {
		/**
		 * The actual formula for acceleration is: a = P_difference * Area
		 * Since the pressure increases over time and not instant:
		 */
		double acceleration = ((pressure - *final_pressure) * pressure_steps * piston->surface_area)
			/ piston->piston_mass;
		if(pressure_steps <= 0.99) {
			pressure_steps += 0.01;
		}
		/**
		 * Assuming uniform acceleration over short interval 'time_step_size'
		 */
		chamber_length += (velocity * time_step_size)
			+ (0.5 * acceleration * time_step_size * time_step_size);
		previous_direction = (velocity > 0.0) ? 1 : ((velocity < 0.0) ? -1 : 0);
		velocity += acceleration * time_step_size;
		direction = (velocity > 0.0) ? 1 : ((velocity < 0.0) ? -1 : 0);
		volume = chamber_length * piston->surface_area;
		pressure = pressure_volume_constant / volume;
		time += time_step_size;
	} while (0 == previous_direction || previous_direction == direction);

	gas->chamber_length = chamber_length;
	*final_volume = volume;
	*final_pressure = pressure;
	return time;
}


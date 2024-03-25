/**
 * Module: Thermodynamic helper
 * Purpose: Handle resource (memory alloc + delete + init) and Computations
 */

#include <stdio.h>
#include <stdlib.h> // For: malloc() & free()

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
 * Definition of interfaces declared in header file starts here:
 */

/**
 * Arguments: volume in liter, pressure in Pascal, temperature in Kelvin
 */
void init(device *gas, double volume, double pressure, double temperature) {
	*gas = (device) malloc(sizeof(struct system_t));
	(*gas)->volume = volume;
	(*gas)->pressure = pressure;
	(*gas)->temperature = temperature;
}

void print_current_state(device gas) {
	printf("volume(L)\tpressure(Pa)\ttemperature(K)\n");
	printf("%.3lfL\t%.0lfPa\t%.0lfK\n", gas->volume, gas->pressure, gas->temperature);
}

void uninit(device gas) {
	free(gas);
}


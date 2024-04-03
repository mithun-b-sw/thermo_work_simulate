/**
 * This is a simple test code to check the thermodyn library.
 */
#include <stdio.h>
#include <stdlib.h>
#include "thermodyn_helper.h"

#define GAS_VOLUME		0.01
#define GAS_PRESSURE	101325
#define GAS_TEMPERATURE	300

int main(int argc, char **argv) {

	int iterations = 1;
	if(argc == 2)
	{
		iterations = atoi(argv[1]);
	}
	/**
	 * The 'device' type is an opaque pointer,
	 * followed by init() and uninit() act as interface using the opaque pointer.
	 */
	device gas;
	init(&gas, GAS_VOLUME, GAS_PRESSURE, GAS_TEMPERATURE);

	printf("# At initial state\n");
	print_current_state(gas);

	for(int i = 0; i < iterations; ++i)
	{

	/**
	 * Isometric heat addition from external source
	 */
	update_pressure_isometric(gas, GAS_PRESSURE * 1.2);
	printf("# After increasing pressure for about 20%%\n");
	print_current_state(gas);

	/**
	 * Isothermal expansion by heat observed from external source
	 */
	update_volume_isothermal(gas, GAS_VOLUME * 1.2);
	printf("# After increasing volume from 0.01 m³ to 0.012 m³\n");
	print_current_state(gas);

	/**
	 * Isometric heat rejection
	 */
	update_pressure_isometric(gas, GAS_PRESSURE * 0.8334);
	printf("# After decreasing pressure below original one\n");
	print_current_state(gas);

	/**
	 * Isothermal compression by rejecting heat
	 */
	update_volume_isothermal(gas, GAS_VOLUME * 0.8334);
	printf("# After decreasing volume back from 0.012 m³ to 0.01 m³\n");
	print_current_state(gas);

	}

	deinit(gas);
	return 0;
}

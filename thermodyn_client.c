/**
 * This is a simple test code to check the thermodyn library.
 *
 * to gen csv file:
 * $ ./thermodyn_cycle 25 | grep -ve "^#" | column -t
 *
 * for right justification:
 * $ ./thermodyn_cycle 25 | grep -ve "^#" | column -t -R 1,2,3,4,5,6
 */
#include <stdio.h>
#include <stdlib.h> // atoi()
#include <math.h>   // NAN
#include "thermodyn_helper.h"

#define GAS_VOLUME		0.01
#define GAS_PRESSURE	101325
#define GAS_TEMPERATURE	300

int main(int argc, char **argv) {

	int iterations = 1;
	if(argc == 2) {
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

	double pressure = get_pressure(gas);
	double coefficient = 1.2; // 1.2 indicates 20% increase in pressure

	for(int i = 0; i < iterations; ++i) {

		double start_time = get_time(gas);
		/**
		 * Isometric heat addition from external source
		 */
		pressure = get_pressure(gas);
		update_pressure_isometric(gas, pressure * coefficient);
		printf("# After increasing pressure for about 20%%\n");
		print_current_state_value(gas);

		/**
		 * Isothermal expansion by heat observed from external source
		 */
		update_volume_isothermal(gas, NAN);
		/** TODO: NAN is added since client set volume doesn't make sense.
		 * Also library's interface change is restricted */
		printf("# After letting gas to increase its volume as per isothermal expansion\n");
		print_current_state_value(gas);
		double max_expansion = get_displacement(gas);

		/**
		 * Isometric heat rejection
		 */
		pressure = get_pressure(gas);
		update_pressure_isometric(gas, pressure / coefficient);
		printf("# After decreasing pressure below original one\n");
		print_current_state_value(gas);

		/**
		 * Isothermal compression by rejecting heat
		 */
		update_volume_isothermal(gas, NAN);
		printf("# After letting gas to decrease its volume as per isothermal contraction\n");
		print_current_state_value(gas);
		double end_time = get_time(gas);
		double max_compression = get_displacement(gas);
		printf("# Rotational Speed = %.3lf rpm and piston displacement = %.3f m at cycle = %d \n",
			60/(end_time - start_time), max_expansion - max_compression, i+1);
	}

	deinit(gas);
	return 0;
}

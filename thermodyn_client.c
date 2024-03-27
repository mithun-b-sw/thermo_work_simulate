/**
 * This is a simple test code to check the thermodyn library.
 */
#include <stdio.h>
#include "thermodyn_helper.h"

#define GAS_VOLUME		10
#define GAS_PRESSURE	101325
#define GAS_TEMPERATURE	300

int main() {
	/**
	 * The 'device' type is an opaque pointer,
	 * followed by init() and uninit() act as interface using the opaque pointer.
	 */
	device gas;
	init(&gas, GAS_VOLUME, GAS_PRESSURE, GAS_TEMPERATURE);

	printf("# At initial state\n");
	print_current_state(gas);

	/**
	 * Isometric heat addition from external source
	 */
	update_pressure_isometric(gas, GAS_PRESSURE * 1.2);
	printf("# After increasing pressure for about 20%%\n");
	print_current_state(gas);

	/**
	 * Isothermal expansion by heating from external source
	 */
	update_volume_isothermal(gas, GAS_VOLUME * 1.2);
	printf("# After increasing volume from 0.1 m³ to 0.12 m³\n");
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
	update_volume_isothermal(gas, GAS_VOLUME);
	printf("# After decreasing volume back from 0.12 m³ to 0.1 m³\n");
	print_current_state(gas);

	deinit(gas);
	return 0;
}

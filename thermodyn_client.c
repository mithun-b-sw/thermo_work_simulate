/**
 * This is a simple test code to check the thermodyn library.
 */
#include "thermodyn_helper.h"

int main() {
	/**
	 * The 'device' type is an opaque pointer,
	 * followed by init() and uninit() act as interface using the opaque pointer.
	 */
	device gas;
	init(&gas, 10, 101325, 300);

	print_current_state(gas);
	/**
	 * Since we are still at staring stage, no calculation part added yet.
	 */

	uninit(gas);
	return 0;
}

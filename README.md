# Simulate Thermo-dynamic cycle

Here the simulation is based on Quasi-equilibrium Processes.

#### Dependency

1. Currently tested on Linux based OS.
2. Needs 'make' and 'gcc' commands to compile (sudo apt-get install build-essential).

#### usage

	$ make
	gcc thermodyn_client.c -c -I./ -o thermodyn_client.o
	gcc thermodyn_helper.c -c -I./ -o thermodyn_helper.o
	gcc thermodyn_client.o thermodyn_helper.o -I./  -o thermodyn_cycle
	
	$ ./thermodyn_cycle | column -t
	volume(L)  pressure(Pa)  temperature(K)
	10.000L    101325Pa      300K

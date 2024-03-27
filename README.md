# Simulate Thermo-dynamic cycle

Here the simulation is based on Quasi-equilibrium Processes.

### Design aspects

This program can be understood considering two parts:

1. First one is the library, which has an interface and a definition.
2. Second part is the client code, which help in knowing the usage of library.

#### More on library part

The interface is developed with an objective to never change (but free to extended).
Keeping this in mind, the data structure in use should be hidden from client code too.
One way to hide internal data structure, is using opaque pointer at the interface.
So an opaque pointer named 'device' is declared in header file: 'thermodyn_client.h'.
Whereas definition of 'device' is placed in 'thermodyn_client.c'.

Are there any more advantages of such design?
Yes, opaque pointers allow to extend the internal data structure and client part doesn't know.
The functions exposed to client (interface) also doesn't need to change.

Basically if client code is compiled,
it doesn't need to compile again to link with the changes done only on library part.

### What simulations are attempted?

The client code containing a main() function described here.

#### Initialization part

An opaque pointer named 'device' started with 10L of air at 300K temperature (assumed values).
Now this initial state will exert 101.325 kPa pressure if not external pressure applied.
The pressure value is taken from standard temperature and pressure STP standards with SI units.
This initial state is set by calling init() function which is part of library.

#### Simulation part

Now there are two operations performed on this device:

1. Pressure increase or decrease holding isometric state.
2. Volume increase or decrease holding isothermal state.

The above two operations can be used to implement all four cycles of ideal Stirling engine.
These operations are converted to functions in the library and are called from client code.
A combinations of these functions makes complete thermodynamic cycle from client side.
The change in pressure, volume and temperature can be know after an operation complete.
A operation completes once an equilibrium is achieved.
The post equilibrium values of pressure, volume and temperature are based on:

1. 'Gay-Lussac's pressure-temperature law' (at constant volume or isometric process)
2. 'Boyle's law' (at constant temperature or isothermal process)

#### Resource deinitialization part

Every client code must end by calling deinit() which frees the resources allocated.

### Build and test

#### Dependency

1. Currently tested on Linux based OS.
2. Needs 'make' and 'gcc' commands to compile (sudo apt-get install build-essential).

#### usage

	$ make
	gcc thermodyn_client.c -c -I./ -o thermodyn_client.o
	gcc thermodyn_helper.c -c -I./ -o thermodyn_helper.o
	gcc thermodyn_client.o thermodyn_helper.o -I./  -o thermodyn_cycle
	
	./thermodyn_cycle
	# At initial state
	volume(L)	pressure(Pa)	temperature(K)
	10.000 L	  101325 Pa	        300 K
	# After increasing pressure for about 20%
	volume(L)	pressure(Pa)	temperature(K)
	10.000 L	  121590 Pa	        360 K
	# After increasing volume from 0.1 m³ to 0.12 m³
	volume(L)	pressure(Pa)	temperature(K)
	12.000 L	  101325 Pa	        360 K
	# After decreasing pressure below original one
	volume(L)	pressure(Pa)	temperature(K)
	12.000 L	   84444 Pa	        300 K
	# After decreasing volume back from 0.12 m³ to 0.1 m³
	volume(L)	pressure(Pa)	temperature(K)
	10.000 L	  101333 Pa	        300 K


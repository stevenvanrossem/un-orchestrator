# Name-resolver

The name-resolver is a module that provides information for all the possible
implementations for a network function.

## Required libraries

Several libraries are required to compile the name-resolver.
In the following we list the steps required on an **Ubuntu 14.04**.

	; Install required libraries
	; - build-essential: it includes GCC, basic libraries, etc
	; - cmake: to create cross-platform makefiles
	; - cmake-curses-gui: nice 'gui' to edit cmake files
	; - libboost-all-dev: nice c++ library with tons of useful functions
	; - libmicrohttpd-dev: embedded micro http server
	; - libxml2-dev: nice library to parse and create xml
	$ sudo apt-get install build-essential cmake cmake-curses-gui libboost-all-dev libmicrohttpd-dev libxml2-dev
	
	; Install JSON Spirit (nice library to parse JSON files)
	$ git clone https://github.com/sirikata/json-spirit
	; alternatively, a copy of JSON Spirit is provided in [un-orchestrator]/contrib/json-spirit.zip
	$ cd json-spirit/

## Compile the name-resolver

We are now ready to compile the name-resolver.

	$ cd name-resolver

	; Choose among possible compilation options
	$ ccmake .

The previous command allows you to select some configuration parameters for the
name-resolver, such as the logging lever. When you're finished, exit from the
`ccmake` interface by *generating the configuration files* (press 'c' and 'g')
and type the following commands:

	; Create makefile scripts based on the previously selected options
	$ cmake .

	; Compile and create the executable
	$ make

# How to run the name-resolver

The full list of command line parameters for the name-resolver can be
retrieved by the following command:

    $ sudo ./name-resolver --h

Please refer to the help provided by the name-resolver itself in order to
understand how to use the different options.

Please check `config/example.xml` to understand the configuration file required by
the name-resolver. This file represents a database containing information on all
the possible implementations for each available network function.

# read_haloes

Compile with "make" (makefile in folder)
Remove object files and read executable with "make clean"

————————————————————————————————————————————————————————————————————————

Usage: ./read <options> filename label

Default option simply reads file and prints to screen quantities.
Command line options are:

 -n	Create NDfield file
 -c	Create column data text file
 -d	Create density plots
 -r	Create mass-radius plots
 -i	Calculate moment of inertia tensor

————————————————————————————————————————————————————————————————————————


TO DO:
	- Fix command line arguments (which represent files, which don't)
	- Remove global NDfield structure, pass to functions (?)
	- Extend to dimensions other than 3 (get rid of nx,ny,nz)
	- Save density plots to file in function
	- Write in usage ./read <flags> filename abbreviation
	- Expand density_plots function to do xz and yz plots also
	- Add in flag dependence for CoM and GRID calculation in main

	- Add in bin size option in mass radius plots (command line argument)

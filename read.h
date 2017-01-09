// Libraries:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <getopt.h>


// Globals:

#define NDFIELD_MAX_DIMS 20
#define NDFIELD_TAG "NDFIELD"
#define OPTLIST "ncdri"


// NDField structure to hold data:

typedef struct NDfield_str 
{
	char comment[80];
	int dims[NDFIELD_MAX_DIMS];
	int ndims;
	int n_dims;
	int fdims_index;
	int datatype;
	double x0[NDFIELD_MAX_DIMS];
	double delta[NDFIELD_MAX_DIMS];
	char dummy[160];
	float *val;

	long nval;
	int datasize;
} NDfield;

extern NDfield *field;		
	/* Define global structure 'field' used as a reference for most functions */


//// PARSING COMMAND LINE ARGUMENTS ////
// Functions are in args.c 

extern bool nflag,cflag,dflag,rflag,iflag,errflag;
extern char *usage;

extern void	flags(int,char **);
	/* Read in command line arguments, set boolean flag values true or false */


//// READING GRID FROM FORTRAN FILE ////
// Functions are in readfile.c 

extern int 	nx,ny,nz;

extern void	read_header(FILE *);		
	/* Read in values from head of fortran file, ignoring size wrappers, which 
	 * inform the number of dimensions, and allows dimensions to be read in */
extern void	read_grid(FILE *);		
	/* Reads in unformatted binary grid from fortran file to field->val and
	 * checks that the number of vals is consistent */


//// READING GRID INTO AN NDFIELD FORMAT ////
// Functions are in createNDfield.c

extern void	create_NDstruct(int,int,double *,double *);	
	/* Fills in NDfield structure with values not given in fortran file -
	 * pass values for fdims_index, datatype, x0 and delta in that order */
extern int	save_NDfield(const char *);	
	/* Saves NDfield structure to file with extension .ND according to format
	 * specified in disperse */


//// PRODUCE COLUMN DATA FOR COLUMNS ////
// Functions are in plotting.c 

extern float	***create_grid(void);		
	/* Creates 3D grid from list of vals */
extern void	column_data(char *);		
	/* Output grid values to column txt file in the format
	 * 		x	y	z	field->val	log(val)	
	 * with a new line after each new x value (for gnuplot) */
extern void	xy_data(char *,float ***);
	/* Sums up all grid values for different z for a given (x,y) to produce 
	 * a 3D grid and outputs to filename specified */
extern void	xz_data(char *,float ***);
	/* Sums up all grid values for different y for a given (x,z) to produce 
	 * a 3D grid and outputs to filename specified */
extern void	yz_data(char *,float ***);
	/* Sums up all grid values for different x for a given (y,z) to produce 
	 * a 3D grid and outputs to filename specified */

//// DENSITY PLOTS ////
// Functions are in plotting.c

extern void	density_plots(char *,char *,char *,char *);		
	/* Plot densities using column data outputed to file, with map view 
	 * using GNUplot commands, for both normal and log scales */


//// MASS RADIUS PLOTS ////
// Functions are in plotting.c

extern void	mass_radius_plots(float ***,float *,char *,char *);	
	/* Creates histogram for mass-radius data */
extern void	bin_histogram(float ***,float,float,float);



//// MOMENT OF INERTIA TENSOR CALCULATIONS ////
// Functions are in momentofinertia.c

extern double 	*moment_of_inertia(float ***,float *);
	/* Returns the elements of the moment of inertia tensor in the form
	 *	I11, I22, I33, I21, I31, I32
	 * */

extern double 	*eigenvalues(double *);
	/* Returns the 3 eigenvlaues of the simplified moment of inertia matrix */


//// MATHS FUNCTIONS ////
// Functions are in functions.c

extern float	*centre_of_mass(float ***);		
	/* Determines the centre of mass of halo, returns as a float array 
	 * the size of ndims */

extern float	radcentre(int,int,int,float,float,float);	
	/* Small function which returns the distance of a point from CoM */





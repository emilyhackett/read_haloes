#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


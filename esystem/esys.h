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

#define LONG	1

extern int max_radius;

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

//// READING GRID FROM FORTRAN FILE ////
// Functions are in readfile.c 

extern int 	nx,ny,nz;

extern void	read_header(FILE *);		
	/* Read in values from head of fortran file, ignoring size wrappers, which 
	 * inform the number of dimensions, and allows dimensions to be read in */
extern void	read_grid(FILE *);		
	/* Reads in unformatted binary grid from fortran file to field->val and
	 * checks that the number of vals is consistent */
extern void	create_NDstruct(int,int,double *,double *);	
	/* Fills in NDfield structure with values not given in fortran file -
	 * pass values for fdims_index, datatype, x0 and delta in that order */


//// MOMENT OF INERTIA TENSOR CALCULATIONS ////
// Functions are in momentofinertia.c

extern double 	*moment_of_inertia(float ***,float *,int);
	/* Returns the elements of the moment of inertia tensor in the form
	 *	I11, I22, I33, I21, I31, I32	*/
extern double	*reduced_inertia(float ***,float *,int);
	/* Returns the elements of the reduced moment of inertia tensor in
	 * the style above	*/
extern double	*evalue_characteristics(double *);
	/* Prints out characteristics of the moment of inertia tensor evalues
	 * such as sphericity and triaxiality */

//// PLOTTING FUNCTIONS ////
// Functions are in plotting.c
extern void	plot_evalues(char *,char *);
	/* Plots radius v. shape characteristics */
extern void	plot_ellipses(char *,char *,char *,float *,float ***);
	/* Plots ellipse for moment of inertia tensor at given max_radius 
	 * on top of density plot */


//// MATHS FUNCTIONS ////
// Functions are in functions.c

extern float	***create_grid(void);		
	/* Creates 3D grid from list of vals */
extern float	*centre_of_mass(float ***);		
	/* Determines the centre of mass of halo, returns as a float array 
	 * the size of ndims */
extern float	radcentre(int,int,int,float,float,float);	
	/* Small function which returns the distance of a point from CoM */
extern int	cmpfunc(const void *,const void *);
	/* Function that compares to values to do a sort */
extern double 	*eigensystem(double *);
	/* Hopefully the finalised method of calculating the eigenvalues and the
	 * corresponding eigenvectors for a given moment of inertia tensor */
extern float	axis_angle(double *esys);
	/* Finds angle between major axis and x-axis */

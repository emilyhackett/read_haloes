#include "read.h"

// Initialising declared variables
NDfield *field;
int nx,ny,nz;
int radius;
float min,max;

// Initialising command line argument structures
bool nflag,cflag,dflag,rflag,iflag,errflag	= false;
char *usage =	"Usage: ./read <options> file_label z_min z_max\n\nDefault option simply reads file and prints to screen quantities.\nCommand line options are:\n\n -n	Create NDfield file\n -c	Create column data text file\n -d	Create density plots\n -r	Create mass-radius plots\n -i	Calculate moment of inertia tensor\n";


//////////////////////////	MAIN: READING FILE FROM ARGS	//////////////////////////
int main(int argc, char *argv[])
{
	// Check command line arguments
	if(argc<4)
	{
		fprintf(stderr,"%s\n", usage);
		exit(1);
    	}

	min=atof(argv[3]);
	max=atof(argv[4]);

	// FILE NAMES:
	char name[20];
	sprintf(name,"%s",argv[argc-3]);
	char datfile[100],NDfile[100],plotfile[100];
	sprintf(datfile,"%s.dat",name);
	char colfile[100],colxy[100],colxz[100],colyz[100];

	// Read in command line argument flags
	flags(argc,argv);
	if(iflag) printf("Radius read in as %i\n",radius);

	// Open file passed in command line arguments
	if(LONG)	printf("	---- READING FROM FORTRAN FILE ----\n");
	FILE *fp;
	fp = fopen(datfile,"r");
	if(LONG)	printf("File opened is %s\n",datfile);

	// Allocate memory for NDfield structure
	field=calloc(1,sizeof(NDfield));
	if(field == NULL)
	{
		printf("Error - NDfield calloc request failed\n");
		exit(EXIT_FAILURE);
	}
	
	// Read values from header in
	read_header(fp);

	// Populate NDfield structure with values
	if(LONG)	printf("	---- CREATING NDFIELD STRUCTURE ----\n");
	create_NDstruct(0,(1<<8),field->x0,field->delta);

	if(LONG)	printf("	---- READING GRID INTO NDFIELD STRUCTURE ----\n");
	read_grid(fp);

	// File names to save into:
	char *MASSFILE=malloc(20);
	char *MASSPLOTFILE=malloc(20);

	if(LONG)	printf("	---- CALCULATING CENTRE OF MASS AND GRID ----\n");
	// Calculate centre of mass and GRID regardless of flags
	float ***GRID=create_grid();
	float *CoM=malloc(sizeof(float)*field->ndims);
	CoM=centre_of_mass(GRID);

	// Save to NDfield file format with .ND extension
	if(nflag) {
		printf("	---- SAVING NDFIELD STRUCTURE TO FILE ----\n");
		sprintf(NDfile,"NDfiles/%s.ND",name);
		save_NDfield(NDfile);
	}

	// Save to column data file format with .dat extension
	if(cflag) {
		printf("	---- WRITING GRID TO COLUMN DATA ----\n");
//		sprintf(colfile,"%s-grid.dat",name);
//		column_data(colfile);
		sprintf(colxy,"%s_%.2f_%.2f-xygrid.dat",name,min,max);
		xy_data(colxy,GRID,min,max);
//		sprintf(colxz,"%s-xzgrid.dat",name);
//		xz_data(colxz,GRID);
//		sprintf(colyz,"%s-yzgrid.dat",name);
//		yz_data(colyz,GRID);	
	}

	// Create density plots and save to file
	if(dflag) {
		printf("	---- CREATING AND SAVING DENSITY PLOTS ----\n");
		sprintf(plotfile,"%s_%.2f_%.2f-denplots.ps",name,min,max);
		density_plots(plotfile,colxy,colxz,colyz,min,max);
	}

	// Create mass-radius histogram plots
	if(rflag)
	{
		printf("	---- PRODUCING MASS-RADIUS PLOTS ----\n");
		sprintf(MASSFILE,"%s-massradius.dat",argv[argc-1]);
		sprintf(MASSPLOTFILE,"%s-massradiusplots.ps",argv[argc-1]);
		mass_radius_plots(GRID,CoM,MASSFILE,MASSPLOTFILE);
	}

	if(iflag)
	{
		printf("	---- CALCULATING MOMENT OF INERTIA TENSOR ----\n");
		double *matrix=malloc(6*sizeof(double));
		matrix=moment_of_inertia(GRID,CoM);	
		double *evalues=malloc(field->ndims*sizeof(double));
		evalues=eigenvalues(matrix);
		evalue_characteristics(evalues);

		printf("	---- NEW EIGENVALUE CALCULATION TEST ----\n");
		
		double *eval2=malloc(4*field->ndims*sizeof(double));
		eval2=eigensystem(matrix);
		evalue_characteristics(eval2);
	}

	printf("	---- END OF PROGRAM REACHED ----\n\n");
}

//// END ////

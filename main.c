#include "read.h"

// Initialising declared variables
NDfield *field;
int nx,ny,nz;

// Initialising command line argument structures
bool nflag,cflag,dflag,rflag,iflag,errflag	= false;
char *usage =	"Usage: ./read <options> filename label\n\nDefault option simply reads file and prints to screen quantities.\nCommand line options are:\n\n -n	Create NDfield file\n -c	Create column data text file\n -d	Create density plots\n -r	Create mass-radius plots\n -i	Calculate moment of inertia tensor\n";


//////////////////////////	MAIN: READING FILE FROM ARGS	//////////////////////////
int main(int argc, char *argv[])
{
	// Check command line arguments
	if(argc<3)
	{
		fprintf(stderr,"%s\n", usage);
		exit(1);
    	}

	// Read in command line argument flags
	flags(argc,argv);

	// Open file passed in command line arguments
	printf("\n	---- READING FROM FORTRAN FILE ----\n");
	FILE *fp;
	fp = fopen(argv[argc-2],"r");
	printf("File opened is %s\n",argv[argc-2]);

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
	printf("\n	---- CREATING NDFIELD STRUCTURE ----\n");
	create_NDstruct(0,(1<<8),field->x0,field->delta);

	printf("\n	---- READING GRID INTO NDFIELD STRUCTURE ----\n");
	read_grid(fp);

	// File names to save into:
	char *NDFILE=malloc(20);
	char *COLFILE=malloc(20);
	char *xyCOLFILE=malloc(20);
	char *xzCOLFILE=malloc(20);
	char *yzCOLFILE=malloc(20);
	char *DENFILE=malloc(20);
	char *MASSFILE=malloc(20);
	char *MASSPLOTFILE=malloc(20);

	printf("\n	---- CALCULATING CENTRE OF MASS AND GRID ----\n");
	// Calculate centre of mass and GRID regardless of flags
	float ***GRID=create_grid();
	float *CoM=malloc(sizeof(float)*field->ndims);
	CoM=centre_of_mass(GRID);

	// Save to NDfield file format with .ND extension
	if(nflag) {
		printf("\n	---- SAVING NDFIELD STRUCTURE TO FILE ----\n");
		sprintf(NDFILE,"%s.ND",argv[argc-1]);
		save_NDfield(NDFILE);
	}

	// Save to column data file format with .dat extension
	if(cflag) {
		printf("\n	---- WRITING GRID TO COLUMN DATA ----\n");
//		sprintf(COLFILE,"%s-grid.dat",argv[argc-1]);
//		column_data(COLFILE);
		sprintf(xyCOLFILE,"%s-xygrid.dat",argv[argc-1]);
		xy_data(xyCOLFILE,GRID);
		sprintf(xzCOLFILE,"%s-xzgrid.dat",argv[argc-1]);
		xz_data(xzCOLFILE,GRID);
		sprintf(yzCOLFILE,"%s-yzgrid.dat",argv[argc-1]);
		yz_data(yzCOLFILE,GRID);	
	}

	// Create density plots and save to file
	if(dflag) {
		printf("\n	---- CREATING AND SAVING DENSITY PLOTS ----\n");
		sprintf(DENFILE,"%s-denplots.ps",argv[argc-1]);
		density_plots(DENFILE,xyCOLFILE,xzCOLFILE,yzCOLFILE);
	}

	// Create mass-radius histogram plots
	if(rflag)
	{
		printf("\n	---- PRODUCING MASS-RADIUS PLOTS ----\n");
		sprintf(MASSFILE,"%s-massradius.dat",argv[argc-1]);
		sprintf(MASSPLOTFILE,"%s-massradiusplots.ps",argv[argc-1]);
		mass_radius_plots(GRID,CoM,MASSFILE,MASSPLOTFILE);
	}

	if(iflag)
	{
		printf("\n	---- CALCULATING MOMENT OF INERTIA TENSOR ----\n");
		double *matrix=malloc(6*sizeof(double));
		matrix=moment_of_inertia(GRID,CoM);	
		double *evalues=malloc(field->ndims*sizeof(double));
		evalues=eigenvalues(matrix);
	}

	printf("\n	---- END OF PROGRAM REACHED ----\n\n");
}

//// END ////

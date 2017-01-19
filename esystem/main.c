#include "esys.h"

#define	RADIUSPLOTS	0
#define ELLIPSEPLOTS	1

// Initialising declared variables
NDfield *field;
int nx,ny,nz;
int max_radius;

/*	This function will take an unstructured 3D grid as an argument
 *	and read it into the NDfield structure.
 *	From here the moment of inertia tensor is calculated in increments
 *	of 1 in the specified radius range (i.e. up to the maximum radius
 *	specified in the command line arguments.
 */

//////////////////////////	MAIN: READING FILE FROM ARGS	//////////////////////////
int main(int argc, char *argv[])
{
	// Read in command line arguments
	if(argc<3)
	{
		fprintf(stderr,"Usage: ./esys max_radius FILE file_extension\n");
		exit(1);
    	}
	if(argc<4)
	{
		if(LONG) printf("No maximum radius specified - will use entire box\n");
	}
	else {
		max_radius=atoi(argv[1]);
		if(LONG) printf("Maximum radius read in as %i\n",max_radius);
	}

	// Open file passed in command line arguments
	if(LONG)	printf("\n	---- READING FROM FORTRAN FILE ----\n");
	
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
	if(LONG)	printf("\n	---- CREATING NDFIELD STRUCTURE ----\n");
	create_NDstruct(0,(1<<8),field->x0,field->delta);

	if(LONG)	printf("\n	---- READING GRID INTO NDFIELD STRUCTURE ----\n");
	read_grid(fp);

	if(LONG)	printf("\n	---- CALCULATING CENTRE OF MASS AND GRID ----\n");
	// Calculate centre of mass and GRID regardless of flags
	float ***GRID=create_grid();
	float *CoM=malloc(sizeof(float)*field->ndims);
	CoM=centre_of_mass(GRID);

	if(LONG)	printf("\n	---- CALCULATING MOMENT OF INERTIA TENSOR ----\n");
	
	// File name to save moment of inertia data into:
	char *OUTFILE=malloc(20);
	sprintf(OUTFILE,"%s-esys_%i.dat",argv[argc-1],max_radius);
	char *PLOTFILE=malloc(20);
	sprintf(PLOTFILE,"%s-shape_%i.dat",argv[argc-1],max_radius);

	if(LONG)	printf("\n	---- SAVING SHAPE CHARACTERISTICS TO FILE ----\n");
	FILE *fout;
	fout=fopen(OUTFILE,"w");
	if(fout==NULL)
	{
		printf("Error opening outfile %s\n",OUTFILE);
		exit(EXIT_FAILURE);
	}

	FILE *fplot;
	fplot=fopen(PLOTFILE,"w");
	if(fplot==NULL)
	{
		printf("Error opening plotfile %s\n",PLOTFILE);
		exit(EXIT_FAILURE);
	}

	int radius;
	for(radius=max_radius;radius<=max_radius;radius++)
	{
		fprintf(fout,"%i\n",radius);
		fprintf(fplot,"%i	",radius);
		double *i=malloc(6*sizeof(double));
		double *ired=malloc(6*sizeof(double));
		i=moment_of_inertia(GRID,CoM,radius);	
		ired=reduced_inertia(GRID,CoM,radius);
		fprintf(fout,"%.2f	%.2f	%.2f\n%.2f	%.2f	%.2f\n%.2f	%.2f	%.2f\n",
				i[0],i[3],i[4],
				i[3],i[1],i[5],
				i[4],i[5],i[2]);
		double *esys=malloc(4*field->ndims*sizeof(double));
		double *esys_reduced=malloc(4*field->ndims*sizeof(double));
		esys=eigensystem(i);
		esys_reduced=eigensystem(ired);
		fprintf(fout,"%.2f	%.2f	%.2f\n%.2f	%.2f	%.2f\n%.2f	%.2f	%.2f\n%.2f	%.2f	%.2f\n",
				esys[0],esys[1],esys[2],
				esys[3],esys[4],esys[5],
				esys[6],esys[7],esys[8],
				esys[9],esys[10],esys[11]);
		fprintf(fplot,"%.2f	%.2f	%.f	",esys[0],esys[1],esys[2]);
		double *shape=malloc(5*sizeof(double));
		double *shape_reduced=malloc(5*sizeof(double));
		shape=evalue_characteristics(esys);
		shape_reduced=evalue_characteristics(esys_reduced);
		fprintf(fout,"%.2f\n%.2f\n%.2f	%.2f	%.2f\n",
				shape[0],shape[1],
				shape[2],shape[3],shape[4]);
		fprintf(fplot,"%.2f	%.2f	%.2f	%.2f	%.2f\n",
				shape[0],shape[1],shape[2],shape[3],shape[4]);
	}

	fclose(fout);
	fclose(fplot);

	printf("Data written to file %s\n",OUTFILE);
	printf("Data for plotting written to file %s in format:\n",PLOTFILE);
	printf(" radius	e1	e2	e3	S	T	E1	E2	E3\n");

	if(RADIUSPLOTS) {
		if(LONG)	printf("\n	---- CREATING SHAPE V. RADIUS PLOTS ----\n");
	
		char *PLOTS=malloc(sizeof(char)*100);
		sprintf(PLOTS,"%s_%i-radiusplots.ps",argv[argc-1],max_radius);
		plot_evalues(PLOTFILE,PLOTS);
	}

	if(ELLIPSEPLOTS) {
		if(LONG)	printf("\n	---- CREATING ELLIPSE-DENSITY PLOTS ----\n");
	
		char *PLOTS=malloc(sizeof(char)*100);
		sprintf(PLOTS,"%s_%i-ellipseplots.ps",argv[argc-1],max_radius);
		char *DENFILE=malloc(sizeof(char)*100);
		sprintf(DENFILE,"%s_0.00_1.00-xygrid.dat",argv[argc-1]);
		//plot_ellipses(PLOTFILE,PLOTS,DENFILE);	
	}

	if(LONG)	printf("\n	---- END OF PROGRAM REACHED ----\n\n");
}

//// END ////

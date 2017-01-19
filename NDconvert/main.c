#include "makeNDfile.h"

NDfield *field;
int nx,ny,nz;

/*	Function of this program is to convert unstructured fortran grids into ND field 
 *	files - will do nothing else
 */

int main(int argc, char *argv[])
{
	// Check command line arguments
	if(argc<3)
	{
		fprintf(stderr,"Error with command line arguments\n");
		exit(1);
    	}

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

	printf("\n	---- SAVING NDFIELD STRUCTURE TO FILE ----\n");
	char *NDFILE=malloc(20);
	sprintf(NDFILE,"%s.ND",argv[argc-1]);
	save_NDfield(NDFILE);

	printf("\n	---- END OF PROGRAM REACHED ----\n\n");

	return 0;
}


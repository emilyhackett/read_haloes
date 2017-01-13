#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NDFIELD_MAX_DIMS 20
#define NDFIELD_TAG "NDFIELD"
#define CHECK 0

//// NDFIELD DESCRIBED IN NDFIELD.H IN DISPERSE ////
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

NDfield *field;		/* Define global structure 'field' */

//// GLOBALS ////
int 	nx,ny,nz;


//////////////////////////	READ FROM ND FILE	//////////////////////////

void readfile(FILE *f)
{
	field=calloc(1,sizeof(NDfield));
	if(field == NULL)
	{
		printf("Error - NDfield calloc request failed\n");
		exit(EXIT_FAILURE);
	}


	// Space to hold dummy characters
	int i;
	
/*1*/	fread(&i,sizeof(int),1,f);
	char *tag=malloc(16);
/*2*/	fread(tag,sizeof(char),16,f);
	printf(" tag		= %s\n",tag);
/*3*/	fread(&i,sizeof(int),1,f);

/*4*/	fread(&i,sizeof(int),1,f);
	printf(" i		= %i\n",i);
	
/*5*/	fread(field->comment,sizeof(char),80,f);
	printf(" comment	= %s\n",field->comment);

/*6*/	fread(&field->ndims,sizeof(int),1,f);
	printf(" ndims		= %i\n",field->ndims);

/*7*/	fread(field->dims,sizeof(int),field->ndims,f);
	for(int i=0;i<field->ndims;i++)
	{
		printf(" dims[%i]	= %i\n",i+1,field->dims[i]);
	}

	int extra=NDFIELD_MAX_DIMS-field->ndims;
	printf("  * extra	= %i\n",extra);

/*8*/	if (field->ndims<NDFIELD_MAX_DIMS) fread(field->dummy,sizeof(int),extra,f);
	
/*9*/	fread(&field->fdims_index,sizeof(int),1,f);
	printf(" fdims_index	= %i\n",field->fdims_index);

/*10*/	fread(&field->datatype,sizeof(int),1,f);
	printf(" datatype	= %i\n",field->datatype);

/*11*/	fread(field->x0,sizeof(double),field->ndims,f);
	for (int i=0;i<field->ndims;i++)
	{
		printf(" x0[%i]		= %f\n",i,field->x0[i]);
	}
	
/*12*/	fread(field->dummy,sizeof(double),extra,f);

/*13*/	fread(field->delta,sizeof(double),field->ndims,f);
	for (int i=0;i<field->ndims;i++)
	{
		printf(" delta[%i]	= %f\n",i,field->delta[i]);
	}
	
/*14*/	fread(field->dummy,sizeof(double),extra,f);	
/*15*/	fread(field->dummy,sizeof(char),160,f);	
/*16*/	fread(&i,sizeof(int),1,f);

/*17*/	fread(&i,sizeof(int),1,f);
	field->nval=i/sizeof(float);
	printf(" nval		= %li\n",field->nval);
	
	field->val=calloc(1,field->nval*sizeof(float));
/*18*/	fread(field->val,sizeof(float),field->nval,f);

/*19*/	fread(&i,sizeof(int),1,f);
	
	printf(" FINISHED READING \n");
}


//////////////////////////	MAIN: READING FILE FROM ARGS	//////////////////////////	

int main(int argc, char *argv[])
{
	printf("\n	---- READING SPECS FROM NDFIELD FILE ----\n");	
	FILE *f;

	if(argc<2)
	{
		f = fopen("FILEIN","r");
		printf("Opened default file FILEIN\n");
	}
	else {
		f = fopen(argv[1],"r");
		printf("File opened is %s\n",argv[1]);
	}
	
	fseek(f,0,SEEK_END);
	size_t fsize=ftell(f);
	fseek(f,SEEK_SET,0);
	
	printf("File size is %lu\n",fsize);

	printf("\n");	
	readfile(f);

	fclose(f);	

	printf("\n	---- END OF PROGRAM REACHED ----\n");
}


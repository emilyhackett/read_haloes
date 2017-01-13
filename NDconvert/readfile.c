#include "makeNDfile.h"

//////////////////////////	READING NX,NY,NZ FROM FORTRAN	//////////////////////////

void read_header(FILE *fp)
{
	int length = 0;
	int *len = &length;
	int *buffer;
	
	fread(len,4,1,fp);
	field->ndims=length/4;
	printf("field->ndims	= %i\n",field->ndims);
	
	buffer = malloc(field->ndims+1);
	fread(buffer,4,field->ndims+1,fp);
	
	if(buffer[field->ndims]!=length)
	{
		printf("Error reading fortran file\n");
		exit(EXIT_FAILURE);
	}
	
//	Change this line in order to feed to delaunay_3D
//	field->dims[0]=field->ndims;

	for(int i=0;i<field->ndims;i++)
	{
		field->dims[i]=buffer[i];
		printf("field->dims[%i]	= %i\n",i,field->dims[i]);
	}
}


//////////////////////////	READING GRID VALUES FROM FORTRAN	//////////////////////////

void read_grid(FILE *fp)
{
	int length = 0;
	int *len = &length;
	
	fread(len,4,1,fp);
	printf("Number of grid points is %i\n",length/4);

	field->val=calloc(field->nval,field->datasize);
	if(field->val==NULL)
	{
		printf("Error requesting grid memory\n");
		exit(EXIT_FAILURE);
	}

	int sizeread=fread(field->val,4,length/4,fp);
	if(4*sizeread != length)
	{
		printf("Error reading file - whole grid not read\n");
	}
}

//// END ////

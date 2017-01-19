#include "esys.h"

//////////////////////////	READING NX,NY,NZ FROM FORTRAN	//////////////////////////

void read_header(FILE *fp)
{
	int length = 0;
	int *len = &length;
	int *buffer;
	
	fread(len,4,1,fp);
	field->ndims=length/4;
	if(LONG)	printf("field->ndims	= %i\n",field->ndims);
	
	buffer = malloc(field->ndims+1);
	fread(buffer,4,field->ndims+1,fp);
	
	if(buffer[field->ndims]!=length)
	{
		printf("Error reading fortran file\n");
		exit(EXIT_FAILURE);
	}
	
//	Change this line in order to feed to delaunay_3D
//	field->dims[0]=field->ndims;
	int i;
	for(i=0;i<field->ndims;i++)
	{
		field->dims[i]=buffer[i];
		printf("field->dims[%i]	= %i,	",i,field->dims[i]);
	}
	printf("\n");
}


//////////////////////////	READING GRID VALUES FROM FORTRAN	//////////////////////////

void read_grid(FILE *fp)
{
	int length = 0;
	int *len = &length;
	
	fread(len,4,1,fp);
	if(LONG)	printf("Number of grid points is %i\n",length/4);

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

//////////////////////////	READING STRUCTURE TO NDFIELD STRUCT	//////////////////////////
void create_NDstruct(int fdims_index,int datatype,double *x0,double *delta)
{
	int i;

	// fdims_index (0 for grid, 1 for particle positions)
	field->fdims_index=fdims_index;
	if(LONG)	printf("fdims_index	= %i\n",field->fdims_index);
		// n_dims determined by fdims_index (see NDfield.c)
	field->nval=1;
	if(!field->fdims_index)
	{
		field->n_dims=field->ndims;
		for (i=0;i<field->ndims;i++)
		{
			field->nval=field->nval*field->dims[i];
		}
	}
	else {
		field->nval=(long)field->dims[0]*(long)field->dims[1];
	}
	if(LONG)	printf("n_dims		= %i\n",field->n_dims);
	if(LONG)	printf("nval		= %li\n",field->nval);

	// Datatype and size
	field->datatype=datatype;
	field->datasize=sizeof(field->datatype);
	if(LONG)	printf("datatype	= %i\n",field->datatype);
	if(LONG)	printf("datasize	= %i\n",field->datasize);

	// x0 and delta
	for(i=0;i<field->ndims;i++)
	{
		field->x0[i]=x0[i];
		if(LONG)	printf("x0[%i]		= %f\n",i,field->x0[i]);
	}
	for(i=0;i<field->ndims;i++)
	{
		field->delta[i]=delta[i];
		if(LONG)	printf("delta[%i]	= %f\n",i,field->delta[i]);
	}
	
	// comment
	if(LONG)	printf("comment	= %s\n",field->comment);
}

//// END ////

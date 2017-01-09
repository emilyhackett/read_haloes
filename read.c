#include "read.h"

//////////////////////////	READING NX,NY,NZ FROM FORTRAN	//////////////////////////

void readhead(FILE *fp)
{
	int length = 0;
	int *len = &length;
	int *buffer;
	
	fread(len,4,1,fp);
	if(length!=12)
	{
		printf("Error reading file - nx,ny,nz not given\n");
		exit(EXIT_FAILURE);
	}
	
	buffer = malloc(length+1);
	fread(buffer,4,4,fp);
	
	if(buffer[3]!=length)
	{
		printf("Error reading file - lengths not equal\n");
		exit(EXIT_FAILURE);
	}
	
	nx=buffer[0];
	ny=buffer[1];
	nz=buffer[2];
	
	if(LONG) printf("nx = %i, ny = %i, nz = %i\n",nx,ny,nz);
}

//////////////////////////	READING GRID VALUES FROM FORTRAN	//////////////////////////

void readgrid(FILE *fp)
{
	int length = 0;
	int *len = &length;
	
	fread(len,4,1,fp);
	if(LONG) printf("Number of grid points is %i\n",length/4);

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
void createNDstruct(void)
{
	field=calloc(1,sizeof(NDfield));
	if(field == NULL)
	{
		printf("Error - NDfield calloc request failed\n");
		exit(EXIT_FAILURE);
	}
	
	// Number of dimensions
	field->ndims=3;
	if(LONG) printf(" ndims		= %i\n",field->ndims);

	// Dimensions of field
	field->dims[0]=nx;
	field->dims[1]=ny;
	field->dims[2]=nz;
	
	for(int i=0;i<field->ndims;i++)
	{
		if(LONG) printf(" dims[%i]	= %i\n",i,field->dims[i]);
	}
	// fdims_index (0 for grid, 1 for particle positions)
	field->fdims_index=0;
	if(LONG) printf(" fdims_index	= %i\n",field->fdims_index);
		// n_dims determined by fdims_index (see NDfield.c)
	field->nval=1;
	if(!field->fdims_index)
	{
		field->n_dims=field->ndims;
		for (int i=0;i<field->ndims;i++)
		{
			field->nval=field->nval*field->dims[i];
		}
	}
	else {
		field->nval=(long)field->dims[0]*(long)field->dims[1];
	}
	if(LONG) printf(" n_dims		= %i\n",field->n_dims);
	if(LONG) printf(" nval		= %li\n",field->nval);

	// Datatype and size
	field->datatype=(1<<8);
	field->datasize=sizeof(field->datatype);
	if(LONG) printf(" datatype	= %i\n",field->datatype);
	if(LONG) printf(" datasize	= %i\n",field->datasize);

	// x0 and delta
	for(int i=0;i<field->ndims;i++)
	{
		printf(" x0[%i]		= %f\n",i,field->x0[i]);
	}
	for(int i=0;i<field->ndims;i++)
	{
		printf(" delta[%i]	= %f\n",i,field->delta[i]);
	}
	
	// comment
	printf(" comment	= %s\n",field->comment);
}

//////////////////////////	SAVING TO NDFIELD FORMAT	//////////////////////////

int saveNDfield(NDfield *field,const char *filename)
{
	int i;
	char tag[16];
	char dummy[160];
	FILE *f;

 	char dims_msg[255];
	sprintf(dims_msg,"[%d",field->dims[0]);
  	for (i=1;i<field->n_dims;i++) sprintf(dims_msg,"%s,%d",dims_msg,field->dims[i]);
  	sprintf(dims_msg,"%s]",dims_msg);

  	char msg[255];
  	strcpy(msg,"FLOAT");
  	if (field->fdims_index!=0) sprintf(msg,"%s coords",msg);
  	else sprintf(msg,"%s array",msg);
  	printf ("Saving %s %s to file %s ...\n",dims_msg,msg,filename);
  	fflush(0);

  	memset(tag,0,16*sizeof(char));
  	memset(dummy,0,160*sizeof(char));
  	strcpy(tag,NDFIELD_TAG);
  	i=16;

  	f=fopen(filename,"w");
  	if(LONG) printf("File %s opened\n",filename);

  	fwrite(&i,sizeof(int),1,f);
  	fwrite(tag,sizeof(char),16,f);
  	fwrite(&i,sizeof(int),1,f);
  	if(CHECK) printf("	- dummy bytes and tag written to file\n");

  	i=sizeof(int)*(NDFIELD_MAX_DIMS+3) + sizeof(double)*(2*NDFIELD_MAX_DIMS) + (160+80)*sizeof(char);
  	fwrite(&i,sizeof(int),1,f);
	if(LONG) printf(" i	= %i\n",i);
  	if(CHECK) printf("	- second dummy byte written to file\n");

  	fwrite(field->comment,sizeof(char),80,f);
	if(CHECK) printf("	- comment written to file\n");
  	fwrite(&field->ndims,sizeof(int),1,f);
  	if(CHECK) printf("	- ndims written to file\n");
  	fwrite(field->dims,sizeof(int),field->ndims,f);
  	if(CHECK) printf("	- dims written to file\n");
  	if (field->ndims<NDFIELD_MAX_DIMS) fwrite(dummy,sizeof(int),NDFIELD_MAX_DIMS-field->ndims,f);
  	if(CHECK) printf("	- dummy[160] written to file\n");
  	fwrite(&field->fdims_index,sizeof(int),1,f);
  	if(CHECK) printf("	- fdims_index written to file\n");
	fwrite(&field->datatype,sizeof(int),1,f);
	if(CHECK) printf("	- datatype written to file\n");
	fwrite(field->x0,sizeof(double),field->ndims,f);
	if(CHECK) printf("	- x0 written to file\n");
	if (field->ndims<NDFIELD_MAX_DIMS) fwrite(dummy,sizeof(double),NDFIELD_MAX_DIMS-field->ndims,f);
	if(CHECK) printf("	- extra dims using dummy[160] written to file\n");
	fwrite(field->delta,sizeof(double),field->ndims,f);
	if(CHECK) printf("	- delta written to file\n");
	if (field->ndims<NDFIELD_MAX_DIMS) fwrite(dummy,sizeof(double),NDFIELD_MAX_DIMS-field->ndims,f);
	if(CHECK) printf("	- extra dims using dummy[160] written to file\n");
	fwrite(field->dummy,sizeof(char),160,f);
	if(CHECK) printf("	- entire of dummy[160] written to file\n");
	fwrite(&i,sizeof(int),1,f);
	if(CHECK) printf("	- dummy byte written to file\n");
  
  	i=field->nval*sizeof(float);
	fwrite(&i,sizeof(int),1,f);
	if(CHECK) printf("	- second dummy byte written to file\n");
	fwrite(field->val,sizeof(float),field->nval,f);
	if(CHECK) printf("	- values written to file\n");
	fwrite(&i,sizeof(int),1,f);
	if(CHECK) printf("	- final dummy byte written to file\n");

	printf("Finished writing to file\n");
	fclose(f);
	if(LONG) printf ("File %s closed\n",filename);
	return 0;
}

//////////////////////////	READING NDFIELD STRUCTURE INTO COLUMN DATA	//////////////////////////
void coldata(char *filename)
{
	FILE *fp;
	
	fp=fopen(filename,"w");
	if(fp==NULL)
	{
		printf("Error opening file %s for col data write\n",filename);
		exit(EXIT_FAILURE);
	}
	printf("File %s opened for col data write\n",filename);

	int v=0;
	for (int i=0;i<nx;i++)
	{
		for(int j=0;j<ny;j++)
		{
			for(int k=0;k<nz;k++)
			{
				fprintf(fp,"%i	%i	%i	%f\n",i,j,k,field->val[v]);
				v++;
			}
		}
		fprintf(fp,"\n");
	}
	printf("Grid data written to file in form:\nx	y	z	field->val\n");

	fclose(fp);
	printf("File %s closed\n",filename);
}




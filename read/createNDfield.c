#include "read.h"

//////////////////////////	READING STRUCTURE TO NDFIELD STRUCT	//////////////////////////
void create_NDstruct(int fdims_index,int datatype,double *x0,double *delta)
{
	// fdims_index (0 for grid, 1 for particle positions)
	field->fdims_index=fdims_index;
	printf("fdims_index	= %i\n",field->fdims_index);
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
	printf("n_dims		= %i\n",field->n_dims);
	printf("nval		= %li\n",field->nval);

	// Datatype and size
	field->datatype=datatype;
	field->datasize=sizeof(field->datatype);
	printf("datatype	= %i\n",field->datatype);
	printf("datasize	= %i\n",field->datasize);

	// x0 and delta
	for(int i=0;i<field->ndims;i++)
	{
		field->x0[i]=x0[i];
		printf("x0[%i]		= %f\n",i,field->x0[i]);
	}
	for(int i=0;i<field->ndims;i++)
	{
		field->delta[i]=delta[i];
		printf("delta[%i]	= %f\n",i,field->delta[i]);
	}
	
	// comment
	printf("comment	= %s\n",field->comment);
}

//////////////////////////	SAVING TO NDFIELD FORMAT	//////////////////////////

int save_NDfield(const char *filename)
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
  	printf("File %s opened\n",filename);

  	fwrite(&i,sizeof(int),1,f);
  	fwrite(tag,sizeof(char),16,f);
  	fwrite(&i,sizeof(int),1,f);

  	i=sizeof(int)*(NDFIELD_MAX_DIMS+3) + sizeof(double)*(2*NDFIELD_MAX_DIMS) + (160+80)*sizeof(char);
  	fwrite(&i,sizeof(int),1,f);
	printf(" i	= %i\n",i);
  	fwrite(field->comment,sizeof(char),80,f);
  	fwrite(&field->ndims,sizeof(int),1,f);
  	fwrite(field->dims,sizeof(int),field->ndims,f);
  	if (field->ndims<NDFIELD_MAX_DIMS) fwrite(dummy,sizeof(int),NDFIELD_MAX_DIMS-field->ndims,f);
  	fwrite(&field->fdims_index,sizeof(int),1,f);
	fwrite(&field->datatype,sizeof(int),1,f);
	fwrite(field->x0,sizeof(double),field->ndims,f);
	if (field->ndims<NDFIELD_MAX_DIMS) fwrite(dummy,sizeof(double),NDFIELD_MAX_DIMS-field->ndims,f);
	fwrite(field->delta,sizeof(double),field->ndims,f);
	if (field->ndims<NDFIELD_MAX_DIMS) fwrite(dummy,sizeof(double),NDFIELD_MAX_DIMS-field->ndims,f);
	fwrite(field->dummy,sizeof(char),160,f);
	fwrite(&i,sizeof(int),1,f);
  
  	i=field->nval*sizeof(float);
	fwrite(&i,sizeof(int),1,f);
	fwrite(field->val,sizeof(float),field->nval,f);
	fwrite(&i,sizeof(int),1,f);

	printf("Finished writing to file\n");
	fclose(f);
	printf ("File %s closed\n",filename);
	return 0;
}

//// END ////

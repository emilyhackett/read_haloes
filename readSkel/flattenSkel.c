#include "readSkel.h"

// Require function that takes a 3D NDskel (i.e. set of nodes and filaments
// and flattens it to a 2D representation:
// 
// Takes params NDskel (i.e. the entire skeleton read from a mse program) and
// the two dimensions that are required (default 0 and 1 i.e. x and y)


void FlattenSkl(NDskel *skl)
{
	
	printf("Reading in %iD skeleton ... \n",skl->ndims);
	
	if(skl->ndims!=3)
	{
		printf("ERROR: Flattening only works from 3D to 2D skeleton\n");
		exit(EXIT_FAILURE);
	}


	// Define new NDskel to return from function
	NDskel *new;
	

	printf("Reading and flattening %i segments ...\n",skl->nsegs);
	
	// Define arrays of segment positions
	float *xpos;
	xpos=malloc(sizeof(float)*skl->nsegs*2);
		if(xpos==NULL)
		{
			fprintf(stderr,"xpos malloc failed\n");
		}
	float *ypos;
	ypos=malloc(sizeof(float)*skl->nsegs*2);
		if(ypos==NULL)
		{
			fprintf(stderr,"ypos malloc failed\n");
		}
	float *zpos;
	zpos=malloc(sizeof(float)*skl->nsegs*2);
		if(zpos==NULL)
		{
			fprintf(stderr,"zpos malloc failed\n");
		}

	// Define counter for xpos,ypos and zpos
	int x=0;
	int y=0;
	int z=0;

	for(int i=0;i<skl->nsegs*2*skl->ndims;i++)
	{
		if((i%3)==0)
		{
			xpos[x]=skl->segpos[i];
//			printf("xpos[%i] = %.2f	",x,xpos[x]);
			x++;
		}
		if((i%3)==1)
		{
			ypos[y]=skl->segpos[i];
//			printf("ypos[%i] = %.2f	",y,ypos[y]);
			y++;
		}
		if((i%3)==2)
		{
			zpos[z]=skl->segpos[i];
//			printf("zpos[%i] = %.2f\n",z,zpos[z]);
			z++;
		}	
	}

	printf("Number of xpos values = %i\n",x);
	printf("Number of ypos values = %i\n",y);
	printf("Number of zpos values = %i\n",z);
	
	printf("Number of segments is then = %i = %i = %i = %i \n",x/2,y/2,z/2,skl->nsegs);
	
}

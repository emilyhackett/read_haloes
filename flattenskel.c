#include "readNDskel.h"

// Require function that takes a 3D NDskel (i.e. set of nodes and filaments
// and flattens it to a 2D representation:
// 
// Takes params NDskel (i.e. the entire skeleton read from a mse program) and
// the two dimensions that are required (default 0 and 1 i.e. x and y)


void FlattenSkl(NDskel *skl)
{
	printf("Reading and flattening %i segments ...\n",skl->nsegs);
	
	float *segments[2];

	int j=0;
	
	for(int i=0;i>skl->nsegs;i++)
	{
		segments[i][0]=skl->segpos[j];
			j++;
		segments[i][1]=skl->segpos[j];
			j++;
		// Skip third value
			j++;
	}	
}

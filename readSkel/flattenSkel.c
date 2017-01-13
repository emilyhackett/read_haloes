#include "readSkel.h"

// Require function that takes a 3D NDskel (i.e. set of nodes and filaments
// and flattens it to a 2D representation:
// 
// Takes params NDskel (i.e. the entire skeleton read from a mse program) and
// the two dimensions that are required (default 0 and 1 i.e. x and y)

float *TrimDimension(float *oldpos,int npos)
{
	float *newpos;
	newpos=malloc(2*npos*sizeof(float));	// Allocate for 2D

	int j=0;	// Index for iterating along newpos list
	for(int i=0;i<npos*3;i++)	// Iterate along old pos list
	{
		if((i%3)==0)
		{
			newpos[j]=oldpos[i];
			j++;
		}
		if((i%3)==1)
		{
			newpos[j]=oldpos[i];
			j++;
		}
		if((i%3)==2)
		{
			continue;	// Ignore 3rd coordinate
		}	
	}

	return newpos;
}

NDskel *FlattenSkl(NDskel *skl)
{
	printf("Reading in %iD skeleton ... \n",skl->ndims);
	
	if(skl->ndims!=3)
	{
		printf("ERROR: Flattening only works from 3D to 2D skeleton\n");
		exit(EXIT_FAILURE);
	}

	// DEFINE LISTS newnodepos and newsegpos - iterate through old skeleton and fill
	// these with only 2 dimension positions, ignore 3rd value
	// Then memcpy these lists to the new skeleton instead of the old one
	//
	// Maybe makes sense to define a different function, "float *TrimDimension(float *) 
	// that takes a 3D list and returns a 2D list.


	// Define new NDskel to return from function
	NDskel *new;

	int i,j;
      	char dummy[160];
	int index;

	// Assign memory to new NDskel
	new=calloc(1,sizeof(NDskel));
	memcpy(new,skl,sizeof(NDskel));
	
	// Set the number of dims of the new skeleton to 2
	// IMPORTANT STEP HERE!
	new->ndims=2;
	printf("new->ndims is now set to %i\n",new->ndims);

	new->dims=calloc(new->ndims,sizeof(int));
	new->x0=calloc(new->ndims,sizeof(double));
	new->delta=calloc(new->ndims,sizeof(double));

	new->segdata_info=calloc(new->nsegdata,sizeof(char *));
	new->nodedata_info=calloc(new->nnodedata,sizeof(char *));

	// Transfer memory data across from old skeleton to new	
	// NOTE: Only copying first two dimensions over (x and y)
	memcpy(new->dims,skl->dims,new->ndims*sizeof(int));
	memcpy(new->x0,skl->x0,new->ndims*sizeof(double));
	memcpy(new->delta,skl->delta,new->ndims*sizeof(double));
	
	for(i=0;i<new->nsegdata;i++)
	{
		new->segdata_info[i]=calloc(NDSKEL_DATA_STR_SIZE,sizeof(char));
		strcpy(new->segdata_info[i],skl->segdata_info[i]);
	}
	for(i=0;i<new->nnodedata;i++)
	{
		new->nodedata_info[i]=calloc(NDSKEL_DATA_STR_SIZE,sizeof(char));
		strcpy(new->nodedata_info[i],skl->nodedata_info[i]);
	}
	
	// Transfer across field data (no flattening required)
	new->segdata=malloc((long)sizeof(double)*new->nsegs*new->nsegdata);
	memcpy(new->segdata,skl->segdata,sizeof(double)*new->nsegs*new->nsegdata);

	new->nodedata=malloc((long)sizeof(double)*new->nnodes*new->nnodedata);
	memcpy(new->nodedata,skl->nodedata,sizeof(double)*new->nnodes*new->nnodedata);
	
	new->Node=malloc((long)sizeof(NDskl_node)*new->nnodes);
	memcpy(new->Node,skl->Node,new->nnodes*sizeof(NDskl_node));

	new->Seg=malloc((long)sizeof(NDskl_seg)*skl->nsegs);
	memcpy(new->Seg,skl->Seg,new->nsegs*sizeof(NDskl_seg));

	//// NEED TO CHANGE THIS - HERE IS WHERE FLATTENING OCCURS ////
	new->segpos=malloc((long)sizeof(float)*new->nsegs*2*new->ndims);
	new->segpos=TrimDimension(skl->segpos,new->nsegs);
	
	new->nodepos=malloc((long)sizeof(float)*new->nnodes*new->ndims);
	new->nodepos=TrimDimension(skl->nodepos,new->nnodes);

	// Index over all nodes and copy across
	for(i=0;i<new->nnodes;i++)
	{
		memcpy(&new->Node[i],&skl->Node[i],sizeof(NDskl_node));
		new->Node[i].Next=malloc(new->Node[i].nnext*sizeof(NDskl_node *));
		new->Node[i].Seg=malloc(new->Node[i].nnext*sizeof(NDskl_seg *));
		new->Node[i].nsegs=malloc(skl->Node[i].nnext*sizeof(int));
		new->Node[i].data=skl->Node[i].data+(new->nodedata-skl->nodedata);
		new->Node[i].pos=skl->Node[i].pos+(new->nodepos-skl->nodepos); // POS TO CHANGE
	
		for(j=0;j<new->Node[i].nnext;j++)
		{
			new->Node[i].Next[j]=skl->Node[i].Next[j]+(new->Node-skl->Node);
			new->Node[i].Seg[j]=skl->Node[i].Seg[j]+(new->Seg-skl->Seg);
			new->Node[i].nsegs[j]=skl->Node[i].nsegs[j];
		}
	}

	// Index over all segments and copy across
	for(i=0;i<new->nsegs;i++)
	{
		memcpy(&new->Seg[i],&skl->Seg[i],sizeof(NDskl_seg));
		new->Seg[i].pos=skl->Seg[i].pos+(new->segpos-skl->segpos);	// POS TO CHANGE
		new->Seg[i].data=skl->Seg[i].data+(new->segdata-skl->segdata);
		if(skl->Seg[i].Next!=NULL)
		{
			new->Seg[i].Next=skl->Seg[i].Next+(new->Seg-skl->Seg);
		}
		if(skl->Seg[i].Prev!=NULL)
		{
			new->Seg[i].Prev=skl->Seg[i].Prev+(new->Seg-skl->Seg);
		}
	}
		
	return new;
}

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "myendian.h"
#include "NDskeleton.h"

// Define global variables required for structure and functions

#define NDSKEL_DATA_STR_SIZE 20
#define NDSKEL_TAG "NDSKEL"
#define NDSKEL_ASCII_TAG "ANDSKEL"
#define NDSKEL_MAX_DIMS 20

#define FLAG_NDNODE_BOUNDARY (1<<0)
#define FLAG_NDNODE_OUT      (1<<1)
#define FLAG_NDNODE_INFINITE (1<<2)

////////////////////	GLOBAL STRUCTURES FOR ND SKELETONS	////////////////////

struct NDskl_seg_str {
	int nodes[2];	/* Index of the nodes at the extremity of the arc, segment
			   oriented from nodes[0] to nodes[1] */
	float *pos;	/* Points to appropriate location in segpos */
	int flags;	/* Non null if on boundary */
	int index;	/* Index of the segment in the Seg array */
	double *data;	/* Points to the nsegdata supplementary data for this
			   segment */
	struct NDskl_seg_str *Next;	/* Points to next segment in the arc
					   NULL if extremity (connected to nodes[1]) */
	struct NDskl_seg_str *Prev;	/* Points to previous segment in the arc
					   NULL if extremity (connected to nodes[0]) */
}; 
typedef struct NDskl_seg_str NDskl_seg;

struct NDskl_node_str {
	float *pos;	/* Points to appropriate location in nodepos */
	int flags;	/* Non null if on boundary */
	int nnext;	/* Number of arcs connected */
	int type;   	/* Critical index (?) */
	int index;	/* Index of the node in the Node array */
    	int *nsegs; 	/* Number of segments in each of the nnext arcs */
    	double *data;	/* Points to the nnodedata supplementary data for this 
			   segment */
    	struct NDskl_node_str **Next;	/* Points to the node at the other extremity of
					 each of the nnext arcs */  
    	struct NDskl_seg_str **Seg;	/* Points to the first segment in the arcs, 
					   starting from the current node */
}; 
typedef struct NDskl_node_str NDskl_node;

typedef struct NDskel_str {
    	char comment[80];
    
    	int ndims;	/* Number of dimensions */
    	int *dims;	/* Dimension of the underlying grid, only meaningful when
			   computed from a regular grid */
    	double *x0;	/* Origin of the bounding box */
    	double *delta;	/* Dimension of the bounding box */
    
    	int nsegs;	/* Number of segments */
    	int nnodes;	/* Number of nodes */
    
    	int nsegdata;	/* Number of additional data for segments */
    	int nnodedata;	/* Number of additional data for nodes */
    	char **segdata_info;	/* Name of additional fields for segments */
    	char **nodedata_info;	/* Name of additional fields for nodes */
    
    	float *segpos;	/* Positions of the extremities of segments, 2x ndims coordinates
			   for each segment */
    	float *nodepos;	/* Positions of the nodes (ndims coords for each segment) */
    	double *segdata;	/* Additional ddata for segments (nsegs times nsegdata
				   consecutive values */
    	double *nodedata;	/* Additional data for nodes (nnodes times nnodedata 
				   consecutive values */
    
    	NDskl_seg *Seg;		/* Segment array (contains all segs) */
    	NDskl_node *Node;	/* Nodes array (contains all nodes) */
} NDskel;


////////////////////	READ ND SKELETON INTO A NDSKEL STRUCTURE	////////////////////	
void readNDskeleton(char *filename)
{
	printf("Reading ND skeleton structure from file %s\n",filename);

	FILE *fp;
	fp=fopen(filename,"r");
	if(fp==NULL)
	{
		fprintf(stderr,"Error opening file %s\n",filename);
	}
	else {
		printf("%s opened for reading\n",filename);
	}
	

	int i,j;
      	char tag[NDSKEL_DATA_STR_SIZE];
      	char dummy[160];
       	int swap=0;
	int index;
      
	// Define structure to read in to
	NDskel *skl;

	skl=calloc(1,sizeof(NDskel));
	memset(tag,0,16*sizeof(char));

	fread_sw(&i,sizeof(int),1,fp,swap);	// DUMMY
	if(i!=16) swap=1-swap;
	
	fread_sw(tag,sizeof(char),16,fp,swap);
	printf("tag	= %s\n",tag);
	fread_sw(&i,sizeof(int),1,fp,swap);	// DUMMY

	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
	fread_sw(skl->comment,sizeof(char),80,fp,swap);
	printf("comment	= %s\n",skl->comment);
	
	fread_sw(&skl->ndims,sizeof(int),1,fp,swap);
	printf("ndims	= %i\n",skl->ndims);
	

	// Allocate room based on ndims
	skl->dims=malloc(skl->ndims*sizeof(int));
	skl->x0=malloc(skl->ndims*sizeof(double));
	skl->delta=malloc(skl->ndims*sizeof(double));
	
	// Read in dummy variables after allocated dimensions
	fread_sw(skl->dims,sizeof(int),skl->ndims,fp,swap);
	if (skl->ndims<NDSKEL_MAX_DIMS) 
	{
		fread_sw(dummy,sizeof(int),NDSKEL_MAX_DIMS-skl->ndims,fp,swap);
	}
      	fread_sw(skl->x0,sizeof(double),skl->ndims,fp,swap);
      	if (skl->ndims<NDSKEL_MAX_DIMS) 
	{
		fread_sw(dummy,sizeof(double),NDSKEL_MAX_DIMS-skl->ndims,fp,swap);
	}
      	fread_sw(skl->delta,sizeof(double),skl->ndims,fp,swap);
      	if (skl->ndims<NDSKEL_MAX_DIMS) 
	{
		fread_sw(dummy,sizeof(double),NDSKEL_MAX_DIMS-skl->ndims,fp,swap);
	}
	// Print out dims, x0 and delta to check
	for(int i=0;i<skl->ndims;i++)
	{
		printf("dims[%i] = %i,	x0[%i] = %.2f,	delta[%i] = %.2f\n",i,skl->dims[i],i,
				skl->x0[i],i,skl->delta[i]);
	}

	fread_sw(&skl->nsegs,sizeof(int),1,fp,swap);
	printf("nsegs	= %i\n",skl->nsegs);
      	fread_sw(&skl->nnodes,sizeof(int),1,fp,swap);
	printf("nnodes	= %i\n",skl->nnodes);
      	fread_sw(&skl->nsegdata,sizeof(int),1,fp,swap);
	printf("nsegdata	= %i\n",skl->nsegdata);
      	fread_sw(&skl->nnodedata,sizeof(int),1,fp,swap);
	printf("nnodedata	= %i\n",skl->nnodedata);
      	fread_sw(&j,sizeof(int),1,fp,swap);


	// Clear and assign data for segment and node data info
	skl->segdata_info = calloc(skl->nsegdata,sizeof(char *));
      	skl->nodedata_info = calloc(skl->nnodedata,sizeof(char *));
 
      	if (skl->nsegdata)	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
	
      	for (i=0;i<skl->nsegdata;i++)
    	{
	  	skl->segdata_info[i]=calloc(NDSKEL_DATA_STR_SIZE,sizeof(char));
	  	fread_sw(skl->segdata_info[i],sizeof(char),NDSKEL_DATA_STR_SIZE,fp,swap);
		printf(" segdata_info[%i]	= %s\n",i,skl->segdata_info[i]);
    	}
	
	// Read in two dummy variables
      	if (skl->nsegdata)	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
      	if (skl->nnodedata)	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
	
      	for (i=0;i<skl->nnodedata;i++)
    	{
	  	skl->nodedata_info[i]=calloc(NDSKEL_DATA_STR_SIZE,sizeof(char));
	  	fread_sw(skl->nodedata_info[i],sizeof(char),NDSKEL_DATA_STR_SIZE,fp,swap);
		printf(" nodedata_info[%i]	= %s\n",i,skl->nodedata_info[i]);
    	}
      
	if (skl->nnodedata)	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
	
	// Assign memory for segment positions
	skl->segpos = malloc((long)sizeof(float)*skl->nsegs*2*skl->ndims);

	
       	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
      	fread_sw(skl->segpos,sizeof(float),skl->nsegs*2*skl->ndims,fp,swap);
	printf("segpos read in, total of %i\n",skl->nsegs*2*skl->ndims);
//	for(int i=0;i<skl->nsegs;i++)
//	{
//		printf("	%.2f\n",skl->segpos[i]);
//	}
      	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
  
	// Assign memory for node positions
      	skl->nodepos = malloc((long)sizeof(float)*skl->nnodes*skl->ndims);
      	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
      	fread_sw(skl->nodepos,sizeof(float),skl->nnodes*skl->ndims,fp,swap);
	printf("nodepos	read in, total of %i\n",skl->nnodes*skl->ndims);
//	for(int i=0;i<skl->nnodes;i++)
//	{
//		printf("	%.2f\n",skl->nodepos[i]);
//	}
	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY

	// Assign memory for segment data
      	skl->segdata = malloc((long)sizeof(double)*skl->nsegs*skl->nsegdata);
      	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
      	fread_sw(skl->segdata,sizeof(double),skl->nsegs*skl->nsegdata,fp,swap);
//	printf("segdata	= %s\n",skl->segdata);
      	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY

	// Assign memory for node data
      	skl->nodedata = malloc((long)sizeof(double)*skl->nnodes*skl->nnodedata);
      	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
      	fread_sw(skl->nodedata,sizeof(double),skl->nnodes*skl->nnodedata,fp,swap);
      	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY

	// Assign memory for Node and Seg
      	skl->Node = malloc((long)sizeof(NDskl_node)*skl->nnodes);      
	skl->Seg = malloc((long)sizeof(NDskl_seg)*skl->nsegs);

	// Initialise node and seg lists;
      	NDskl_node *node;
      	NDskl_seg *seg;

	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY

	// Index over all nodes and read in data      
	for(i=0;i<skl->nnodes;i++)
    	{
		// Initialise array of nodes 
	  	node = &skl->Node[i];
	  	fread_sw(&index,sizeof(int),1,fp,swap);	
	  	node->pos = &skl->nodepos[(long)index*skl->ndims];
	  	if (skl->nnodedata) node->data = &skl->nodedata[(long)index*skl->nnodedata];
	  	else node->data=NULL;
	  	fread_sw(&node->flags,sizeof(int),1,fp,swap);
	  	fread_sw(&node->nnext,sizeof(int),1,fp,swap);
	  	fread_sw(&node->type,sizeof(int),1,fp,swap);
	   	fread_sw(&node->index,sizeof(int),1,fp,swap);

		// Allocate memory for next node
	  	node->nsegs=malloc(sizeof(int)*node->nnext);
	  	node->Next=malloc(sizeof(NDskl_node *)*node->nnext);
	  	node->Seg=malloc(sizeof(NDskl_seg *)*node->nnext);
	  	fread_sw(node->nsegs,sizeof(int),node->nnext,fp,swap);
	  	for (j=0;j<node->nnext;j++)
		{
	      		fread_sw(&index,sizeof(int),1,fp,swap);
	      		if (index>=0) node->Next[j] = &skl->Node[index];
	      		else node->Next[j] =NULL;

	      		fread_sw(&index,sizeof(int),1,fp,swap);

	     		if (index>=0) node->Seg[j] = &skl->Seg[index];
	     		else node->Seg[j] =NULL;
		}
    	}
	printf("node array read\n");

      	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
      	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
      
	for(i=0;i<skl->nsegs;i++)
    	{
		// Initialise array of segments
	  	seg = &skl->Seg[i];
	  	fread_sw(&index,sizeof(int),1,fp,swap);
	  	seg->pos = &skl->segpos[(long)index*2*skl->ndims];
	 	if (skl->nsegdata) seg->data = &skl->segdata[(long)index*skl->nsegdata];
	  	else seg->data = NULL;
	  	fread_sw(seg->nodes,sizeof(int),2,fp,swap);
	  	fread_sw(&seg->flags,sizeof(int),1,fp,swap);
	  	fread_sw(&seg->index,sizeof(int),1,fp,swap);
      
	  	fread_sw(&index,sizeof(int),1,fp,swap);
	  	if (index<0) seg->Next=NULL;
	  	else seg->Next = &skl->Seg[index];
      
	  	fread_sw(&index,sizeof(int),1,fp,swap);
	  	if (index<0) seg->Prev=NULL;
	  	else seg->Prev = &skl->Seg[index];
    
    	}
      	printf("seg array read\n");

	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY

	printf("%s read COMPLETED\n",filename);

	fclose(fp);
}


////////////////////	MAIN FUNCTION TO TAKE IN FILE AS COMMAND ARGUMENT ////////////////////	

int main(int argc, char *argv[])
{
	if(argc>1)
	{
		readNDskeleton(argv[1]);
	}
	else {
		fprintf(stderr,"Usage: %s file\n",argv[0]);
	}

	printf("---- END OF PROGRAM REACHED ----\n");

	return 0;
}

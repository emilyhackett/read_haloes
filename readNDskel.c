#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "myendian.h"

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
      
	// Define structure to read in to
	NDskel *skl;

	skl=calloc(1,sizeof(NDskel));
	memset(tag,0,16*sizeof(char));


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

	return 0;
}

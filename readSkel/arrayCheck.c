#include "readSkel.h"

// 	Aim of this function/s will be to index through the array
//	of segments contained in the NDskel structure and extract info
//	to be put in to a column data file that can then be plotted

void ExampleSegment(NDskel *skl,int n)
{
	int i;
	//	Prints to screen example of a NDskl_seg structure
	//	so that data can be understood.
	
	printf("EXAMPLE SEGMENT: skl->Seg[%i]\n",n);

	printf("Indexes of the two nodes at the extremities of the arc are:\n");
	printf(" nodes[0]	= %i\n",skl->Seg[n].nodes[0]);
	printf(" nodes[1]	= %i\n",skl->Seg[n].nodes[1]);

	// I.e. points to a float that is position in segpos
	printf("The %iD position coordinate of the segment is:\n",skl->ndims);
	for(i=0;i<skl->ndims;i++)
	{
		printf(" pos[%i]		= %f\n",i,skl->Seg[n].pos[i]);
	}

	printf("The flag will be non null if on the boundary:\n");
	printf(" flags		= %i\n",skl->Seg[n].flags);
	printf("The index of the segment in the Seg array is:\n");
	printf(" index		= %i\n",skl->Seg[n].index);

	// I.e. points to a double that is position in segdata
	printf("The supplementary data for this segment is:\n");
	for(i=0;i<skl->nsegdata;i++)
	{
		printf(" data[%i]	= %f\n",i,skl->Seg[n].data[i]);
	}
	
	printf("NOTE: Next and Prev segments not printed\n");
}

void ExampleNode(NDskel *skl,int n)
{
	int i;
	//	Prints to screen example of a NDskl_node structure
	//	so that data can be understood

	printf("EXAMPLE NODE: skl->Node[%i]\n",n);

	printf("The %iD position coordinate of the node is:\n",skl->ndims);
	for(i=0;i<skl->ndims;i++)
	{
		printf(" pos[%i]		= %f\n",i,skl->Node[n].pos[i]);
	}
	
	printf("The flag will be non null if on the boundary:\n");
	printf(" flags		= %i\n",skl->Node[n].flags);
	printf("The number of arcs connected to this node is:\n");
	printf(" nnext		= %i\n",skl->Node[n].nnext);
	printf("The critical index of this node is:\n");
	printf(" type		= %i\n",skl->Node[n].type);
	printf("The index of this node in the Node array is:\n");
	printf(" index		= %i\n",skl->Node[n].index);

	// Index over all connected arcs to find there number of segments
	printf("The number of segments in each of the nnext Arcs is:\n");
	for(i=0;i<skl->Node[n].nnext;i++)
	{
		printf(" nsegs[%i]	= %i\n",i,skl->Node[n].nsegs[i]);
	}
	
	printf("The supplementary data for this node is:\n");
	for(i=0;i<skl->nnodedata;i++)
	{
		printf(" data[%i]	= %f\n",i,skl->Node[n].data[i]);
	}
	
	printf("NOTE: Next nodes array and Seg array for each arc not printed\n");
	
}

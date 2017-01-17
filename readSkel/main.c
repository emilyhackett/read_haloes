#include "readSkel.h"

#define EXAMPLE		0	// Basically a concept check - no output files wanted

#define PLOTNODES	0	// Output node pos to file and save 2D list plot to .ps file
#define PLOTSEGS	1	// Output seg pos to file and save 2D list plot to .ps file

#define PLOTNODEFIELDS	0	// Ouptut all node field data to .dat file
#define PLOTSEGFIELDS	0	// Output all seg field data to .dat file

#define NODEDATA	0	// Output all node data (position and field values) to .dat file

////////////////////	MAIN FUNCTION TO TAKE IN FILE AS COMMAND ARGUMENT ////////////////////

int main(int argc, char *argv[])
{
	
	
	// Define NDskeleton
	NDskel	*skl;
	
	if(argc<2)
	{
		fprintf(stderr,"Usage: %s file\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("\n	---------- READING .NDskl FILE ----------\n");
		skl=readNDskeleton(argv[1]);
	}
	
	// Check sanity of skeleton (e.g. carry over contamination from .ND files)
	NDskelCheckSanity(skl,0);

	// Save ASCII skeleton if filename passed to function
	if(argc>2 && !EXAMPLE)
	{
		printf("	---------- SAVING ASCII SKELETON ----------\n");
		Save_ASCIIskel(skl,argv[2]);
	}	

	if(PLOTNODES && !EXAMPLE)
	{
		printf("	---------- PLOTTING NODE POSITIONS ----------\n");
		PlotNodePos(skl,"nodepos.dat","nodeplot.ps",0.2,0.4,0);
	}
	
	if(PLOTSEGS && !EXAMPLE)
	{
		printf("	---------- PLOTTING SEGMENT POSITIONS ----------\n");
		PlotSegPos(skl,"segpos.dat","segplot.ps","GAS10G-xygrid.dat",0.35,0.4,1);
	}
			

	if(PLOTNODEFIELDS && !EXAMPLE)
	{
		printf("	---------- WRITING NODE FIELD VALUES ----------\n");
		ListNodeFieldVals(skl,"nodefieldvals.dat");
	}

	if(PLOTSEGFIELDS && !EXAMPLE)
	{
		printf("	---------- WRITING SEGMENT FIELD VALUES ----------\n");
		ListSegFieldVals(skl,"segfieldvals.dat");
	}

	if(NODEDATA && !EXAMPLE)
	{
		printf("	---------- WRITING NODE DATA TO FILE ----------\n");
		NodeData(skl,"NodePosField.dat");
	}
	
/*	if(EXAMPLE)
	{
		printf("	---------- CONCEPT CHECK RUN - EXAMPLE FLAGGED ----------\n");
		ExampleSegment(skl,10);
		ExampleNode(skl,10);
	}
*/
	printf("	---------- END OF PROGRAM REACHED ----------\n\n");

	return 0;
}

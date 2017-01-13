#include "readSkel.h"

#define FLATTEN		0	// Take a 3D skeleton and flatten to 2D position by ignoring z coords

#define PLOTNODES	0	// Output node pos to file and save 2D list plot to .ps file
#define PLOTSEGS	0	// Output seg pos to file and save 2D list plot to .ps file

#define PLOTNODEFIELDS	0	// Ouptut all node field data to .dat file
#define PLOTSEGFIELDS	0	// Output all seg field data to .dat file

#define NODEDATA	1	// Output all node data (position and field values) to .dat file

////////////////////	MAIN FUNCTION TO TAKE IN FILE AS COMMAND ARGUMENT ////////////////////

int main(int argc, char *argv[])
{
	
	printf("\n");
	printf("	---------- READING .NDskl FILE ----------\n");
	// Define NDskeleton
	NDskel	*skl;
	
	if(argc>1)
	{
		skl=readNDskeleton(argv[1]);
	}
	else if(argc<2) {
		fprintf(stderr,"Usage: %s file\n",argv[0]);
	}
	
	NDskelCheckSanity(skl,0);

	if(argc>2)
	{
		printf("	---------- SAVING ASCII SKELETON ----------\n");
		Save_ASCIIskel(skl,argv[2]);
	}	
	
	if(FLATTEN)
	{
		printf("	---------- FLATTENING TO 2D SKELETON ----------\n");
		FlattenSkl(skl);
	}	

	if(PLOTNODES)
	{
		printf("	---------- PLOTTING NODE POSITIONS ----------\n");
		PlotNodePos(skl,"nodepos.dat","nodeplot.ps",1);
	}
	
	if(PLOTSEGS)
	{
		printf("	---------- PLOTTING SEGMENT POSITIONS ----------\n");
		PlotSegPos(skl,"segpos.dat","segplot.ps",1);
	}

	if(PLOTNODEFIELDS)
	{
		printf("	---------- WRITING NODE FIELD VALUES ----------\n");
		ListNodeFieldVals(skl,"nodefieldvals.dat");
	}

	if(PLOTSEGFIELDS)
	{
		printf("	---------- WRITING SEGMENT FIELD VALUES ----------\n");
		ListSegFieldVals(skl,"segfieldvals.dat");
	}

	if(NODEDATA)
	{
		printf("	---------- WRITING NODE DATA TO FILE ----------\n");
		NodeData(skl,"NodePosField.dat");
	}
	
	printf("	---------- END OF PROGRAM REACHED ----------\n\n");

	return 0;
}

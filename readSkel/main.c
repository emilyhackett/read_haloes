#include "readSkel.h"

#define FLATTEN 0
#define PLOTNODES 0
#define PLOTSEGS 0

////////////////////	MAIN FUNCTION TO TAKE IN FILE AS COMMAND ARGUMENT ////////////////////

int main(int argc, char *argv[])
{
	printf("\n");
	printf("	--------------------------------------------\n");
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
		printf("	--------------------------------------------\n");
		Save_ASCIIskel(skl,argv[2]);
	}	
	
	if(FLATTEN)
	{
		printf("	--------------------------------------------\n");
		FlattenSkl(skl);
	}	

	if(PLOTNODES)
	{
		printf("	--------------------------------------------\n");
		PlotNodes(skl,"nodepos","nodeplot.ps");
	}
	
	if(PLOTSEGS)
	{
		printf("	--------------------------------------------\n");
		PlotSegs(skl,"segpos","segplot.ps");
	}

//	MakePlotFile(skl,"CP_data");

	NodeFieldVals(skl,"NodeData");
	
	printf("	---------- END OF PROGRAM REACHED ----------\n\n");

	return 0;
}

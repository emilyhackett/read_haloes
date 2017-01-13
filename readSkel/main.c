#include "readSkel.h"

#define FLATTEN 0
#define PLOTNODES 1
#define PLOTSEGS 1
#define PLOTNODEFIELDS 1
#define PLOTSEGFIELDS 1

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

	printf("	---------- END OF PROGRAM REACHED ----------\n\n");

	return 0;
}

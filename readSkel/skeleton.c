#include "readNDskel.h"

#define FLATTEN 1



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
	
	printf("	--------------------------------------------\n");
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

	printf("	---------- END OF PROGRAM REACHED ----------\n\n");

	return 0;
}

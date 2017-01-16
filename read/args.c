#include "read.h"

//Functions for determining flags present from command line arguments
void flags(int argc, char *argv[])
{
	int opt;
	while((opt = getopt(argc, argv, OPTLIST)) != -1)
	{
		switch(opt)
		{
			case 'n':		// Create NDfield file
				nflag = !nflag;
				break;
			case 'c':		// Create column data text file
				cflag = !cflag;
				break;
	    		case 'd':		// Create density plots
		 		dflag = !dflag;
				break;
	    		case 'r':		// Create mass-radius plots
				rflag = !rflag;
				break;
			case 'i':		// Calculate moment of inertia tensor
				iflag= !iflag;
				radius=atof(optarg);	
				break;
		}
    	}

	if(dflag)
	{
		cflag=true;
	}

/*	printf(" --- Reading flags ---\n");
	printf("nflag	= %i	- Creating NDfield file\n",nflag);
	printf("cflag	= %i	- Creating column data text file\n",cflag);
	printf("dflag	= %i	- Create density plots\n",dflag);
	printf("rflag	= %i	- Create mass-radius plots\n",rflag);
	printf("iflag	= %i	- Calculate moment of inertia tensor\n",iflag);
*/
	if(errflag)
    	{
		fprintf(stderr, "Usage : %s", usage);
		exit(2);
    	}
}


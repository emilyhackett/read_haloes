#include "readSkel.h"

#define EXAMPLE		0	// Basically a concept check - no output files wanted

#define	ASCII		0	// Output to ASCII format

#define PLOTNODES	0	// Output node pos to file and save 2D list plot to .ps file
#define PLOTSEGSONLY	1
#define PLOTSEGDENSITY	0	// Output seg pos to file and save 2D list plot to .ps file

#define PLOTNODEFIELDS	0	// Ouptut all node field data to .dat file
#define PLOTSEGFIELDS	0	// Output all seg field data to .dat file

#define NODEDATA	0	// Output all node data (position and field values) to .dat file

#define FILAMENT	0	// Find the filament for a section

////////////////////	MAIN FUNCTION TO TAKE IN FILE AS COMMAND ARGUMENT ////////////////////

int main(int argc, char *argv[])
{
	
	// DEFINE CENTRE OF MASS
	float *CoM=malloc(sizeof(float)*3);
	CoM[0]=65.078*10/128;
	CoM[1]=64.630*10/128;
	CoM[2]=64.664*10/128;

	char *name=malloc(sizeof(char)*100);
	sprintf(name,"%s",argv[1]);

	char *sklfile=malloc(sizeof(char)*100);
	char *asciifile=malloc(sizeof(char)*100);
	char *datfile=malloc(sizeof(char)*100);
	char *plotfile=malloc(sizeof(char)*100);
	char *denfile=malloc(sizeof(char)*100);
	
	// Define NDskeleton
	NDskel	*skl;
	float min;
	float max;
	
	if(argc<4)
	{
		fprintf(stderr,"Usage: %s file_label z_min z_max\n",argv[0]);
		if(PLOTSEGSONLY) printf("PLOTTING SEGMENTS ONLY\n");
		if(PLOTSEGDENSITY) printf("PLOTTING SEGMENTS ON DENSITY BACKGROUND\n");
		if(ASCII) printf("OUTPUTTING TO ASCII FORMAT\n");
		if(FILAMENT) printf("CALCULATING OVERALL FILAMENT FOR DIF. RADII\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("\n	---------- READING .NDskl FILE ----------\n");
		sprintf(sklfile,"%s.NDskl",name);
		skl=readNDskeleton(sklfile);
		min=atof(argv[2]);
		max=atof(argv[3]);
	}

	// Check sanity of skeleton (e.g. carry over contamination from .ND files)
	NDskelCheckSanity(skl,0);

	// Save ASCII skeleton if filename passed to function
	if(ASCII && !EXAMPLE)
	{
		printf("	---------- SAVING ASCII SKELETON ----------\n");
		sprintf(asciifile,"%s-ASCII.txt",name);
		Save_ASCIIskel(skl,asciifile);
	}	

	if(PLOTNODES && !EXAMPLE)
	{
		printf("	---------- PLOTTING NODE POSITIONS ----------\n");
		sprintf(datfile,"%s-nodepos.dat",name);
		sprintf(plotfile,"%s-nodeplot.ps",name);
		sprintf(denfile,"%s-xygrid.dat",name);
		PlotNodePos(skl,datfile,plotfile,denfile,min,max,0);
	}
	
	if(PLOTSEGSONLY && !EXAMPLE)
	{
		printf("	---------- PLOTTING SEGMENT POSITIONS ----------\n");
		sprintf(datfile,"%s_%.2f_%.2f-segpos.dat",name,min,max);
		sprintf(plotfile,"%s_%.2f_%.2f-segplot.ps",name,min,max);
		PlotSegOnly(skl,datfile,plotfile,min,max,0);
	}	
	

	if(PLOTSEGDENSITY && !EXAMPLE && !PLOTSEGSONLY)
	{
		printf("	---------- PLOTTING SEGMENT POSITIONS ----------\n");
		sprintf(datfile,"%s_%.2f_%.2f-segpos.dat",name,min,max);
		sprintf(plotfile,"%s_%.2f_%.2f-segplot.ps",name,min,max);
		sprintf(denfile,"%s_%.2f_%.2f-xygrid.dat",name,min,max);
		PlotSegPos(skl,datfile,plotfile,denfile,min,max,0);
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
	
	if(EXAMPLE)
	{
		printf("	---------- CONCEPT CHECK RUN - EXAMPLE FLAGGED ----------\n");
		ExampleSegment(skl,10);
		ExampleNode(skl,10);
	}

	if(FILAMENT)
	{
		FILE *fp=fopen("FIL.dat","w");

		float max_radius=1;
		printf("	---------- FINDING FILAMENT INSIDE RADIUS  ----------\n");
		int counter=0;

		float xtot=0;
		float ytot=0;
		float ztot=0;

		while(max_radius<=10)
		{
			max_radius=max_radius+0.15625;
			float *fil=malloc(sizeof(float)*4);
			fil=SkelFilament(skl,max_radius,CoM);
			printf("Filament in radius %.2f is:\n	{%.4f,%.4f,%.4f}\n",max_radius,fil[0],fil[1],fil[2]);

			xtot=xtot+fil[0];
			ytot=ytot+fil[1];
			ztot=ztot+fil[2];

			fprintf(fp,"%.2f	%.4f	%.4f	%.4f\n",max_radius,fil[0],fil[1],fil[2]);
			counter++;
		}

		float mag=sqrt(xtot*xtot+ytot*ytot+ztot*ztot);
		float xave=xtot/mag;
		float yave=ytot/mag;
		float zave=ztot/mag;
	
		printf("\nNumber of radi positions used is %i\n",counter);
		printf("Average filament =\n	{%0.4f,%0.4f,%0.4f}\n",xave,yave,zave);

/*		FILE *line=fopen("FIL_line2Mpc.dat","w");
		float *fil=malloc(sizeof(float)*4);
		fil=SkelFilament(skl,2,CoM);
		printf("Filament in radius %.2f is:\n	{%.2f,%.2f,%.2f}\n",max_radius,fil[0],fil[1],fil[2]);
		fprintf(line,"5	2	%.4f	%.4f	%.4f	1\n",fil[0],fil[1],fil[2]);
		fclose(line);
*/
	//	ListSegPosRADIUS(skl,"SegPosRADIUS.dat",10,CoM);

		fclose(fp);
	}		

	printf("	---------- END OF PROGRAM REACHED ----------\n\n");

	return 0;
}

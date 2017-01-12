#include "readSkel.h"


////////////////////	SAVE ASCII FORMAT OF SKELETON (VISUALISATION?)	////////////////////

// Save plot of node positions
void PlotNodes(NDskel *skl,char *filename,char *fileout)
{
	// Define temporary file for gnuplot commands
	FILE *tmp;
	tmp=fopen("temp","w");

	// Define  file for critical points
	FILE *fp;
	fp=fopen(filename,"w");
	printf("File %s opened for critical point data write\n",filename);

	// Define counter for x and y position values
	int x=0;
	int y=0;

		
	// Index over all nodes and turn long list into 2D array of coordinates
	for(int i=0;i<skl->nnodes*skl->ndims;i++)
	{
		if((i%2)==0)
		{
			fprintf(fp,"%f	",skl->segpos[i]);
			x++;
		}
		if((i%2)==1)
		{
			fprintf(fp,"%f\n",skl->segpos[i]);
			y++;
		}
	}
	
	printf("%i x positions read, %i y positions read to file %s in form\n",x,y,filename);
	printf(" x	y\n");
	fclose(fp);
	printf("File %s closed\n",filename);

	// Define gnuplot commands
	printf(" --- GNUplot COMMANDS ---\n");
	
	// Initial conditions
	char *settings="set size square 1,1\nset tmargin 2\nset view map\nset ticslevel 0\nset palette rgb 36,35,34\n";	
	fputs(settings,tmp);
	printf("%s",settings);
	
	// Outputs
	char output[BUFSIZ];
	sprintf(output,"set term postscript color\nset output '%s'\n",fileout);
	fputs(output,tmp);
	printf("%s",output);

	// Plot commands 
	char title[BUFSIZ];
	sprintf(title,"set title 'Point plot for skeleton nodes from file %s with xy values'\n",filename);
	fputs(title,tmp);
	printf("%s",title);

	char plotcom[BUFSIZ];
	sprintf(plotcom,"plot '%s' using 1:2 with points pointsize 1 pointtype 7\n",filename);
	fputs(plotcom,tmp);	
	printf("%s",plotcom);

	fclose(tmp);
	system("gnuplot -p 'temp'");
//	remove("temp");

	printf("Node point plots saved to file %s\n",fileout);

}





////////////////////	SAVE ASCII FORMAT OF SKELETON (VISUALISATION?)	////////////////////

// Save plot of node positions
void PlotNodes(NDskel *skl,char *file)
{
	// Define temporary file for gnuplot commands
	FILE *tmp;
	tmp=fopen("temp","w");

	// Define  file for critical points
	FILE *fp;
	fp=fopen("nodepos","w");
	printf("File nodepos opened for critical point data write\n");

	// Index over all nodes and turn long list into 2D array of coordinates
	for(int i=0;i<skl->nnodes*skl->ndims;i++)
	{
		if((i%2)==1)
		{
			fprintf(fp,"%d	",skl->segpos[i]);
		}
		else if((i%2)==0)
		{
			fprintf(fp,"%d\n",skl->segpos[i]);
		}
	}
	
	fclose(fp);
	printf("File closed\n");
}





////////////////////	SAVE ASCII FORMAT OF SKELETON (VISUALISATION?)	////////////////////

// Save plot of node positions
void PlotNodes(NDskel *skl,char *file)
{
	// Define temporary file for gnuplot commands
	FILE *tmp;
	tmp=fopen("temp","w");

	// Define  file for critical points
	FILE *fp;
	fp=fopen("CP_data","w");
	printf("File CP_data opened for critical point data write\n");

	// Index over all segments and write position to file
	for(int i=0;i<skl->nsegs*2*skl->ndims;i++)
	{
		fprintf(fp,"%d",
	



////////////////////	SAVE ASCII FORMAT OF SKELETON (VISUALISATION?)	////////////////////

// Save plot of node positions
void PlotNodes(NDskel *skl,char *file)
{
	// Define temporary file for gnuplot commands
	FILE *tmp;
	tmp=fopen("temp","w");

	// Define temporary file for critical points
	FILE *fp;




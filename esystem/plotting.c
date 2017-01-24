#include "esys.h"

////////////////	PLOTTING RADIUS V. SHAPE DATA	//////////////////

void plot_evalues(char *datfile, char *plotfile)
{
	FILE *fp=fopen("temp","w");

	if(LONG)	printf(" --- GNUplot COMMANDS ---\n");
	
	char output[BUFSIZ];
	sprintf(output,"set term postscript color\nset output '%s'\n",plotfile);
	fputs(output,fp);
	if(LONG)	printf("%s",output);

	char title[BUFSIZ];
	char plot[BUFSIZ];
	
	// FIRST PLOT - RADIUS AGAINST SPHERICITY
	sprintf(title,"set title 'Radius v. sphericity from file %s'\n",datfile);
	fputs(title,fp);
	if(LONG)	printf("%s",title);
	sprintf(plot,"plot '%s' using 1:5 with lines notitle\n",datfile);
	fputs(plot,fp);
	if(LONG)	printf("%s",plot);

	// SECOND PLOT - RADIUS AGAINST TRIAXIALITY
	sprintf(title,"set title 'Radius v. triaxiality from file %s'\n",datfile);
	fputs(title,fp);
	if(LONG)	printf("%s",title);
	sprintf(plot,"plot '%s' using 1:6 with lines notitle\n",datfile);
	fputs(plot,fp);
	if(LONG)	printf("%s",plot);

	// THIRD PLOT - RADIUS AGAINST 3 ELLIPTICITIES
	sprintf(title,"set title 'Radius v. Ellipticity from file %s'\n",datfile);
	fputs(title,fp);
	if(LONG)	printf("%s",title);
	sprintf(plot,"plot '%s' using 1:7 with lines notitle, '%s' using 1:8 with lines notitle, '%s' using 1:9 with lines notitle\n",datfile,datfile,datfile);
	fputs(plot,fp);
	if(LONG)	printf("%s",plot);

	// FOURTH PLOT - RADIUS AGAINST 3 EIGENVALUES
	sprintf(title,"set title 'Radius v. Eigenvalues from file %s'\n",datfile);
	fputs(title,fp);
	if(LONG)	printf("%s",title);
	sprintf(plot,"plot '%s' using 1:2 with lines notitle, '%s' using 1:3 with lines notitle, '%s' using 1:4 with lines notitle\n",datfile,datfile,datfile);
	fputs(plot,fp);
	if(LONG)	printf("%s",plot);

	fclose(fp);

	system("gnuplot -p 'temp'");

	remove("temp");
	
	printf("Shape-radius plots saved to file %s\n",plotfile);
}

////////////////	PLOTTING ELLIPSES AGAINST DENSITY BACKGROUND	//////////////////

void plot_ellipses(char *datfile, char *plotfile, char *denfile, float *CoM, double *esys)
{
	FILE *fp=fopen("temp","w");

	if(LONG)	printf(" --- GNUplot COMMANDS ---\n");

	char *settings="set size square 1,1\nset tmargin 2\nset view map\nset ticslevel 0\nset palette rgb 36,35,34\nset xrange [:128]\nset yrange [:128]\n";	
	fputs(settings,fp);
	if(LONG)	printf("%s",settings);

	char output[BUFSIZ];
	sprintf(output,"set term postscript color\nset output '%s'\n",plotfile);
	fputs(output,fp);
	if(LONG)	printf("%s",output);

	int i;
	int j=1;
	for(i=max_radius;i>0;i=i-10)
	{
		char ellipse[BUFSIZ];
		sprintf(ellipse,"set object %i ellipse center %.4f, %.4f size %.4f,%.4f angle %.2f units xy front fillstyle empty\n",j,CoM[0],CoM[1],esys[0]*2*i,esys[1]*2*i,axis_angle(esys));
		fputs(ellipse,fp);
		if(LONG)	printf("%s",ellipse);
		j++;
	}

	char title[BUFSIZ];
	char plot[BUFSIZ];
	
	sprintf(title,"set title 'E-vector ellipse at radius %i'\n",max_radius);
	fputs(title,fp);
	if(LONG)	printf("%s",title);

	sprintf(plot,"splot '%s' using 1:2:4 with points palette ps 1 pt 7 notitle\n",denfile);
	fputs(plot,fp);
	if(LONG)	printf("%s",plot);

	fclose(fp);
	system("gnuplot -p 'temp'");
	remove("temp");

}


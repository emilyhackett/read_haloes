#include "esys.h"

////////////////	PLOTTING RADIUS V. SHAPE DATA	//////////////////

void plot_evalues(char *datfile, char *plotfile)
{
	FILE *fp=fopen("temp","w");

	if(LONG)	printf(" --- GNUplot COMMANDS ---\n");
	
	char output[BUFSIZ];
	sprintf(output,"set term postscript color\nset output '%s'\nset xlabel 'radius (Mpc)'\n",plotfile);
	fputs(output,fp);
	if(LONG)	printf("%s",output);

	char title[BUFSIZ];
	char plot[BUFSIZ];
	
	// FIRST PLOT - RADIUS AGAINST SPHERICITY
	sprintf(title,"set title 'Radius v. sphericity for temp 10Mpc'\n");
	fputs(title,fp);
	if(LONG)	printf("%s",title);
	sprintf(plot,"plot '%s' using ($1*10/128):5 with lines notitle\n",datfile);
	fputs(plot,fp);
	if(LONG)	printf("%s",plot);

	// SECOND PLOT - RADIUS AGAINST TRIAXIALITY
	sprintf(title,"set title 'Radius v. triaxiality for temp 10Mpc'\n");
	fputs(title,fp);
	if(LONG)	printf("%s",title);
	sprintf(plot,"plot '%s' using ($1*10/128):6 with lines notitle\n",datfile);
	fputs(plot,fp);
	if(LONG)	printf("%s",plot);

	// THIRD PLOT - RADIUS AGAINST 3 ELLIPTICITIES
	sprintf(title,"set title 'Radius v. Ellipticity for temp 10Mpc'\n");
	fputs(title,fp);
	if(LONG)	printf("%s",title);
	sprintf(plot,"plot '%s' using ($1*10/128):7 with lines notitle, '%s' using ($1*10/128):8 with lines notitle, '%s' using ($1*10/128):9 with lines notitle\n",datfile,datfile,datfile);
	fputs(plot,fp);
	if(LONG)	printf("%s",plot);

	// THIRD PLOT - RADIUS AGAINST 3 ELLIPTICITIES - ** LOGSCALE!! **
	sprintf(title,"set title 'Radius v. Ellipticity for temp 10Mpc log'\nset logscale y\n");
	fputs(title,fp);
	if(LONG)	printf("%s",title);
	sprintf(plot,"plot '%s' using ($1*10/128):7 with lines notitle, '%s' using ($1*10/128):8 with lines notitle, '%s' using ($1*10/128):9 with lines notitle\n",datfile,datfile,datfile);
	fputs(plot,fp);
	if(LONG)	printf("%s",plot);


	// FOURTH PLOT - RADIUS AGAINST 3 EIGENVALUES
	sprintf(title,"unset logscale y\nset title 'Radius v. Eigenvalues for temp 10Mpc'\n");
	fputs(title,fp);
	if(LONG)	printf("%s",title);
	sprintf(plot,"plot '%s' using ($1*10/128):2 with lines notitle, '%s' using ($1*10/128):3 with lines notitle, '%s' using ($1*10/128):4 with lines notitle\n",datfile,datfile,datfile);
	fputs(plot,fp);
	if(LONG)	printf("%s",plot);

	fclose(fp);

	system("gnuplot -p 'temp'");

	remove("temp");
	
	printf("Shape-radius plots saved to file %s\n",plotfile);
}

////////////////	PLOTTING ELLIPSES AGAINST DENSITY BACKGROUND	//////////////////

void plot_ellipses(char *datfile, char *plotfile, char *denfile, float *CoM,float ***GRID)
{
	FILE *fp=fopen("temp","w");

	if(LONG)	printf(" --- GNUplot COMMANDS ---\n");

	char *settings="set size square 1,1\nset tmargin 2\nset view map\nset ticslevel 0\nset palette rgb 36,35,34\nset xrange [:10]\nset yrange [:10]\nset xlabel 'radius (Mpc)'\n";	
	fputs(settings,fp);
	if(LONG)	printf("%s",settings);

	char output[BUFSIZ];
	sprintf(output,"set term postscript color\nset output '%s'\n",plotfile);
	fputs(output,fp);
	if(LONG)	printf("%s",output);

	int i;
	int j=1;
	for(i=60;i>0;i=i-20)
	{
		// Calculate eigenvalues for particular radius:
		double *inertia=malloc(6*sizeof(double));
		printf("radius/i = %i\n",i);
		inertia=reduced_inertia(GRID,CoM,i);	
		double *esys=malloc(4*field->ndims*sizeof(double));
		esys=eigensystem(inertia);

		char ellipse[BUFSIZ];
		sprintf(ellipse,"set object %i ellipse center (%.4f*10/128), (%.4f*10/128) size (%.4f*10/128),(%.4f*10/128) angle %.2f units xy front fillstyle border lt 4\n",j,CoM[0],CoM[1],esys[0]*2*i,esys[1]*2*i,axis_angle(esys));
		fputs(ellipse,fp);
		if(LONG)	printf("%s",ellipse);
		j++;
	}

	char title[BUFSIZ];
	char plot[BUFSIZ];
	
	sprintf(title,"set title 'E-vector ellipse at different radii for DM 10Mpc'\n");
	fputs(title,fp);
	if(LONG)	printf("%s",title);

	sprintf(plot,"splot '%s' using ($1*10/128):($2*10/128):4 with points palette ps 1 pt 7 notitle\n",denfile);
	fputs(plot,fp);
	if(LONG)	printf("%s",plot);

	fclose(fp);
	system("gnuplot -p 'temp'");
//	remove("temp");

}


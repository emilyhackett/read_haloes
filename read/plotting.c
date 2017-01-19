#include "read.h"

//////////////////////////	READING NDFIELD STRUCTURE INTO COLUMN DATA	//////////////////////////
void column_data(char *filename)
{
	FILE *fp;
	
	fp=fopen(filename,"w");
	if(fp==NULL)
	{
		printf("Error opening file %s for col data write\n",filename);
		exit(EXIT_FAILURE);
	}
	printf("File %s opened for column data write\n",filename);
	
	int x,y,z;
	int i=0;
	for (x=0;x<field->dims[0];x++)
	{
		for(y=0;y<field->dims[1];y++)
		{
			for(z=0;z<field->dims[2];z++)
			{
				fprintf(fp,"%i	%i	%i	%f	%f\n",x,y,z,field->val[i],log(field->val[i]));
				i++;
			}
		}
		fprintf(fp,"\n");
	}
	
	printf("Grid data written to file in form:\nx	y	z	field->val	log(val)\n");

	fclose(fp);
	printf("File %s closed\n",filename);
}


//////////////////////////	READING GRID INTO XY COLUMN DATA	//////////////////////////
void xy_data(char *filename,float ***GRID,float min,float max)
{
	FILE *fp;
	
	fp=fopen(filename,"w");
	if(fp==NULL)
	{
		printf("Error opening file %s for col data write\n",filename);
		exit(EXIT_FAILURE);
	}
	printf("File %s opened for column data write\n",filename);
	
	int x,y,z;

	for (x=0;x<field->dims[0];x++)
	{
		for(y=0;y<field->dims[1];y++)
		{
			float sum=0;
			for(z=0;z<field->dims[2];z++)
			{
				if(z>=(min*field->dims[2]) && z<=(max*field->dims[2]))
				{
					sum=sum+GRID[x][y][z];	
				}
						
			}
			if(log(sum) == -INFINITY)
			{
				fprintf(fp,"%i	%i	%f	-4\n",x,y,sum);	
			}
			else {
				fprintf(fp,"%i	%i	%f	%f\n",x,y,sum,log(sum));
			}
		}
	}
	
	printf("Grid data written to file in form:\nx	y	field->val	log(val)\n");

	fclose(fp);
	printf("File %s closed\n",filename);
}

//////////////////////////	READING GRID INTO XZ COLUMN DATA	//////////////////////////
void xz_data(char *filename,float ***GRID)
{
	FILE *fp;
	
	fp=fopen(filename,"w");
	if(fp==NULL)
	{
		printf("Error opening file %s for col data write\n",filename);
		exit(EXIT_FAILURE);
	}
	printf("File %s opened for column data write\n",filename);
	
	int x,y,z;
	for (x=0;x<field->dims[0];x++)
	{
		for(z=0;z<field->dims[2];z++)
		{
			float sum=0;
			for(y=0;y<field->dims[1];y++)
			{
				sum=sum+GRID[x][y][z];				
			}
			
			fprintf(fp,"%i	%i	%f	%f\n",x,z,sum,log(sum));
		}
	}
	
	printf("Grid data written to file in form:\nx	z	field->val	log(val)\n");
	fclose(fp);
	printf("File %s closed\n",filename);
}

//////////////////////////	READING GRID INTO YZ COLUMN DATA	//////////////////////////
void yz_data(char *filename,float ***GRID)
{
	FILE *fp;
	
	fp=fopen(filename,"w");
	if(fp==NULL)
	{
		printf("Error opening file %s for col data write\n",filename);
		exit(EXIT_FAILURE);
	}
	printf("File %s opened for column data write\n",filename);
	
	int x,y,z;
	for (y=0;y<field->dims[1];y++)
	{
		for(z=0;z<field->dims[2];z++)
		{
			float sum=0;
			for(x=0;x<field->dims[0];x++)
			{
				sum=sum+GRID[x][y][z];				
			}
			
			fprintf(fp,"%i	%i	%f	%f\n",y,z,sum,log(sum));
		}
	}
	
	printf("Grid data written to file in form:\ny	z	field->val	log(val)\n");
	fclose(fp);
	printf("File %s closed\n",filename);
}

//////////////////////////	PLOTTING DENSITY DATA	//////////////////////////

void density_plots(char *fileout,char *xyfile,char *xzfile,char *yzfile,float min,float max)
{
	// Temporary file for gnuplot commands
	FILE *fp=fopen("temp","w");
	
	if(LONG)	printf(" --- GNUplot COMMANDS ---\n");
	// Initial conditions
	char *settings="set size square 1,1\nset tmargin 2\nset view map\nset ticslevel 0\nset palette rgb 36,35,34\n";	
	fputs(settings,fp);
	if(LONG)	printf("%s",settings);

	// X, Y and Z ranges
	char ranges[BUFSIZ];
	sprintf(ranges,"set xrange [:%i]\nset yrange [:%i]\n",field->dims[0],field->dims[2]);
	fputs(ranges,fp);
	if(LONG)	printf("%s",ranges);

	// Outputs
	char output[BUFSIZ];
	sprintf(output,"set term postscript color\nset output '%s'\n",fileout);
	fputs(output,fp);
	if(LONG)	printf("%s",output);

	//// LOG ////

	// Plot commands for XY plot
	char title[BUFSIZ];
	sprintf(title,"set title 'Density log plot from file %s with xy values in z range [%.2f,%.2f]'\n",xyfile,min,max);
	fputs(title,fp);
	if(LONG)	printf("%s",title);
	
	char plotcom[BUFSIZ];
	sprintf(plotcom,"splot '%s' using 1:2:4 with points palette pointsize 1 pointtype 7\n",xyfile);
	fputs(plotcom,fp);	
	if(LONG)	printf("%s",plotcom);

	/*	IGNORING EVERYTHING OTHER THAN XY LOG PLOT

	// Plot commands for XZ plot
	sprintf(title,"set title 'Density log plot from file %s with xz values'\n",xzfile);
	fputs(title,fp);
	printf("%s",title);
	
	sprintf(plotcom,"splot '%s' using 1:2:4 with points palette pointsize 1 pointtype 7\n",xzfile);
	fputs(plotcom,fp);
	printf("%s",plotcom);

	// Plot commands for YZ plot
	sprintf(title,"set title 'Density log plot from file %s with yz values'\n",yzfile);
	fputs(title,fp);
	printf("%s",title);

	sprintf(plotcom,"splot '%s' using 1:2:4 with points palette pointsize 1 pointtype 7\n",yzfile);
	fputs(plotcom,fp);
	printf("%s",plotcom);

	//// NORMAL ////

	// Plot commands for XY plot
	sprintf(title,"set title 'Density plot from file %s with xy values'\n",xyfile);
	fputs(title,fp);
	printf("%s",title);
	
	sprintf(plotcom,"splot '%s' using 1:2:3 with points palette pointsize 1 pointtype 7\n",xyfile);
	fputs(plotcom,fp);	
	printf("%s",plotcom);

	// Plot commands for XZ plot
	sprintf(title,"set title 'Density plot from file %s with xz values'\n",xzfile);
	fputs(title,fp);
	printf("%s",title);
	
	sprintf(plotcom,"splot '%s' using 1:2:3 with points palette pointsize 1 pointtype 7\n",xzfile);
	fputs(plotcom,fp);
	printf("%s",plotcom);

	// Plot commands for YZ plot
	sprintf(title,"set title 'Density plot from file %s with yz values'\n",yzfile);
	fputs(title,fp);
	printf("%s",title);

	sprintf(plotcom,"splot '%s' using 1:2:3 with points palette pointsize 1 pointtype 7\n",yzfile);
	fputs(plotcom,fp);
	printf("%s",plotcom);

	*/

	fclose(fp);

	system("gnuplot -p 'temp'");

	// Remove temp file
	remove("temp");

	printf("Density plots saved to file %s\n",fileout);
}

//////////////////////////	BINNED HISTOGRAM OF MASS-RADIUS DATA	//////////////////////////
void bin_histogram(float ***GRID,float xCoM,float yCoM,float zCoM)
{
	int i;
	float **HIST;
	
	struct {
		float radius;
		float val;	
	} GRIDPOLAR[field->nval];
	
	int x=0;
	int y=0;
	int z=0;
	printf(" --- Converting to polar coordinates ---\n");
	for(i=0;i<field->nval;i++)
	{
		float posx=xCoM-x;
		float posy=yCoM-y;
		float posz=zCoM-z;
		GRIDPOLAR[i].radius=sqrt(posx*posx+posy*posy+posz*posz);
		GRIDPOLAR[i].val=GRID[x][y][z];
	}


}

//////////////////////////	PLOTTING HISTOGRAM OF MASS-RADIUS DATA	//////////////////////////

void mass_radius_plots(float ***GRID,float *CoM,char *filename,char *plotfile)
{	
	// Define radius
	int maxradius=field->dims[0]/2;
	float cumulative_mass[maxradius];

	int radius,x,y,z;

	for(radius=2;radius<maxradius+1;radius++)
	{
		float mass=0;
		for(x=0;x<field->dims[0];++x)
		{
			for(y=0;y<field->dims[1];++y)
			{
				for(z=0;z<field->dims[2];++z)
				{
					if(radcentre(x,y,z,CoM[0],CoM[1],CoM[2])<radius)
					{
						mass=mass+GRID[x][y][z];
						/* NOTE: worried about indexing - values for x,y,z are 							off to what grid is actually reading */
					}
				}
			}
		}
		cumulative_mass[radius]=mass;
		//printf("radius	= %i,	cumulative_mass	= %f\n",radius,cumulative_mass[radius]);	
	}

	float mass_shell[maxradius];
	for(radius=2;radius<maxradius+1;radius++)
	{
		mass_shell[radius]=cumulative_mass[radius]-cumulative_mass[radius-1];
		//printf("radius	- %i,	mass_shell	= %f\n",radius,mass_shell[radius]);
	}
	
	printf("\n --- Reading binned data to file for plotting ---\n");
	FILE *f=fopen(filename,"w");
	
	if(f==NULL) {
		printf("Error opening %s\n",filename);
		exit(EXIT_FAILURE);
	}
	printf("File %s opened for write\n",filename);

	for(radius=1;radius<maxradius+1;radius++)
	{
		fprintf(f,"%i	%.2f	%.2f\n",radius,cumulative_mass[radius],mass_shell[radius]);
	}
	printf("Cumulative mass and mass shell data written to %s\n",filename);
	fclose(f);	

	// Temporary file for gnuplot commands
	FILE *fp=fopen("temp","w");
	
	printf("\n --- GNUplot COMMANDS ---\n");

	// Initial conditions
	char *settings="set size square 1,1\nset ticslevel 0\n";
	fputs(settings,fp);
	printf("%s",settings);

	// X, Y and Z ranges
	char ranges[BUFSIZ];
	sprintf(ranges,"set xrange [:%i]\n",field->dims[0]/2);
	fputs(ranges,fp);
	printf("%s",ranges);

	// Outputs
	char output[BUFSIZ];
	sprintf(output,"set term postscript color\nset output '%s'\n",plotfile);
	fputs(output,fp);
	printf("%s",output);

	// First plot - cumulative data normal
	char title[BUFSIZ];
	sprintf(title,"set title 'Plot from file %s using cumulative radius data'\n",filename);
	fputs(title,fp);
	printf("%s",title);
	char plotcom[BUFSIZ];
	sprintf(plotcom,"plot '%s' using 1:2 with points pointsize 1 pointtype 7\n",filename);
	fputs(plotcom,fp);
	printf("%s",plotcom);

	// Second plot - cumulative data logscale
	sprintf(title,"set title 'Plot from file %s using cumulative radius data - logscale'\n",filename);
	fputs(title,fp);
	printf("%s",title);
	char *logscale="set logscale y\nreplot\n";
	fputs(logscale,fp);
	printf("%s",logscale);

	// Third plot - shell data normal
	char *unset="unset logscale y\n";
	fputs(unset,fp);
	printf("%s",unset);
	sprintf(title,"set title 'Plot from file %s using shell radius data'\n",filename);
	printf("%s",title);
	fputs(title,fp);
	sprintf(plotcom,"plot '%s' using 1:3 with points pointsize 1 pointtype 7\n",filename);
	printf("%s",plotcom);
	fputs(plotcom,fp);

	// Fourth plot - shell data logscale
	sprintf(title,"set title 'Plot from file %s using shell radius data - logscale'\n",filename);
	printf("%s",title);
	fputs(title,fp);
	fputs(logscale,fp);
	printf("%s",logscale);	

	fclose(fp);
	system("gnuplot -p 'temp'");

	remove("temp");	
}

//// END ////

#include "esys.h"

//////////////////////////	CALCULATING MOMENT OF INERTIA FROM A GRID	//////////////////////////

double *moment_of_inertia(float ***GRID,float *CoM,int radius)
{
	double *matrix=malloc(6*sizeof(double));
	
	double I[field->ndims+1][field->ndims+1];
	
	int xmid=(int)field->dims[0]/2;
	int ymid=(int)field->dims[1]/2;
	int zmid=(int)field->dims[2]/2;
//	printf("xmid	= %i, ymid	= %i, zmid	= %i\n",xmid,ymid,zmid);

	int x,y,z;
	for(x=(xmid-radius);x<(xmid+radius);x++)
	{
		for(y=(ymid-radius);y<(ymid+radius);y++)
		{
			for(z=(zmid-radius);z<(zmid+radius);z++)
			{
				I[1][1]=I[1][1]+(x-CoM[0])*(x-CoM[0])*GRID[x][y][z];
				I[1][2]=I[1][2]+(x-CoM[0])*(y-CoM[1])*GRID[x][y][z];
				I[1][3]=I[1][3]+(x-CoM[0])*(z-CoM[2])*GRID[x][y][z];
				I[2][1]=I[2][1]+(y-CoM[1])*(x-CoM[0])*GRID[x][y][z];
				I[2][2]=I[2][2]+(y-CoM[1])*(y-CoM[1])*GRID[x][y][z];
				I[2][3]=I[2][3]+(y-CoM[1])*(z-CoM[2])*GRID[x][y][z];
				I[3][1]=I[3][1]+(z-CoM[2])*(x-CoM[0])*GRID[x][y][z];
				I[3][2]=I[3][2]+(z-CoM[2])*(y-CoM[1])*GRID[x][y][z];
				I[3][3]=I[3][3]+(z-CoM[2])*(z-CoM[2])*GRID[x][y][z];		
			}
		}
	}
	
	if(LONG) {
	printf("I11	I12	I13	->	%.2f	%.2f	%.2f\n",I[1][1],I[1][2],I[1][3]);
	printf("I21	I22	I23	->	%.2f	%.2f	%.2f\n",I[2][1],I[2][2],I[2][3]);
	printf("I31	I32	I33	->	%.2f	%.2f	%.2f\n",I[3][1],I[3][2],I[3][3]);
	}

	matrix[0]=I[1][1];
	matrix[1]=I[2][2];
	matrix[2]=I[3][3];
	matrix[3]=I[2][1];
	matrix[4]=I[3][1];
	matrix[5]=I[3][2];

	if(LONG)	printf("Moment of inertia tensor passed to eigenvalues function ...\n");

	// ADD CHECK FOR SYMMETRY (?)

	return matrix;
}

//////////////////	CALCULATING MOMENT OF INERTIA FROM A GRID	///////////////////

double *reduced_inertia(float ***GRID,float *CoM,int radius)
{
	double *matrix=malloc(6*sizeof(double));
	
	double I[4][4];
	double Ired[4][4];
	
	int xmid=(int)field->dims[0]/2;
	int ymid=(int)field->dims[1]/2;
	int zmid=(int)field->dims[2]/2;
	
	double MASS=0;

	int x,y,z;
	for(x=(xmid-radius);x<(xmid+radius);x++)
	{
		for(y=(ymid-radius);y<(ymid+radius);y++)
		{
			for(z=(zmid-radius);z<(zmid+radius);z++)
			{	
				I[1][1]=I[1][1]+(x-CoM[0])*(x-CoM[0])*GRID[x][y][z]/
					((x-CoM[0])*(x-CoM[0])+(y-CoM[1])*(y-CoM[1])
					 +(z-CoM[2])*(z-CoM[2]));
				I[1][2]=I[1][2]+(x-CoM[0])*(y-CoM[1])*GRID[x][y][z]/
					((x-CoM[0])*(x-CoM[0])+(y-CoM[1])*(y-CoM[1])
					 +(z-CoM[2])*(z-CoM[2]));
				I[1][3]=I[1][3]+(x-CoM[0])*(z-CoM[2])*GRID[x][y][z]/
					((x-CoM[0])*(x-CoM[0])+(y-CoM[1])*(y-CoM[1])
					 +(z-CoM[2])*(z-CoM[2]));
			//	I[2][1]=I[2][1]+(y-CoM[1])*(x-CoM[0])*GRID[x][y][z]/
			//		((x-CoM[0])*(x-CoM[0])+(y-CoM[1])*(y-CoM[1])
			//		 +(z-CoM[2])*(z-CoM[2]));
				I[2][2]=I[2][2]+(y-CoM[1])*(y-CoM[1])*GRID[x][y][z]/
					((x-CoM[0])*(x-CoM[0])+(y-CoM[1])*(y-CoM[1])
					 +(z-CoM[2])*(z-CoM[2]));
				I[2][3]=I[2][3]+(y-CoM[1])*(z-CoM[2])*GRID[x][y][z]/
					((x-CoM[0])*(x-CoM[0])+(y-CoM[1])*(y-CoM[1])
					 +(z-CoM[2])*(z-CoM[2]));
			//	I[3][1]=I[3][1]+(z-CoM[2])*(x-CoM[0])*GRID[x][y][z]/
			//		((x-CoM[0])*(x-CoM[0])+(y-CoM[1])*(y-CoM[1])
			//		 +(z-CoM[2])*(z-CoM[2]));
			//	I[3][2]=I[3][2]+(z-CoM[2])*(y-CoM[1])*GRID[x][y][z]/
			//		((x-CoM[0])*(x-CoM[0])+(y-CoM[1])*(y-CoM[1])
			//		 +(z-CoM[2])*(z-CoM[2]));
				I[3][3]=I[3][3]+(z-CoM[2])*(z-CoM[2])*GRID[x][y][z]/
					((x-CoM[0])*(x-CoM[0])+(y-CoM[1])*(y-CoM[1])
					 +(z-CoM[2])*(z-CoM[2]));
				
				MASS=MASS+GRID[x][y][z];	// Total mass of simulation
			}
		}
	}

	Ired[1][1]=I[1][1]/MASS;
	Ired[1][2]=I[1][2]/MASS;
	Ired[1][3]=I[1][3]/MASS;
	Ired[2][2]=I[2][2]/MASS;
	Ired[2][3]=I[2][3]/MASS;
	Ired[3][3]=I[3][3]/MASS;

	if(LONG) {
	printf("Ired11	Ired12	Ired13	->	%.6f	%.6f	%.6f\n",Ired[1][1],Ired[1][2],Ired[1][3]);
	printf("Ired21	Ired22	Ired23	->	%.6f	%.6f	%.6f\n",Ired[1][2],Ired[2][2],Ired[2][3]);
	printf("Ired31	Ired32	Ired33	->	%.6f	%.6f	%.6f\n",Ired[1][3],Ired[2][3],Ired[3][3]);
	}

	matrix[0]=Ired[1][1];
	matrix[1]=Ired[2][2];
	matrix[2]=Ired[3][3];
	matrix[3]=Ired[1][2];
	matrix[4]=Ired[1][3];
	matrix[5]=Ired[2][3];

	return matrix;

}
//////////////////////////	PRINT OUT SPHERICITY ETC. FROM E'VALUES	//////////////////////////

double *evalue_characteristics(double *evalues)
{
	if(LONG)	printf("Determining physical characteristics from eigenvalues:\n");
	
	double l1=sqrt(fabs(-evalues[0]+evalues[1]+evalues[2]))/sqrt(2);
	double l2=sqrt(fabs(evalues[0]-evalues[1]+evalues[2]))/sqrt(2);
	double l3=sqrt(fabs(evalues[0]+evalues[1]-evalues[2]))/sqrt(2);

	if(LONG)	printf(" {a,b,c}	= {%.2f,%.2f,%.2f}\n",l1,l2,l3);

	// Define the sphericity, S of the halo:
	double S=l3/l1;
	if(LONG)	printf("Sphericity, S	= %.2f\n",S);

	// Define the triaxiality, T of the halo:
	double T=(l1*l1-l2*l2)/(l1*l1-l3*l3);
	if(LONG)	printf("Triaxiality, T	= %.2f\n",T);

	// Define the ellipticity, E of the halo:
	double E1=(1-l1/l2);
	if(LONG)	printf("Ellipticity, E1 = %.2f\n",E1);
	double E2=(1-l2/l3);
	if(LONG)	printf("Ellipticity, E2 = %.2f\n",E2);
	double E3=(1-l1/l3);
	if(LONG)	printf("Ellipticity, E3 = %.2f\n",E3);

	
	// NEW ELLIPTICITY DEFINITION:
	double a=sqrt(fabs(evalues[0]));
	double b=sqrt(fabs(evalues[1]));
	double c=sqrt(fabs(evalues[2]));
	E1=(a-b)/a;
	E2=0;
	E3=0;


	double *shape;
	shape=malloc(5*sizeof(double));

	shape[0]=S;
	shape[1]=T;
	shape[2]=E1;
	shape[3]=E2;
	shape[4]=E3;

	return shape;
}

//////////////////////////	REQUIRED FUNCTIONS FOR MEETING WED	//////////////////////////

void print_esys(char *OUTFILE1,char *OUTFILE2,float ***GRID,float *CoM)
{
	FILE *fp1=fopen(OUTFILE1,"w");
	FILE *fp2=fopen(OUTFILE2,"w");
	
//	fprintf(fp1,"1-rad	2-i11	3-i22	4-i33	5-i12	6-i13	7-i23	8-la	9-lb	10-lc	11-eax	12-eay	13-eaz	14-S	15-T	16-E\n");

	int radius;
	for(radius=1;radius<=max_radius;radius++)
	{
		// First column - RADIUS
		fprintf(fp1,"%i	",radius);
		
		double *i=malloc(6*sizeof(double));
		i=reduced_inertia(GRID,CoM,radius);
		
		// 2nd to 5th columns - INERTIA TENSOR
		fprintf(fp1,"%.6f	%.6f	%.6f	%.6f	%.6f	%.6f	",i[0],i[1],i[2],i[3],i[4],i[5]);

		double *esys=malloc(12*sizeof(double));
		esys=eigensystem(i);

		// 6th to 12th columns - EIGENVALUES AND FIRST E'VECTOR
		fprintf(fp1,"%.6f	%.6f	%.6f	%.6f	%.6f	%.6f	",esys[0],
				esys[1],esys[2],esys[3],esys[4],esys[5]);

		double *shape=malloc(5*sizeof(double));
		shape=evalue_characteristics(esys);

		// 13th to 16th columns - SPHERICITY, TRIAXIALITY, ELLIPTICITY
		fprintf(fp1,"%.6f	%.6f	%.6f\n",shape[0],shape[1],shape[2]);
	}

	for(radius=60;radius>0;radius=radius-20)
	{
		// First column - RADIUS
		fprintf(fp2,"%i	",radius);
		
		double *i=malloc(6*sizeof(double));
		i=reduced_inertia(GRID,CoM,radius);
		
		// 2nd to 7th columns - INERTIA TENSOR
		fprintf(fp2,"%.6f	%.6f	%.6f	%.6f	%.6f	%.6f	",i[0],i[1],i[2],i[3],i[4],i[5]);

		double *esys=malloc(12*sizeof(double));
		esys=eigensystem(i);

		// 8th to 13th columns - EIGENVALUES AND FIRST E'VECTOR
		fprintf(fp2,"%.6f	%.6f	%.6f	%.6f	%.6f	%.6f	",esys[0],
				esys[1],esys[2],esys[3],esys[4],esys[5]);

		double *shape=malloc(5*sizeof(double));
		shape=evalue_characteristics(esys);

		// 14th to 16th columns - SPHERICITY, TRIAXIALITY, ELLIPTICITY
		fprintf(fp2,"%.6f	%.6f	%.6f\n",shape[0],shape[1],shape[2]);
	}

	fclose(fp1);
	fclose(fp2);
}


//// END ////

#include "read.h"

//////////////////////////	CALCULATING MOMENT OF INERTIA FROM A GRID	//////////////////////////

double *moment_of_inertia(float ***GRID,float *CoM)
{
	double *matrix=malloc(6*sizeof(double));
	
	double I[field->ndims+1][field->ndims+1];

	for(int x=0;x<field->dims[0];x++)
	{
		for(int y=0;y<field->dims[1];y++)
		{
			for(int z=0;z<field->dims[2];z++)
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
	
	printf("I11	I12	I13	->	%.2f	%.2f	%.2f\n",I[1][1],I[1][2],I[1][3]);
	printf("I21	I22	I23	->	%.2f	%.2f	%.2f\n",I[2][1],I[2][2],I[2][3]);
	printf("I31	I32	I33	->	%.2f	%.2f	%.2f\n",I[3][1],I[3][2],I[3][3]);

	matrix[0]=I[1][1];
	matrix[1]=I[2][2];
	matrix[2]=I[3][3];
	matrix[3]=I[2][1];
	matrix[4]=I[3][1];
	matrix[5]=I[3][2];

	printf("Moment of inertia tensor passed to eigenvalues function ...\n");

	// ADD CHECK FOR SYMMETRY (?)

	return matrix;
}

//////////////////////////	CALCULATING E'VALUES FROM 3x3 SYMMETRIC	//////////////////////////

double *eigenvalues(double *matrix)
{
	double pi=3.141593;
	double *evalues=malloc(field->ndims*sizeof(double));

	
	// Define the trace of the matrix (sum of diagonal elements)
	double trace=matrix[0]+matrix[1]+matrix[2];
	printf("	trace	= %.2f\n",trace);
	double q=trace/3;
	//printf("	q	= %.2f\n",q);	

	// Define the following values:
	double p1=matrix[3]*matrix[3]+matrix[4]*matrix[4]+matrix[5]*matrix[5];
	//printf("	p1	= %.2f\n",p1);
	double p2=(matrix[0]-trace)*(matrix[0]-trace)+(matrix[1]-trace)*(matrix[1]-trace)+(matrix[2]-trace)*(matrix[2]-trace)+2*p1;
	//printf("	p2	= %.2f\n",p2);
	double p=sqrt(p2/6);
	//printf("	p	= %.2f\n",p);

	// Define simplified symmetric matrix in same style as matrix passed to function
	double *B=malloc(6*sizeof(double));
	double *I=malloc(6*sizeof(double));
	for(int i=0;i<3;i++)
	{
		I[i]=1;
	}
	for(int i=3;i<6;i++)
	{
		I[i]=0;
	}

	for(int i=0;i<6;i++)
	{
		B[i]=(1/p)*(matrix[i]-q*I[i]);
	}

	// Calculate determinant of matrix B
	double detB=B[0]*B[1]*B[2]+2*B[3]*B[5]*B[4]-B[4]*B[1]*B[4]-B[3]*B[3]*B[2]-B[0]*B[5]*B[5];
	printf("	detB	= %.8f\n",detB);

	double r=detB/2;
	//printf("	r	= %.2f\n",r);

	// Check for different r values
	double phi;
	if(r<= -1)
	{
		phi=pi/3;
	}
	else if(r >= 1)
	{
		phi=0;
	}
	else {
		phi=acos(r)/3;
	}
	//printf("	phi	= %.2f\n",phi);	


	double e1=q+2*p*cos(phi);
	printf("	e1	= %.2f\n",e1);
	
	double e3=q+2*p*cos(phi+(2*pi/3));
	printf("	e3	= %.2f\n",e3);

	double e2=3*q - e1-e3;
	printf("	e2	= %.2f\n",e2);
	
	evalues[0]=e1;
	evalues[1]=e2;
	evalues[2]=e3;
	
	qsort(evalues,3,sizeof(double),cmpfunc);
	printf("After sort, eigenvalues returned from function are:\n");
	printf("evalues[0]	= %.2f\n",evalues[0]);
	printf("evalues[1]	= %.2f\n",evalues[1]);
	printf("evalues[2]	= %.2f\n",evalues[2]);	

	return evalues;
}

//////////////////////////	PRINT OUT SPHERICITY ETC. FROM E'VALUES	//////////////////////////

void evalue_characteristics(double *evalues)
{

	printf("	Determining physical characteristics from eigenvalues:\n");
	
	double l1=evalues[0];
	double l2=evalues[1];
	double l3=evalues[2];
	
	// Define the sphericity, S of the halo:
	double S=l3/l1;
	printf("sphericity, S	= %.2f\n",S);


	// Define the triaxiality, T of the halo:
	double T=(l1*l1-l2*l2)/(l1*l1-l3*l3);
	printf("triaxiality, T	= %.2f\n",T);
}


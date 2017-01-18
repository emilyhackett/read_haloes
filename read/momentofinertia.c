#include "read.h"

double *eigensystem(double *I)
{
	double *esys;
	esys=malloc(12*sizeof(double));
	
	// This function will take the symmetric moment of inertia matrix in the 
	// simplified form defined previously and will output a list of floats:
	// e1, e2, e3, (v1x,v1y,v1z), (v2x,v2y,v2z), (v3x,v3y,v3z) 
	// corresponding to the values for the eigensystem as a whole

	double det;	// Define the 3x3 matrix determinant
	det=I[0]*(I[1]*I[2]-I[5]*I[5])
		-I[3]*(I[3]*I[2]-I[5]*I[4])
		+I[4]*(I[3]*I[5]-I[1]*I[4]);

	// Define the following values as the coefficients for the cubic equation
	// a x^3 + b x^2 + c x + d = 0

	double a,b,c,d;

	a=-1;
	
	b=I[0]+I[1]+I[2];
	
	c=-I[0]*I[1]-I[0]*I[2]-I[1]*I[2]+I[3]*I[3]+I[4]*I[4]+I[5]*I[5];

	d=I[0]*I[1]*I[2]-I[2]*I[3]*I[3]-I[1]*I[4]*I[4]
		+2*I[3]*I[4]*I[5]-I[0]*I[5]*I[5];

//	printf("a	= %.2f\nb	= %.2f\nc 	= %.2f\nd	= %.2f\n",a,b,c,d);
	
	b /= a;
	c /= a;
	d /= a;
	
	double disc,q,r,dum1,s,t,term1,r13,pi;

	q=(3.0*c-(b*b))/9.0;
	r=-(27.0*d)+b*(9.0*c-2.0*(b*b));
	r /= 54.0;
	disc=q*q*q + r*r;
	term1=(b/3.0);
	if(disc>0)	// One root real, two complex (ERROR!)
	{
		printf("ERROR - one root real, two complex\n");
		exit(EXIT_FAILURE);
	}

	if(disc == 0)	// All roots real, two equal (unlikely)
	{
		printf("ERROR - two real roots equal\n");
	}
	
	q=-q;
	dum1=q*q*q;
	dum1=acos(r/sqrt(dum1));
	r13=2.0*sqrt(q);
	pi=3.14159265;

	esys[0]=-term1+r13*cos(dum1/3.0);
	esys[1]=-term1+r13*cos((dum1+2.0*pi)/3.0);
	esys[2]=-term1+r13*cos((dum1+4.0*pi)/3.0);

	printf(" {e1,e2,e3}	= {%.2f,%.2f,%.2f}\n",esys[0],esys[1],esys[2]);

	// NOTE: Still need to define eigenvectors
	// Iterate over 3 eigenvalues and find solutions:

	return esys;
}

//////////////////////////	CALCULATING MOMENT OF INERTIA FROM A GRID	//////////////////////////

double *moment_of_inertia(float ***GRID,float *CoM)
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
	
//	printf("I11	I12	I13	->	%.2f		%.2f		%.2f\n",I[1][1],I[1][2],I[1][3]);
//	printf("I21	I22	I23	->	%.2f		%.2f		%.2f\n",I[2][1],I[2][2],I[2][3]);
//	printf("I31	I32	I33	->	%.2f		%.2f		%.2f\n",I[3][1],I[3][2],I[3][3]);

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

//////////////////////////	CALCULATING E'VALUES FROM 3x3 SYMMETRIC	//////////////////////////

double *eigenvalues(double *matrix)
{
	double pi=3.141593;
	double *evalues=malloc(field->ndims*sizeof(double));

	
	// Define the trace of the matrix (sum of diagonal elements)
	double trace=matrix[0]+matrix[1]+matrix[2];
	//printf("	trace	= %.2f\n",trace);
	double q=trace/3;
	//printf("	q	= %.2f\n",q);	

	// Define the following values:
	double p1=matrix[3]*matrix[3]+matrix[4]*matrix[4]+matrix[5]*matrix[5];
	//printf("	p1	= %.2f\n",p1);
	double p2=(matrix[0]-trace)*(matrix[0]-trace)+(matrix[1]-trace)*(matrix[1]-trace)+(matrix[2]-trace)*(matrix[2]-trace)+2*p1;
	//printf("	p2	= %.2f\n",p2);
	double p=sqrt(p2/6);
	//printf("	p	= %.2f\n",p);

	int i;
	// Define simplified symmetric matrix in same style as matrix passed to function
	double *B=malloc(6*sizeof(double));
	double *I=malloc(6*sizeof(double));
	for(i=0;i<3;i++)
	{
		I[i]=1;
	}
	for(i=3;i<6;i++)
	{
		I[i]=0;
	}

	for(i=0;i<6;i++)
	{
		B[i]=(1/p)*(matrix[i]-q*I[i]);
	}

	// Calculate determinant of matrix B
	double detB=B[0]*B[1]*B[2]+2*B[3]*B[5]*B[4]-B[4]*B[1]*B[4]-B[3]*B[3]*B[2]-B[0]*B[5]*B[5];
	//printf("	detB	= %.8f\n",detB);

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
	double e3=q+2*p*cos(phi+(2*pi/3));
	double e2=3*q - e1-e3;
	
	evalues[0]=e1;
	evalues[1]=e2;
	evalues[2]=e3;
	
	return evalues;
}

//////////////////////////	PRINT OUT SPHERICITY ETC. FROM E'VALUES	//////////////////////////

void evalue_characteristics(double *evalues)
{
	qsort(evalues,3,sizeof(double),cmpfunc);
	if(LONG)	printf("After sort, eigenvalues returned from function are:\n");
	if(LONG)	printf(" {%.2f,%.2f,%.2f}\n",evalues[0],evalues[1],evalues[2]);

	if(LONG)	printf("Determining physical characteristics from eigenvalues:\n");
	
	double l1=sqrt(fabs(-evalues[0]+evalues[1]+evalues[2]))/sqrt(2);
	double l2=sqrt(fabs(evalues[0]-evalues[1]+evalues[2]))/sqrt(2);
	double l3=sqrt(fabs(evalues[0]+evalues[1]-evalues[2]))/sqrt(2);

	printf(" {a,b,c}	= {%.2f,%.2f,%.2f}\n",l1,l2,l3);

	// Define the sphericity, S of the halo:
	double S=l3/l1;
	printf("Sphericity, S	= %.2f\n",S);


	// Define the triaxiality, T of the halo:
	double T=(l1*l1-l2*l2)/(l1*l1-l3*l3);
	printf("Triaxiality, T	= %.2f\n",T);

	// Define the ellipticity, E of the halo:
	double E1=(1-l1/l2);
	printf("Ellipticity, E1 = %.2f\n",E1);
	double E2=(1-l2/l3);
	printf("Ellipticity, E2 = %.2f\n",E2);
	double E3=(1-l1/l3);
	printf("Ellipticity, E3 = %.2f\n",E3);
}

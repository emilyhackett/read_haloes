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
	
	double I[field->ndims+1][field->ndims+1];
	double Ired[field->ndims+1][field->ndims+1];
	
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
	qsort(evalues,3,sizeof(double),cmpfunc);
	if(LONG)	printf("After sort, eigenvalues returned from function are:\n");
	if(LONG)	printf(" {%.2f,%.2f,%.2f}\n",evalues[0],evalues[1],evalues[2]);

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

	double *shape;
	shape=malloc(5*sizeof(double));

	shape[0]=S;
	shape[1]=T;
	shape[2]=E1;
	shape[3]=E2;
	shape[4]=E3;

	return shape;
}

//////////////////////////	DETERMINE EVALUES AND EVECTORS FROM TENSOR	//////////////////////////

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

	if(LONG)	printf(" {e1,e2,e3}	= {%.2f,%.2f,%.2f}\n",esys[0],esys[1],esys[2]);

	// NOTE: Still need to define eigenvectors
	// Iterate over 3 eigenvalues and find solutions:

	return esys;
}

//// END ////

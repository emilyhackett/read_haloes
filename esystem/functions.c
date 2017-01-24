#include "esys.h"

/////////	CREATES 3D GRID ARRAY - GRID[nx][ny][nz]	/////////
float ***create_grid(void)
{
	int nvals=0;
	float ***GRID;

	int x,y,z;
	int v=0;
	GRID=calloc(field->dims[0],sizeof(GRID));
	if(GRID != NULL)
	{
		for(x=0;x<field->dims[0];++x)
		{
			GRID[x]=calloc(field->dims[1],sizeof(GRID[0]));
			for(y=0;y<field->dims[1];++y)
			{
				GRID[x][y]=calloc(field->dims[2],sizeof(GRID[0][0]));
				for(z=0;z<field->dims[2];++z)
				{
					GRID[x][y][z]=field->val[v];
					v++;
					nvals++;
				}
			}
		}
	}

	if(LONG)	printf("Grid cleared and allocated for GRID[%i][%i][%i]\n",nx,ny,nz); 
	if(LONG)	printf("nvals read into grid are %i\n",nvals);
	
	return GRID;	
}

//////////////////////////	FINDING CENTRE OF MASS FROM GRID DATA	//////////////////////////

float *centre_of_mass(float ***GRID)
{
	int x,y,z;
	float *CoM=malloc(sizeof(float)*(field->ndims+1));

	///////// FINDING X Centre of Mass ////////
	float wsum_x=0.0;
	float gridtot_x=0.0;
	
	for(x=0;x<field->dims[0];++x)
	{
		float weight=0;
		for(y=0;y<field->dims[1];++y)
		{
			for(z=0;z<field->dims[2];++z)
			{
				weight=weight+GRID[x][y][z];
			}
		}
		gridtot_x=gridtot_x+weight;
		wsum_x=wsum_x+x*weight;
	}

	///////// FINDING Y Centre of Mass ////////

	float wsum_y=0.0;
	float gridtot_y=0.0;

	for(y=0;y<field->dims[1];++y)
	{
		float weight=0;
		for(x=0;x<field->dims[0];++x)
		{
			for(z=0;z<field->dims[2];++z)
			{
				weight=weight+GRID[x][y][z];
			}
		}
		gridtot_y=gridtot_y+weight;
		wsum_y=wsum_y+y*weight;
	}
	///////// FINDING Z Centre of Mass ////////
	float wsum_z=0.0;
	float gridtot_z=0.0;
	for(z=0;z<field->dims[2];++z)
	{
		float weight=0;
		for(x=0;x<field->dims[0];++x)
		{
			for(y=0;y<field->dims[1];++y)
			{
				weight=weight+GRID[x][y][z];
			}
		}
		gridtot_z=gridtot_z+weight;
		//printf("x	= %i,	weight	= %f,	gridtot_x	= %f\n",x,weight,gridtot_x);
		wsum_z=wsum_z+z*weight;
	}
	
	if(LONG)	printf(" m_ave	= {%.2f,%.2f,%.2f}\n",wsum_x/gridtot_x,wsum_y/gridtot_y,wsum_z/gridtot_z);

	// Defining centre of mass by taking correct average
	CoM[0]=wsum_x/gridtot_x;
	CoM[1]=wsum_y/gridtot_y;
	CoM[2]=wsum_z/gridtot_z;
	
	printf(" CoM		= {%.2f,%.2f,%.2f}\n",CoM[0],CoM[1],CoM[2]);
	
	return CoM;
}

///////// SMALL FUNCTION TO DEFINE RADIUS FROM CENTRE OF MASS	/////////
float radcentre(int x,int y,int z,float xCoM,float yCoM,float zCoM)
{
	float xdif=x-xCoM;
	float ydif=y-yCoM;
	float zdif=z-zCoM;
	
	double radiusSquare = xdif*xdif + ydif*ydif + zdif*zdif;
	float radius = sqrt(radiusSquare);	
	return radius;
}

///////// COMPARE FUNCTION TO SORT DOUBLES	/////////
int cmpfunc(const void *x,const void *y)
{
	double xx=*(double*)x;
	double yy=*(double*)y;
	if(xx<yy) return 1;
	if(xx>yy) return -1;
	return 0;
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
		exit(EXIT_FAILURE);
	}
	
	q=-q;
	dum1=q*q*q;
	dum1=acos(r/sqrt(dum1));
	r13=2.0*sqrt(q);
	pi=3.14159265;

	esys[0]=-term1+r13*cos(dum1/3.0);
	esys[1]=-term1+r13*cos((dum1+2.0*pi)/3.0);
	esys[2]=-term1+r13*cos((dum1+4.0*pi)/3.0);

	qsort(esys,3,sizeof(double),cmpfunc);

	if(LONG)	printf(" {e1,e2,e3}	= {%.2f,%.2f,%.2f}\n",esys[0],esys[1],esys[2]);

	// NOTE: Still need to define eigenvectors
	// Iterate over 3 eigenvalues and find solutions:

	int i;
	int j=3;
	for(i=0;i<=2;i++)
	{
		printf(" e%i:	{",i+1);
		float beta=-I[0]*I[1]+I[3]*I[3]+I[0]*esys[i]+I[1]*esys[i]-esys[i]*esys[i];
		float e1=(I[1]*I[4]-I[3]*I[5]-I[4]*esys[i])/beta;
		float e2=(I[3]*I[4]-I[0]*I[5]+I[5]*esys[i])/beta;
		float e3=1;
		
		// Find unit vectors
		esys[j]=e1/sqrt(e1*e1+e2*e2+e3*e3);
		printf("%.4f,",esys[j]);	
		j++;
		esys[j]=e2/sqrt(e1*e1+e2*e2+e3*e3);
		printf("%.4f,",esys[j]);
		j++;
		esys[j]=e3/sqrt(e1*e1+e2*e2+e3*e3);
		printf("%.4f}\n",esys[j]);
		j++;
	}

	return esys;
}

/////////	FINDS ANGLE BETWEEN MAJOR AND MINOR AXIS	/////////

float axis_angle(double *esys)
{
	float PI=3.14159;
	float angle;
	angle=acos(esys[3])*(180/PI);
	return angle;
}


//// END ////

#include "read.h"

//////////////////////////	CALCULATING MOMENT OF INERTIA FROM A GRID	//////////////////////////

double *moment_of_inertia(float ***GRID,float *CoM)
{
	double *evalues=malloc(field->ndims*sizeof(double));
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

	// Produce characteristic polynomial and solve for eigenvalues

/*	float lambda;

	(I[1][1]-lambda)
*/
	return evalues;
}

//////////////////////////	DETERMINANT OF A 3X3 MATRIX	//////////////////////////



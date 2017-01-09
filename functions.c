#include "read.h"

/////////	CREATES 3D GRID ARRAY - GRID[nx][ny][nz]	/////////
float ***create_grid(void)
{
	int nvals=0;
	float ***GRID;

	int v=0;
	GRID=calloc(field->dims[0],sizeof(GRID));
	if(GRID != NULL)
	{
		for(int x=0;x<field->dims[0];++x)
		{
			GRID[x]=calloc(field->dims[1],sizeof(GRID[0]));
			for(int y=0;y<field->dims[1];++y)
			{
				GRID[x][y]=calloc(field->dims[2],sizeof(GRID[0][0]));
				for(int z=0;z<field->dims[2];++z)
				{
					GRID[x][y][z]=field->val[v];
					v++;
					nvals++;
				}
			}
		}
	}

	printf("Grid cleared and allocated for GRID[%i][%i][%i]\n",nx,ny,nz); 
	printf("nvals read into grid are %i\n",nvals);
	
	return GRID;	
}

//////////////////////////	FINDING CENTRE OF MASS FROM GRID DATA	//////////////////////////

float *centre_of_mass(float ***GRID)
{

	float *CoM=malloc(sizeof(float)*(field->ndims+1));

	///////// FINDING X Centre of Mass ////////
	float wsum_x=0.0;
	float gridtot_x=0.0;
	
	for(int x=0;x<field->dims[0];++x)
	{
		float weight=0;
		for(int y=0;y<field->dims[1];++y)
		{
			for(int z=0;z<field->dims[2];++z)
			{
				weight=weight+GRID[x][y][z];
			}
		}
		gridtot_x=gridtot_x+weight;
		wsum_x=wsum_x+x*weight;
	}
	printf("wave_x		= %f\n",wsum_x/gridtot_x);

	///////// FINDING Y Centre of Mass ////////

	float wsum_y=0.0;
	float gridtot_y=0.0;

	for(int y=0;y<field->dims[1];++y)
	{
		float weight=0;
		for(int x=0;x<field->dims[0];++x)
		{
			for(int z=0;z<field->dims[2];++z)
			{
				weight=weight+GRID[x][y][z];
			}
		}
		gridtot_y=gridtot_y+weight;
		wsum_y=wsum_y+y*weight;
	}
	printf("wave_y		= %f\n",wsum_y/gridtot_y);

	///////// FINDING Z Centre of Mass ////////
	float wsum_z=0.0;
	float gridtot_z=0.0;
	for(int z=0;z<field->dims[2];++z)
	{
		float weight=0;
		for(int x=0;x<field->dims[0];++x)
		{
			for(int y=0;y<field->dims[1];++y)
			{
				weight=weight+GRID[x][y][z];
			}
		}
		gridtot_z=gridtot_z+weight;
		//printf("x	= %i,	weight	= %f,	gridtot_x	= %f\n",x,weight,gridtot_x);
		wsum_z=wsum_z+z*weight;
	}
	printf("wave_z		= %f\n",wsum_z/gridtot_z);

	// Defining centre of mass by taking correct average
	CoM[0]=wsum_x/gridtot_x;
	CoM[1]=wsum_y/gridtot_y;
	CoM[2]=wsum_z/gridtot_z;
	
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

//// END ////

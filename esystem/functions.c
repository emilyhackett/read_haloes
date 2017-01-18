#include "read.h"

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

//// END ////

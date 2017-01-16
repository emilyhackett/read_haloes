
double trace(double *matrix)
{
	double t;
	t=matrix[0]+matrix[1]+matrix[2];
	return t;
}

double twobytwos(double *matrix)
{
	double val;
	val=matrix[1]*matrix[2]
		+matrix[0]*matrix[2]
		+matrix[0]*matrix[1]
		-matrix[5]*matrix[5]
		-matrix[4]*matrix[4]
		-matrix[3]*matrix[3];
	return val;
}

double det(double *matrix)
{
	double d;
	d=matrix[0]*matrix[1]*matrix[2]
		+matrix[3]*matrix[5]*matrix[4]
		+matrix[4]*matrix[3]*matrix[5]
		-matrix[0]*matrix[5]*matrix[5]
		-matrix[3]*matrix[3]*matrix[2]
		-matrix[4]*matrix[1]*matrix[4];
	return d;
}

double *eigenvalues(double *matrix)
{
	/*
	double I11=matrix[0];
	double I22=matrix[1];
	double I33=matrix[2];
	double I21=matrix[3];
	double I31=matrix[4];
	double I32=matrix[5];
	*/

	// Coefficients of cubic equation for evalues
	double trace=trace(matrix);
	double beta=twobytwos(matrix);
	double det=det(matrix);

	// Define values for solving cubic equation
	double p=-trace/3;	
	double q=p*p*p+(trace*beta-3*det)/6;
	double r=beta/3;

	double x=cbrt(q+sqrt([q*q+(r-p*p)*(r-p*p)*(r-p*p)]))
		+cbrt(q-sqrt([q*q+(r-p*p)*(r-p*p)*(r-p*p)]))
		+p;

	printf("new x	= %.2f\n",x);

	return x;
}

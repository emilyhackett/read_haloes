
float *eigensystem(float *I)
{

	float *esys;
	esys=malloc(12*sizeof(float));
	
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

	
	float *evals;
	evals=malloc(3*sizeof(double));

	// Define some useful quantities to simplify the equations
	
	double beta,alpha,lambda;
	
	alpha=-b*b+3*a*c;
	beta=-2*b*b*b+9*a*b*c-27*a*a*d;
	lambda=4*beta*beta*beta+alpha*alpha;
	
	evals[0]=-b/3*a
		-(1.25992*alpha)/(3*a*cbrt(beta+sqrt(lambda)))
		+cbrt(beta+sqrt(lambda))/(3*1.25992*a);

	return esys;
}

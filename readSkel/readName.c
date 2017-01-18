#include "readSkel.h"

/*	Aim of this program is to read the name of the skeleton file
 *	from the command line arguments and get all required information
 *	about the particular cube
 */

NAME *readname(char *str)
{
	NAME *cube;
	cube=calloc(1,sizeof(NAME));

	char buffer[BUFSIZ];
	void *b=&buffer;
	
	while(str!=NULL)
	{
		b=str;
		str++;
		b++;
			
	

	p=strstr(filename,"smooth");
	if(p!=NULL)
	{
		printf("Skeleton smoothed\n");
	}
	
	return cube;
}

int main(int argc, char *argv[])
{
	char filename[50];
	sprintf(filename,"%s",argv[1]);
	printf("filename = %s\n",filename);

	NAME *cube;
	
	cube=readname(filename);

	return 0;
}

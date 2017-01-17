#include "readSkel.h"	

////////////////////	EXPORT ALL NODE DATA (POSITION AND FIELD) TO COLUMN TEXT	////////////////////

void NodeData(NDskel *skl,char *datafile)
{
	FILE *fp;
	fp=fopen(datafile,"w");
	printf("File %s opened for node data write\n",datafile);

	int pos=0;

	for(int i=0;i<skl->nnodes;i++)		// Index over all nodes (n lines in file)
	{
		for(int j=0;j<skl->ndims;j++)
		{
			fprintf(fp,"%f,",skl->nodepos[pos]);
			pos++;
		}
		  
	  	for (int k=0;k<skl->nnodedata;k++)
		{
			fprintf(fp,"%.7g,",skl->Node[i].data[k]);	// Field values to file
		}
	
		fprintf(fp,"\n");	// Print new line for each node
    	}
	
	fclose(fp);
}

////////////////////	EXPORT NODE POSITION DATA TO COLUMN TEXT	////////////////////

// Save column data list of node positions
void ListNodePos(NDskel *skl,char *datafile)
{
	if(skl->ndims!=2)
	{
		printf("ERROR: Can only plot 2D skeleton\n");
		exit(EXIT_FAILURE);
	}

	// Define  file for node positions
	FILE *fp;
	fp=fopen(datafile,"w");
	printf("File %s opened for critical point data write\n",datafile);

	// Define counter for x and y position values
	int x=0;
	int y=0;
	int z=0;
	
	// Index over all nodes and turn long list into 2D array of coordinates
	for(int i=1;i<=skl->nnodes*skl->ndims;i++)
	{
		if((i%3)==2)
		{
			fprintf(fp,"%f	",skl->nodepos[i]);
			x++;
		}
		if((i%3)==1)
		{
			fprintf(fp,"%f	",skl->nodepos[i]);
			y++;
		}
		if((i%3)==0)
		{
			fprintf(fp,"%f\n",skl->nodepos[i]);
			z++;
		}
	}
	
	printf("%i x positions, %i y positions, %i z positions read to file %s in form\n",x,y,z,datafile);
	printf(" x	y	z\n");
	fclose(fp);
	printf("File %s closed\n",datafile);
}

////////////////////	EXPORT SEG POSITION DATA TO COLUMN TEXT	////////////////////

void ListSegPos(NDskel *skl,char *datafile)
{
	// Save column data list of segment positions, aimed to be used 

	// Define  file for node positions
	FILE *fp;
	fp=fopen(datafile,"w");
	printf("File %s opened for critical point data write\n",datafile);

	// Define counter for x and y position values
	int x=0;
	int y=0;
	int z=0;
	 
	// Index over all segment start/end positions and write to two columns in file
	for(int i=1;i<=skl->nsegs*2*skl->ndims;i++)
	{
		if((i%3)==2)
		{
			fprintf(fp,"%f		",skl->segpos[i]);
			x++;
		}
		if((i%3)==1)
		{
			fprintf(fp,"%f	",skl->segpos[i]);
			y++;
		}
		if((i%3)==0)
		{
			fprintf(fp,"%f\n",skl->segpos[i]);
			z++;
		}
	}
	
	printf("%i x positions, %i y positions, %i z positions read to file %s in form\n",x,y,z,datafile);
	printf(" x	y	z\n");
	fclose(fp);
	printf("File %s closed\n",datafile);
}
////////////////////	PLOT NODE POSITIONS TO POSTSCRIPT FILE	////////////////////

void	PlotNodePos(NDskel *skl,char *datafile, char *plotfile, float min, float max, int print)
{
	// This function will take the datafile name as an argument, call the ListNodePos function 
	// to fill the datafile with values, then create a temporary gnuplot script to run.
	// Note: If print is non-zero, will print out the GNUplot commands to terminal, else
	// will simply save them to temp file then delete them

	ListNodePos(skl,datafile);

	// Define temporary file for gnuplot commands
	FILE *tmp;
	tmp=fopen("temp","w");
	
	if(print)	printf(" --- GNUplot COMMANDS ---\n");
	
	// Set outputs
	char output[BUFSIZ];
	sprintf(output,"set term postscript color\nset output '%s'\n",plotfile);
	fputs(output,tmp);
	if(print)	printf("%s",output);

	// Set box settings
	char settings[BUFSIZ];
	sprintf(settings,"set size square 1,1\nset tmargin 2\nset xrange [0:1]\nset yrange [0:1]\n");
	fputs(settings,tmp);
	if(print)	printf("%s",settings);

	// Plot commands
	char title[BUFSIZ];
	sprintf(title,"set title 'Point plot for skeleton nodes from file %s with xy values'\n",datafile);
	fputs(title,tmp);
	if(print)	printf("%s",title);
	
	char plotcom[BUFSIZ];
	sprintf(plotcom,"plot '%s' using 1:2 with points pointsize 1\n",datafile);
	fputs(plotcom,tmp);	
	if(print)	printf("%s",plotcom);

	// Close temp file before running commands through system
	fclose(tmp);
	system("gnuplot -p 'temp'");
	
	// Remove temp file - comment out for trouble shooting
	remove("temp");

	printf("%i node points plotted in %s\n",skl->nnodes,plotfile);
}

////////////////////	PLOT SEG POSITIONS TO POSTSCRIPT FILE	////////////////////

void	PlotSegPos(NDskel *skl,char *datafile, char *plotfile, float min, float max, int print)
{
	// This function will take the datafile name as an argument, call the ListSegPos function 
	// to fill the datafile with values, then create a temporary gnuplot script to run.
	// Note: If print is non-zero, will print out the GNUplot commands to terminal, else
	// will simply save them to temp file then delete them

	ListSegPos(skl,datafile);

	// Define temporary file for gnuplot commands
	FILE *tmp;
	tmp=fopen("temp","w");
	
	if(print)	printf(" --- GNUplot COMMANDS ---\n");
	
	// Set outputs
	char output[BUFSIZ];
	sprintf(output,"set term postscript color\nset output '%s'\n",plotfile);
	fputs(output,tmp);
	if(print)	printf("%s",output);

	// Set box settings
	char settings[BUFSIZ];
	sprintf(settings,"set size square 1,1\nset tmargin 2\nset xrange [0:1]\nset yrange [0:1]\n");
	fputs(settings,tmp);
	if(print)	printf("%s",settings);

	// Plot commands
	char title[BUFSIZ];
	sprintf(title,"set title 'Point plot for skeleton nodes from file %s with xy values'\n",datafile);
	fputs(title,tmp);
	if(print)	printf("%s",title);
	
	char plotcom[BUFSIZ];
	sprintf(plotcom,"plot '%s' using 1:(($3 > %f) ? $2:NaN) with points pointsize 1\n",datafile,min);
	fputs(plotcom,tmp);	
	if(print)	printf("%s",plotcom);

	// Close temp file before running commands through system
	fclose(tmp);
	system("gnuplot -p 'temp'");
	
	// Remove temp file - comment out for trouble shooting
	remove("temp");

	printf("%i seg points plotted in %s\n",skl->nsegs,plotfile);
}

////////////////////	WRITE COLUMN DATA FILE WITH NODE FIELD VALUES	////////////////////

void	ListNodeFieldVals(NDskel *skl,char *datafile)
{
	// Prints node field names at the start of the file and then 
	// all values for each node in columns in a txt file
	int i,j,l;

	FILE *fp;
	fp=fopen(datafile,"w");
	if(fp==NULL)
	{
		printf("ERROR opening %s file for node field data write\n",datafile);
		exit(EXIT_FAILURE);
	}

	int nodeDenId=getDataFieldID(skl,0,VALUE_TAG);
	int nodePairId=getDataFieldID(skl,0,PERSISTENCE_PAIR_TAG);

	long **filTab=NULL;
	int *filSegCount=NULL;
	NDskl_seg **filSegTab=NULL;

	int nfil=getNDskelFilTab(skl,&filSegTab,&filSegCount);

	filTab=(long**)calloc(skl->nnodes,sizeof(long*));
	for(i=0;i<skl->nnodes;i++)	// Index over all nodes in the skeleton
	{
		if(skl->Node[i].nnext)
		{
			filTab[i]=(long*)calloc(skl->Node[i].nnext,sizeof(long));
		}
		else {
			filTab[i]=NULL;
		}
	}

	NDskl_node *oldNode=NULL;
	NDskl_node *oldNext=NULL;
	int jNext=-1;
	int jNode=-1;
	
	for(i=0;i<nfil;i++)
	{
		NDskl_node *node=&skl->Node[filSegTab[i]->nodes[0]];
		NDskl_node *next=&skl->Node[filSegTab[i]->nodes[1]];
		
		if((node == oldNode) && (next==oldNext))
		{
			jNode++;
			jNext++;
		}
		else {
			jNode=0;
			jNext=0;
		}

		for(;jNode<node->nnext;jNode++)
		{
			if(node->Next[jNode]==next)	break;
		}

		if(jNode==node->nnext)
		{
			fprintf(stderr,"ERROR in saving skeleton - invalid file!\n");
		}
		filTab[node->index][jNode]=i;
			
		for(;jNext<next->nnext;jNext++)
		{
			if(next->Next[jNext]==node)	break;
		}

		if(jNext==next->nnext)
		{
			fprintf(stderr,"ERROR in saving skeleton - invalid file!\n");
		}
		filTab[next->index][jNext]=i;
		oldNode=node;
		oldNext=next;
	}

//  	for (i=0;i<skl->nnodedata;i++)	
//	    	fprintf(fp,"%s	",skl->nodedata_info[i]);	// Names of fields to file
//      	fprintf(fp,"\n");
	
	for (i=0;i<skl->nnodes;i++)
    	{	  
	  	for (j=0;j<skl->nnodedata-1;j++)
			fprintf(fp,"%.7g	",skl->Node[i].data[j]);	// Field values to file
	  	fprintf(fp,"%.7g\n",skl->Node[i].data[j]);			// Print new line for each node
    	}

	printf("Critical point data fields written to file %s in order:\n",datafile);
	
      	for (i=0;i<skl->nnodedata;i++)	
	    	printf("%s	",skl->nodedata_info[i]);	// Names of fields to screen
	printf("\n");


	fclose(fp);
}

////////////////////	WRITE COLUMN DATA FILE WITH SEGMENT FIELD VALUES	////////////////////

void	ListSegFieldVals(NDskel *skl,char *datafile)
{
	// Prints segment field names at the start of the file and then 
	// all values for each node in columns in a txt file
	int i,j,l;

	FILE *fp;
	fp=fopen(datafile,"w");
	if(fp==NULL)
	{
		printf("ERROR opening %s file for node field data write\n",datafile);
		exit(EXIT_FAILURE);
	}

	int nodeDenId=getDataFieldID(skl,0,VALUE_TAG);
	int nodePairId=getDataFieldID(skl,0,PERSISTENCE_PAIR_TAG);

	long **filTab=NULL;
	int *filSegCount=NULL;
	NDskl_seg **filSegTab=NULL;

	int nfil=getNDskelFilTab(skl,&filSegTab,&filSegCount);

	filTab=(long**)calloc(skl->nnodes,sizeof(long*));
	for(i=0;i<skl->nnodes;i++)	// Index over all nodes in the skeleton
	{
		if(skl->Node[i].nnext)
		{
			filTab[i]=(long*)calloc(skl->Node[i].nnext,sizeof(long));
		}
		else {
			filTab[i]=NULL;
		}
	}

	NDskl_node *oldNode=NULL;
	NDskl_node *oldNext=NULL;
	int jNext=-1;
	int jNode=-1;
	
	for(i=0;i<nfil;i++)
	{
		NDskl_node *node=&skl->Node[filSegTab[i]->nodes[0]];
		NDskl_node *next=&skl->Node[filSegTab[i]->nodes[1]];
		
		if((node == oldNode) && (next==oldNext))
		{
			jNode++;
			jNext++;
		}
		else {
			jNode=0;
			jNext=0;
		}

		for(;jNode<node->nnext;jNode++)
		{
			if(node->Next[jNode]==next)	break;
		}

		if(jNode==node->nnext)
		{
			fprintf(stderr,"ERROR in saving skeleton - invalid file!\n");
		}
		filTab[node->index][jNode]=i;
			
		for(;jNext<next->nnext;jNext++)
		{
			if(next->Next[jNext]==node)	break;
		}

		if(jNext==next->nnext)
		{
			fprintf(stderr,"ERROR in saving skeleton - invalid file!\n");
		}
		filTab[next->index][jNext]=i;
		oldNode=node;
		oldNext=next;
	}

	// Beginning of data section for filaments
	int pid[skl->nsegdata*2];
      	char tmp[skl->nsegdata][255];
      	for (i=0,j=0;i<skl->nsegdata;i++)
    	{
	  	char *w1=strstr(skl->segdata_info[i],"_p1");
	  	char *w2=strstr(skl->segdata_info[i],"_p2");
      
	  	if ((w1==NULL)&&(w2==NULL))
		{
	      		pid[2*j]=pid[2*j+1]=i;	  
	      		strcpy(tmp[j],skl->segdata_info[i]);
	      		j++;
		}
	  	else if (w1!=NULL)
		{	  
	      		strReplace(skl->segdata_info[i],tmp[j],"_p1","_p2");
	      		int id=NDskel_SegDataIndex(skl,tmp[j]);
	      		pid[2*j]=i;
	      		pid[2*j+1]=id;	  
	      		strcpy(tmp[j],skl->segdata_info[i]);
	      		w1=strstr(tmp[j],"_p1");
	      		(*w1)='\0';
	      		j++;
		}
	  	else continue;       
    	}

//      	for (i=0;i<skl->nsegdata;i++)	
//	    	fprintf(fp,"%s	",skl->segdata_info[i]);	// Names of fields to file
//	fprintf(fp,"\n");
	
	// Print field values for 2*nsegs to file
	int nsegdata=j;
      	for (i=0;i<nfil;i++)
    	{
	  	NDskl_seg *seg=filSegTab[i];
            
	  	for (l=0;l<nsegdata-1;l++)
			fprintf(fp,"%.7g	",seg->data[pid[2*l]]);
	  	fprintf(fp,"%.7g\n",seg->data[pid[2*l]]);
		
	  	if (seg->Next!=NULL)
		{	  
	      		do {	    
		    		seg=seg->Next;
		    		for (l=0;l<nsegdata-1;l++)
			  		fprintf(fp,"%.7g	",seg->data[pid[2*l]]);
		    		fprintf(fp,"%.7g\n",seg->data[pid[2*l]]);
	      		} while(seg->Next!=NULL);
		}

	  	for (l=0;l<nsegdata-1;l++)
			fprintf(fp,"%.7g	",seg->data[pid[2*l+1]]);
	  	fprintf(fp,"%.7g\n",seg->data[pid[2*l+1]]);
    	}

	printf("Filament data fields written to file %s in order:\n",datafile);
      	for (i=0;i<skl->nsegdata;i++)	
	    	printf("%s	",skl->segdata_info[i]);	// Names of fields to screen
	printf("\n");

	fclose(fp);	
}





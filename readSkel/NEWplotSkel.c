#include "readSkel.h"	

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

	float *xpos;
	xpos=malloc(sizeof(float)*skl->nnodes);
		if(xpos==NULL)
		{
			fprintf(stderr,"xpos malloc failed\n");
		}
	float *ypos;
	ypos=malloc(sizeof(float)*skl->nnodes);
		if(ypos==NULL)
		{
			fprintf(stderr,"ypos malloc failed\n");
		}

	// Index over all nodes and turn long list into 2D array of coordinates
	for(int i=1;i<=skl->nnodes*skl->ndims;i++)
	{
		if((i%2)==1)
		{
			fprintf(fp,"%f		",skl->nodepos[i]);
			x++;
		}
		if((i%2)==0)
		{
			fprintf(fp,"%f\n",skl->nodepos[i]);
			y++;
		}
	}
	
	printf("%i x positions read, %i y positions read to file %s in form\n",x,y,datafile);
	printf(" x	y\n");
	fclose(fp);
	printf("File %s closed\n",datafile);
}

////////////////////	EXPORT SEG POSITION DATA TO COLUMN TEXT	////////////////////

void ListSegPos(NDskel *skl,char *datafile)
{
	// Save column data list of segment positions, aimed to be used 

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

	float *xpos;
	xpos=malloc(sizeof(float)*skl->nsegs*2);
		if(xpos==NULL)
		{
			fprintf(stderr,"xpos malloc failed\n");
		}
	float *ypos;
	ypos=malloc(sizeof(float)*skl->nsegs*2);
		if(ypos==NULL)
		{
			fprintf(stderr,"ypos malloc failed\n");
		}

	// Index over all segment start/end positions and write to two columns in file
	for(int i=1;i<=skl->nsegs*2*skl->ndims;i++)
	{
		if((i%2)==1)
		{
			fprintf(fp,"%f		",skl->segpos[i]);
			x++;
		}
		if((i%2)==0)
		{
			fprintf(fp,"%f\n",skl->segpos[i]);
			y++;
		}
	}
	
	printf("%i x positions read, %i y positions read to file %s in form\n",x,y,datafile);
	printf(" x	y\n");
	fclose(fp);
	printf("File %s closed\n",datafile);
}
////////////////////	PLOT NODE POSITIONS TO POSTSCRIPT FILE	////////////////////

void	PlotSegPos(NDskel *skl,char *datafile, char *plotfile, int print)
{
	// This function will take the datafile name as an argument, call the ListSegPos function 
	// to fill the datafile with values, then create a temporary gnuplot script to run.
	// Note: If print is non-zero, will print out the GNUplot commands to terminal, else
	// will simply save them to temp file then delete them

	if(skl->ndims!=2)
	{
		printf("ERROR: Can only plot 2D skeleton\n");
		exit(EXIT_FAILURE);
	}

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

	// Plot commands
	char title[BUFSIZ];
	sprintf(title,"set title 'Point plot for skeleton nodes from file %s with xy values'\n",datafile);
	fputs(title,tmp);
	if(print)	printf("%s",title);
	
	char plotcom[BUFSIZ];
	sprintf(plotcom,"plot '%s' using 1:2 with points pointsize 1 pointtype 7\n",datafile);
	fputs(plotcom,tmp);	
	if(print)	printf("%s",plotcom);

	// Close temp file before running commands through system
	fclose(temp);
	system("gnuplot -p 'temp'");
	
	// Remove temp file - comment out for trouble shooting
	remove("temp");

	printf("%i node points plotted in %s\n",skl->nnodes,plotfile);
}

////////////////////	PLOT SEG POSITIONS TO POSTSCRIPT FILE	////////////////////

void	PlotSegPos(NDskel *skl,char *datafile, char *plotfile, int print)
{
	// This function will take the datafile name as an argument, call the ListSegPos function 
	// to fill the datafile with values, then create a temporary gnuplot script to run.
	// Note: If print is non-zero, will print out the GNUplot commands to terminal, else
	// will simply save them to temp file then delete them

	if(skl->ndims!=2)
	{
		printf("ERROR: Can only plot 2D skeleton\n");
		exit(EXIT_FAILURE);
	}

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

	// Plot commands
	char title[BUFSIZ];
	sprintf(title,"set title 'Point plot for skeleton nodes from file %s with xy values'\n",datafile);
	fputs(title,tmp);
	if(print)	printf("%s",title);
	
	char plotcom[BUFSIZ];
	sprintf(plotcom,"plot '%s' using 1:2 with points pointsize 1 pointtype 7\n",datafile);
	fputs(plotcom,tmp);	
	if(print)	printf("%s",plotcom);

	// Close temp file before running commands through system
	fclose(temp);
	system("gnuplot -p 'temp'");
	
	// Remove temp file - comment out for trouble shooting
	remove("temp");

	printf("%i node points plotted in %s\n",skl->nnodes,plotfile);
}

////////////////////	EXTRACT PLOTTING DATA FROM NDSKEL	////////////////////

void	MakePlotFile(NDskel *skl,char *datafile)
{
	int i,j,k;

	FILE *fp;
	fp=fopen(datafile,"w");
	if(fp==NULL)
	{
		printf("ERROR opening %s file for data write\n",datafile);
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
 
	// Index over all nodes and write to file
      	for (i=0;i<skl->nnodes;i++)
    	{
	  	NDskl_node *node=&(skl->Node[i]);
      
//	  	fprintf(fp,"%d",node->type);		// Print node type to file
		
//	  	for (j=0;j<skl->ndims;j++) fprintf(fp," %g",node->pos[j]);
//		fprintf(fp,"\n");

//	  	if (nodeDenId>=0) fprintf(fp," %g",node->data[nodeDenId]);	// Print 
//	  	else fprintf(fp," 0");

//	  	if (nodePairId>=0) fprintf(fp," %d",(int)(node->data[nodePairId]));
//	  	else fprintf(f," %d",(int)i);

	  	int boundary=0;
	  	if (node->flags&FLAG_NDNODE_INFINITE) boundary=3;
	  	else if (node->flags&FLAG_NDNODE_OUT) boundary=2;
	  	else if (node->flags&FLAG_NDNODE_BOUNDARY) boundary=1;
	  
//		fprintf(f," %d\n",boundary);
//	  	fprintf(f," %d\n",node->nnext);
      
//	  	for (j=0;j<node->nnext;j++)
//			fprintf(fp," %d %ld\n",node->Next[j]->index,(long)filTab[i][j]);
    	}

      	for (i=0;i<skl->nnodes;i++) free(filTab[i]);
      	free(filTab);

	// Print filament data to file
	 
       	for (i=0;i<nfil;i++)
	{
	  	NDskl_seg *seg=filSegTab[i];
	  	NDskl_node *node=&skl->Node[seg->nodes[0]];
	  	NDskl_node *next=&skl->Node[seg->nodes[1]];
     
		// Print to file index of CP1, index of CP2 and number of sampling points
//	  	fprintf(fp,"%ld %ld %ld\n",(long)node->index,(long)next->index,(long)filSegCount[i]+1);
	  	for (j=0;j<skl->ndims;j++) 
		{
			// Print the position of sampling point for each dimension
			fprintf(fp," %g",node->pos[j]);
		}
			fprintf(fp,"\n");
	  	if (seg->Next!=NULL)
		{
	      		seg=seg->Next;
	      		do {	    
		    		for (j=0;j<skl->ndims;j++) 
				{
			  		fprintf(fp," %g",seg->pos[j]);
				}
				fprintf(fp,"\n");
		    		seg=seg->Next;
	      		} while(seg!=NULL);
		}
	  	for (j=0;j<skl->ndims;j++) 
		{
			fprintf(fp," %g",next->pos[j]);
		}
			fprintf(fp,"\n");
    	}


	fclose(fp);

	printf("Critical points written to file %s\n",datafile);
}

////////////////////	WRITE COLUMN DATA FILE WITH NODE POSITIONS AND FIELD VALUES	////////////////////

void	NodeFieldVals(NDskel *skl,char *datafile)
{
	int i,j,l;

	FILE *fp;
	fp=fopen(datafile,"w");
	if(fp==NULL)
	{
		printf("ERROR opening %s file for data write\n",datafile);
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

/*	// Beginning of data section for critical points (nodes?)
//	fprintf(fp,"%d\n",skl->nnodedata);	// Number of fields associated to CP
//      	for (i=0;i<skl->nnodedata;i++)	
//	    	fprintf(fp,"%s\n",skl->nodedata_info[i]);	// Names of fields
      	for (i=0;i<skl->nnodes;i++)
    	{	  
	  	for (j=0;j<skl->nnodedata-1;j++)
			fprintf(fp,"%.7g	",skl->Node[i].data[j]);
	  	fprintf(fp,"%.7g\n",skl->Node[i].data[j]);
    	}

	printf("Critical point data fields written to file %s in order:\n",datafile);
	
      	for (i=0;i<skl->nnodedata;i++)	
	    	printf("%s	",skl->nodedata_info[i]);	// Names of fields
	printf("\n");
*/


	// Beginning of data section for filaments
	printf("Filament data fields written to file %s in order:\n",datafile);
	
      	for (i=0;i<skl->nsegdata;i++)	
	    	printf("%s	",skl->segdata_info[i]);	// Names of fields
	printf("\n");
	
	
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

	int nsegdata=j;

      	for (i=0;i<nfil;i++)
    	{
	  	NDskl_seg *seg=filSegTab[i];
            
	  	for (l=0;l<nsegdata-1;l++)
			fprintf(fp,"%.7g ",seg->data[pid[2*l]]);
	  	fprintf(fp,"%.7g\n",seg->data[pid[2*l]]);
		
	  	if (seg->Next!=NULL)
		{	  
	      		do {	    
		    		seg=seg->Next;
		    		for (l=0;l<nsegdata-1;l++)
			  		fprintf(fp,"%.7g ",seg->data[pid[2*l]]);
		    		fprintf(fp,"%.7g\n",seg->data[pid[2*l]]);
	      		} while(seg->Next!=NULL);
		}

	  	for (l=0;l<nsegdata-1;l++)
			fprintf(fp,"%.7g ",seg->data[pid[2*l+1]]);
	  	fprintf(fp,"%.7g\n",seg->data[pid[2*l+1]]);
    	}	
}





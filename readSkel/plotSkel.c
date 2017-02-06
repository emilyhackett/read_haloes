#include "readSkel.h"	

////////////////////	EXPORT SEG POSITION DATA TO COLUMN TEXT	////////////////////

void ListSegPos(NDskel *skl,char *datafile,float min,float max)
{
	// Save column data list of segment positions, aimed to be used 
	int i;

	// Define  file for node positions
	FILE *fp;
	fp=fopen(datafile,"w");
	printf("File %s opened for critical point data write\n",datafile);

	// Define counter for x and y position values
	int x1=0;
	int y1=0;
	int z1=0;

	int x2=0;
	int y2=0;
	int z2=0;

	float *x1pos;
	float *y1pos;
	float *z1pos;
	
	float *x2pos;
	float *y2pos;
	float *z2pos;

	x1pos=malloc(2*skl->nsegs*sizeof(float));
	y1pos=malloc(2*skl->nsegs*sizeof(float));
	z1pos=malloc(2*skl->nsegs*sizeof(float));
	
	x2pos=malloc(2*skl->nsegs*sizeof(float));
	y2pos=malloc(2*skl->nsegs*sizeof(float));
	z2pos=malloc(2*skl->nsegs*sizeof(float));
	 
	// Index over all segment start/end positions and write to two columns in file
	for(i=0;i<=skl->nsegs*2*skl->ndims;i++)
	{
		if((i%6)==5)
		{
			x1pos[x1]=skl->segpos[i];
			x1++;
		}
		if((i%6)==4)
		{
			y1pos[y1]=skl->segpos[i];
			y1++;
		}
		if((i%6)==3)
		{
			z1pos[z1]=skl->segpos[i];
			z1++;
		}
		if((i%6)==2)
		{
			x2pos[x2]=skl->segpos[i];
			x2++;
		}
		if((i%6)==1)
		{
			y2pos[y2]=skl->segpos[i];
			y2++;
		}
		if((i%6)==0)
		{
			z2pos[z2]=skl->segpos[i];
			z2++;
		}
	}
	
	for(i=0;i<=skl->nsegs;i++)
	{
		if(z1pos[i]>=min && z1pos[i]<=max && z2pos[i]>=min && z2pos[i]<=max)
		{
			fprintf(fp,"%f	%f	%f	",x1pos[i],y1pos[i],z1pos[i]);
			fprintf(fp,"%f	%f	%f	1\n",x2pos[i],y2pos[i],z2pos[i]);
		}
	}

	printf("%i x positions, %i y positions, %i z positions read to file %s in form\n",x1,y1,z1,datafile);
	printf(" x	y	z	1\n");
	fclose(fp);
	printf("File %s closed\n",datafile);
}

////////////////////	PLOT SEG POSITIONS TO POSTSCRIPT FILE	////////////////////

void	PlotSegOnly(NDskel *skl,char *datafile, char *plotfile, float min, float max, int print)
{
	// This function will take the datafile name as an argument, call the ListSegPos function 
	// to fill the datafile with values, then create a temporary gnuplot script to run.
	// Note: If print is non-zero, will print out the GNUplot commands to terminal, else
	// will simply save them to temp file then delete them

	ListSegPos(skl,datafile,min,max);

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
	sprintf(settings,"set size square 1,1\nset tmargin 2\nset xrange [0:1]\nset yrange [0:1]\nunset label\n");
	fputs(settings,tmp);
	if(print)	printf("%s",settings);

	// FIRST PLOT - Skeleton only
	char title[BUFSIZ];
	sprintf(title,"set title 'Skeleton segments from file %s with xy values for z in [%.2f,%.2f]'\n",datafile,min,max);
	fputs(title,tmp);
	if(print)	printf("%s",title);
	
	char plotcom[BUFSIZ];
	sprintf(plotcom,"plot '%s' using 1:2:($4-$1):($5-$2) with vectors nohead notitle\n",datafile);
	fputs(plotcom,tmp);	
	if(print)	printf("%s",plotcom);

	// Close temp file before running commands through system
	fclose(tmp);
	system("gnuplot -p 'temp'");
	
	// Remove temp file - comment out for trouble shooting
	remove("temp");

	printf("%i seg points plotted in %s\n",skl->nsegs,plotfile);
}

////////////////////	PLOT SEG POSITIONS TO POSTSCRIPT FILE	////////////////////

void	PlotSegPos(NDskel *skl,char *datafile, char *plotfile, char *denfile,float min, float max, int print)
{
	// This function will take the datafile name as an argument, call the ListSegPos function 
	// to fill the datafile with values, then create a temporary gnuplot script to run.
	// Note: If print is non-zero, will print out the GNUplot commands to terminal, else
	// will simply save them to temp file then delete them

	ListSegPos(skl,datafile,min,max);

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
	sprintf(settings,"set size square 1,1\nset tmargin 2\nset xrange [0:1]\nset yrange [0:1]\nunset label\n");
	fputs(settings,tmp);
	if(print)	printf("%s",settings);

	// FIRST PLOT - Skeleton only
	char title[BUFSIZ];
	sprintf(title,"set title 'Skeleton segments from file %s with xy values for z in [%.2f,%.2f]'\n",datafile,min,max);
	fputs(title,tmp);
	if(print)	printf("%s",title);
	
	char plotcom[BUFSIZ];
	sprintf(plotcom,"plot '%s' using 1:2:($4-$1):($5-$2) with vectors nohead notitle\n",datafile);
	fputs(plotcom,tmp);	
	if(print)	printf("%s",plotcom);

	// SECOND PLOT - Skeleton on top of density file
	sprintf(settings,"set view map\nset palette rgb 36,35,34\n");
	fputs(settings,tmp);
	if(print)	printf("%s",settings);
	
	sprintf(title,"set title 'Skeleton segments on density map xy values for z in [%.2f:%.2f]'\n",min,max);
	fputs(title,tmp);
	if(print)	printf("%s",title);
	
	sprintf(plotcom,"splot '%s' using ($1/128):($2/128):4 with points palette pointtype 7 notitle,'%s' using 1:2:7:($4-$1):($5-$2):7 with vectors nohead lc 5 notitle\n",denfile,datafile);
	fputs(plotcom,tmp);
	if(print)	printf("%s",plotcom);
	

	// Close temp file before running commands through system
	fclose(tmp);
	system("gnuplot -p 'temp'");
	
	// Remove temp file - comment out for trouble shooting
	remove("temp");

	printf("%i seg points plotted in %s\n",skl->nsegs,plotfile);
}

////////////////////	EXPORT ALL NODE DATA (POSITION AND FIELD) TO COLUMN TEXT	////////////////////

void NodeData(NDskel *skl,char *datafile)
{
	FILE *fp;
	fp=fopen(datafile,"w");
	printf("File %s opened for node data write\n",datafile);

	int pos=0;
	int i,j,k;

	for(i=0;i<skl->nnodes;i++)		// Index over all nodes (n lines in file)
	{
		for(j=0;j<skl->ndims;j++)
		{
			fprintf(fp,"%f,",skl->nodepos[pos]);
			pos++;
		}
		  
	  	for (k=0;k<skl->nnodedata;k++)
		{
			fprintf(fp,"%.7g,",skl->Node[i].data[k]);	// Field values to file
		}
	
		fprintf(fp,"\n");	// Print new line for each node
    	}
	
	fclose(fp);
}

////////////////////	EXPORT NODE POSITION DATA TO COLUMN TEXT	////////////////////

// Save column data list of node positions
void ListNodePos(NDskel *skl,char *datafile,float min,float max)
{
	int i;
	
	// Define  file for node positions
	FILE *fp;
	fp=fopen(datafile,"w");
	printf("File %s opened for critical point data write\n",datafile);

	// Define counter for x and y position values
	int x=0;
	int y=0;
	int z=0;

	float *xpos;
	float *ypos;
	float *zpos;

	xpos=malloc(skl->nnodes*sizeof(float));
	ypos=malloc(skl->nnodes*sizeof(float));
	zpos=malloc(skl->nnodes*sizeof(float));	
	
	// Index over all nodes and turn long list into 2D array of coordinates
	for(i=1;i<=skl->nnodes*skl->ndims;i++)
	{
		if((i%3)==2)
		{
			xpos[x]=skl->nodepos[i];
			x++;
		}
		if((i%3)==1)
		{
			ypos[y]=skl->nodepos[i];
			y++;
		}
		if((i%3)==0)
		{
			zpos[z]=skl->nodepos[i];
			z++;
		}
	}
	
	for(i=0;i<=skl->nnodes;i++)
	{
		if(zpos[i]>=min && zpos[i]<=max)
		{
			fprintf(fp,"%f	%f	%f	1\n",xpos[i],ypos[i],zpos[i]);
		}
	}
	
	printf("%i x positions, %i y positions, %i z positions read to file %s in form\n",x,y,z,datafile);
	printf(" x	y	z\n");
	fclose(fp);
	printf("File %s closed\n",datafile);
}

////////////////////	PLOT NODE POSITIONS TO POSTSCRIPT FILE	////////////////////

void	PlotNodePos(NDskel *skl,char *datafile, char *plotfile, char *denfile, float min, float max, int print)
{
	// This function will take the datafile name as an argument, call the ListNodePos function 
	// to fill the datafile with values, then create a temporary gnuplot script to run.
	// Note: If print is non-zero, will print out the GNUplot commands to terminal, else
	// will simply save them to temp file then delete them

	ListNodePos(skl,datafile,min,max);

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
	sprintf(settings,"set size square 1,1\nset tmargin 2\nset xrange [0:1]\nset yrange [0:1]\nunset label\n");
	fputs(settings,tmp);
	if(print)	printf("%s",settings);

	// FIRST PLOT - Nodes only
	char title[BUFSIZ];
	sprintf(title,"set title 'Skeleton nodes from file %s with xy values for z in [%.2f,%.2f]'\n",datafile,min,max);
	fputs(title,tmp);
	if(print)	printf("%s",title);
	
	char plotcom[BUFSIZ];
	sprintf(plotcom,"plot '%s' using 1:2 with points pt 7 ps 0.6 notitle\n",datafile);
	fputs(plotcom,tmp);	
	if(print)	printf("%s",plotcom);

	// SECOND PLOT - Nodes on top of density file
	sprintf(settings,"set view map\nset palette rgb 36,35,34\n");
	fputs(settings,tmp);
	if(print)	printf("%s",settings);
	
	sprintf(title,"set title 'Skeleton segments on density map xy values for z in [%.2f:%.2f]'\n",min,max);
	fputs(title,tmp);
	if(print)	printf("%s",title);
	
	sprintf(plotcom,"splot '%s' using ($1/128):($2/128):4 with points palette pointtype 7,'%s' using 1:2:4 with points pt 7 ps 0.6 lc 5 notitle\n",denfile,datafile);
	fputs(plotcom,tmp);
	if(print)	printf("%s",plotcom);
	
	// Close temp file before running commands through system
	fclose(tmp);
	system("gnuplot -p 'temp'");
	
	// Remove temp file - comment out for trouble shooting
	remove("temp");

	printf("%i node points plotted in %s\n",skl->nnodes,plotfile);
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





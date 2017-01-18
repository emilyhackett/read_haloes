#include "readSkel.h"

///////////////////	STRING REPLACE (FROM MYSTRING.C)	////////////////////

char *strReplace(char *st,char *dest,const char *orig,const char *repl) 
{
  char *ch;
  if (!(ch = strstr(st, orig)))
    return st;
  strncpy(dest, st, ch-st);  
  dest[ch-st] = 0;
  sprintf(dest+(ch-st), "%s%s", repl, ch+strlen(orig));
  return dest;
}

///////////////////	GET SEGMENT DATA INDEX FROM SKELETON	////////////////////

int NDskel_SegDataIndex(NDskel *skl, const char *name)
{
    int i;
    
    for (i=0;i<skl->nsegdata;i++)
    {
      if (!strcmp(name,skl->segdata_info[i]))
	return i;
    }

    return -1;
}

////////////////////	GET DATA FIELD ID (USED TO MAKE FITS)	////////////////////

int getDataFieldID(NDskel *skl,int which, const char *name)
{
  int index=-1;
  long i;

  if (which == 0)
    {
      for (i=0;i<skl->nnodedata;i++)
	{
	  if (!strcmp(skl->nodedata_info[i],name)) index = i;
	}
      
    }
  else
    {
      for (i=0;i<skl->nsegdata;i++)
	{
	  if (!strcmp(skl->segdata_info[i],name)) index = i;
	}
     
    }
  
   return index;
}

////////////////////	FREE FIL TAB FOR NDSKEL (USED TO MAKE FITS)	////////////////////

void freeNDskelFilTab(NDskl_seg ***filTab, int **filSize)
{
  if (filTab!=NULL)
    {
      free(*filTab);
      (*filTab)=NULL;
    }
  
  if (filSize!=NULL)
    {      
      free(*filSize);      
      (*filSize)=NULL;
    } 
}

////////////////////	GET FIL TAB FOR NDSKEL (USED TO MAKE FITS)	////////////////////

long getNDskelFilTab(NDskel *skl, NDskl_seg ***filTab, int **filSize)
{
  long nfil=0;  
  long i,j;

  for (i=0,nfil=0;i<skl->nnodes;i++)
    {
      NDskl_node *node=&(skl->Node[i]);
      for (j=0;j<node->nnext;j++) 
	{	  
	  if (node->Seg[j]->nodes[0]==i) 
	    {
	      nfil++;
	      //(*filTab)=(NDskl_seg**) realloc((*filTab),sizeof(NDskl_seg*)*(nfil+1));
	      //(*filTab)[nfil++]=node->Seg[j];
	    }
	}
    }

  (*filTab)=(NDskl_seg**) realloc((*filTab),sizeof(NDskl_seg*)*(nfil));

  for (i=0,nfil=0;i<skl->nnodes;i++)
    {
      NDskl_node *node=&(skl->Node[i]);
      for (j=0;j<node->nnext;j++) 
	{
	  if (node->Seg[j]->nodes[0]==i) 
	    {
	      //(*filTab)=(NDskl_seg**) realloc((*filTab),sizeof(NDskl_seg*)*(nfil+1));
	      (*filTab)[nfil++]=node->Seg[j];
	    }
	}
    }

  if (filSize!=NULL)
    {
      (*filSize)=realloc((*filSize),nfil*sizeof(int));
      for (i=0;i<nfil;i++)
	{
	  NDskl_seg *seg=(*filTab)[i];
	  j=1;
	  while(seg->Next!=NULL)
	    {
	      seg=seg->Next;
	      j++;
	    }
	  (*filSize)[i]=j;
	}
    }

  return nfil;
}

////////////////////	READ ND SKELETON INTO A NDSKEL STRUCTURE	////////////////////	
NDskel *readNDskeleton(char *filename)
{
	printf("Reading ND skeleton structure from file %s\n",filename);

	FILE *fp;
	fp=fopen(filename,"r");
	if(fp==NULL)
	{
		fprintf(stderr,"Error opening file %s\n",filename);
	}
	else {
		printf("%s opened for reading\n",filename);
	}
	

	int i,j;
      	char tag[NDSKEL_DATA_STR_SIZE];
      	char dummy[160];
       	int swap=0;
	int index;
      
	// Define structure to read in to
	NDskel *skl;

	skl=calloc(1,sizeof(NDskel));
	memset(tag,0,16*sizeof(char));

	fread_sw(&i,sizeof(int),1,fp,swap);	// DUMMY
	if(i!=16) swap=1-swap;
	
	fread_sw(tag,sizeof(char),16,fp,swap);
	if(LONG)	printf(" tag	= %s\n",tag);
	fread_sw(&i,sizeof(int),1,fp,swap);	// DUMMY

	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
	fread_sw(skl->comment,sizeof(char),80,fp,swap);
	if(LONG)	printf(" comment	= %s\n",skl->comment);
	
	fread_sw(&skl->ndims,sizeof(int),1,fp,swap);
	if(LONG)	printf(" ndims	= %i\n",skl->ndims);
	

	// Allocate room based on ndims
	skl->dims=malloc(skl->ndims*sizeof(int));
	skl->x0=malloc(skl->ndims*sizeof(double));
	skl->delta=malloc(skl->ndims*sizeof(double));
	
	// Read in dummy variables after allocated dimensions
	fread_sw(skl->dims,sizeof(int),skl->ndims,fp,swap);
	if (skl->ndims<NDSKEL_MAX_DIMS) 
	{
		fread_sw(dummy,sizeof(int),NDSKEL_MAX_DIMS-skl->ndims,fp,swap);
	}
      	fread_sw(skl->x0,sizeof(double),skl->ndims,fp,swap);
      	if (skl->ndims<NDSKEL_MAX_DIMS) 
	{
		fread_sw(dummy,sizeof(double),NDSKEL_MAX_DIMS-skl->ndims,fp,swap);
	}
      	fread_sw(skl->delta,sizeof(double),skl->ndims,fp,swap);
      	if (skl->ndims<NDSKEL_MAX_DIMS) 
	{
		fread_sw(dummy,sizeof(double),NDSKEL_MAX_DIMS-skl->ndims,fp,swap);
	}
	// Print out dims, x0 and delta to check
	for(i=0;i<skl->ndims;i++)
	{
		if(LONG)	printf(" dims[%i] = %i,	x0[%i] = %.2f,	delta[%i] = %.2f\n",i,skl->dims[i],i,
				skl->x0[i],i,skl->delta[i]);
	}

	fread_sw(&skl->nsegs,sizeof(int),1,fp,swap);
	if(LONG)	printf(" nsegs	= %i\n",skl->nsegs);
      	fread_sw(&skl->nnodes,sizeof(int),1,fp,swap);
	if(LONG)	printf(" nnodes	= %i\n",skl->nnodes);
      	fread_sw(&skl->nsegdata,sizeof(int),1,fp,swap);
	if(LONG)	printf(" nsegdata	= %i\n",skl->nsegdata);
      	fread_sw(&skl->nnodedata,sizeof(int),1,fp,swap);
	if(LONG)	printf(" nnodedata	= %i\n",skl->nnodedata);
      	fread_sw(&j,sizeof(int),1,fp,swap);


	// Clear and assign data for segment and node data info
	skl->segdata_info = calloc(skl->nsegdata,sizeof(char *));
      	skl->nodedata_info = calloc(skl->nnodedata,sizeof(char *));
 
      	if (skl->nsegdata)	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
	
      	for (i=0;i<skl->nsegdata;i++)
    	{
	  	skl->segdata_info[i]=calloc(NDSKEL_DATA_STR_SIZE,sizeof(char));
	  	fread_sw(skl->segdata_info[i],sizeof(char),NDSKEL_DATA_STR_SIZE,fp,swap);
		if(LONG)	printf(" segdata_info[%i]	= %s\n",i,skl->segdata_info[i]);
    	}
	
	// Read in two dummy variables
      	if (skl->nsegdata)	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
      	if (skl->nnodedata)	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
	
      	for (i=0;i<skl->nnodedata;i++)
    	{
	  	skl->nodedata_info[i]=calloc(NDSKEL_DATA_STR_SIZE,sizeof(char));
	  	fread_sw(skl->nodedata_info[i],sizeof(char),NDSKEL_DATA_STR_SIZE,fp,swap);
		if(LONG)	printf(" nodedata_info[%i]	= %s\n",i,skl->nodedata_info[i]);
    	}
      
	if (skl->nnodedata)	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
	
	// Assign memory for segment positions
	skl->segpos = malloc((long)sizeof(float)*skl->nsegs*2*skl->ndims);

	
       	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
      	fread_sw(skl->segpos,sizeof(float),skl->nsegs*2*skl->ndims,fp,swap);
	printf("segpos read in, total of %i with %iD each\n",skl->nsegs,skl->ndims);
//	for(int i=0;i<skl->nsegs;i++)
//	{
//		printf("	%.2f\n",skl->segpos[i]);
//	}
      	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
  
	// Assign memory for node positions
      	skl->nodepos = malloc((long)sizeof(float)*skl->nnodes*skl->ndims);
      	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
      	fread_sw(skl->nodepos,sizeof(float),skl->nnodes*skl->ndims,fp,swap);
	printf("nodepos	read in, total of %i with %iD each\n",skl->nnodes,skl->ndims);
//	for(int i=0;i<skl->nnodes;i++)
//	{
//		printf("	%.2f\n",skl->nodepos[i]);
//	}
	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY

	// Assign memory for segment data
      	skl->segdata = malloc((long)sizeof(double)*skl->nsegs*skl->nsegdata);
      	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
	if(LONG)	printf("Reading in segdata ...");
      	fread_sw(skl->segdata,sizeof(double),skl->nsegs*skl->nsegdata,fp,swap);
	if(LONG)	printf("		read\n");
      	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY

	// Assign memory for node data
      	skl->nodedata = malloc((long)sizeof(double)*skl->nnodes*skl->nnodedata);
      	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
	if(LONG)	printf("Reading in nodedata ...");
      	fread_sw(skl->nodedata,sizeof(double),skl->nnodes*skl->nnodedata,fp,swap);
	if(LONG)	printf("		read\n");
      	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY

	// Assign memory for Node and Seg
      	skl->Node = malloc((long)sizeof(NDskl_node)*skl->nnodes);      
	skl->Seg = malloc((long)sizeof(NDskl_seg)*skl->nsegs);

	// Initialise node and seg lists;
      	NDskl_node *node;
      	NDskl_seg *seg;

	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY

	// Index over all nodes and read in data
	if(LONG)	printf("Reading in node array ...");      
	for(i=0;i<skl->nnodes;i++)
    	{
		// Initialise array of nodes 
	  	node = &skl->Node[i];
	  	fread_sw(&index,sizeof(int),1,fp,swap);	
	  	node->pos = &skl->nodepos[(long)index*skl->ndims];
	  	if (skl->nnodedata) node->data = &skl->nodedata[(long)index*skl->nnodedata];
	  	else node->data=NULL;
	  	fread_sw(&node->flags,sizeof(int),1,fp,swap);
	  	fread_sw(&node->nnext,sizeof(int),1,fp,swap);
	  	fread_sw(&node->type,sizeof(int),1,fp,swap);
	   	fread_sw(&node->index,sizeof(int),1,fp,swap);

		// Allocate memory for next node
	  	node->nsegs=malloc(sizeof(int)*node->nnext);
	  	node->Next=malloc(sizeof(NDskl_node *)*node->nnext);
	  	node->Seg=malloc(sizeof(NDskl_seg *)*node->nnext);
	  	fread_sw(node->nsegs,sizeof(int),node->nnext,fp,swap);
	  	for (j=0;j<node->nnext;j++)
		{
	      		fread_sw(&index,sizeof(int),1,fp,swap);
	      		if (index>=0) node->Next[j] = &skl->Node[index];
	      		else node->Next[j] =NULL;

	      		fread_sw(&index,sizeof(int),1,fp,swap);

	     		if (index>=0) node->Seg[j] = &skl->Seg[index];
	     		else node->Seg[j] =NULL;
		}
    	}
	if(LONG)	printf("	read\n");

      	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
      	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
      
	if(LONG)	printf("Reading in seg array ...");
	for(i=0;i<skl->nsegs;i++)
    	{
		// Initialise array of segments
	  	seg = &skl->Seg[i];
	  	fread_sw(&index,sizeof(int),1,fp,swap);
	  	seg->pos = &skl->segpos[(long)index*2*skl->ndims];
	 	if (skl->nsegdata) seg->data = &skl->segdata[(long)index*skl->nsegdata];
	  	else seg->data = NULL;
	  	fread_sw(seg->nodes,sizeof(int),2,fp,swap);
	  	fread_sw(&seg->flags,sizeof(int),1,fp,swap);
	  	fread_sw(&seg->index,sizeof(int),1,fp,swap);
      
	  	fread_sw(&index,sizeof(int),1,fp,swap);
	  	if (index<0) seg->Next=NULL;
	  	else seg->Next = &skl->Seg[index];
      
	  	fread_sw(&index,sizeof(int),1,fp,swap);
	  	if (index<0) seg->Prev=NULL;
	  	else seg->Prev = &skl->Seg[index];
    
    	}
      	if(LONG)	printf("	read\n");

	fread_sw(&j,sizeof(int),1,fp,swap);	// DUMMY
	fclose(fp);

	printf("%s read COMPLETED, file closed\n",filename);
	
	return skl;
}

////////////////////	COMPUTE SEGMENT LENGTH	////////////////////	

double ComputeSegLen(NDskel *skl, NDskl_seg* seg)
{
  int i;
  double d=0;
  
  for (i=0;i<skl->ndims;i++)
    d+= pow(seg->pos[i+skl->ndims]-seg->pos[i],2);

  return sqrt(d);
}

////////////////////	COMPUTE DISTANCE TO NEXT SEGMENT	////////////////////	

double ComputeDistToNext(NDskel *skl, NDskl_seg* seg_p)
{
  int i;
  double d=0;
  double dt=0;
  NDskl_seg* seg = seg_p;

  do 
    {
      //printf ("index:%d, seg=%d\n",seg->index,seg);
      for (i=0,d=0;i<skl->ndims;i++)
	d+= pow(seg->pos[i+skl->ndims]-seg->pos[i],2);
      dt+=sqrt(d);
    } while ((seg=seg->Next)!=NULL);

  return dt;
}

////////////////////	COMPUTE DISTANCE FROM PREVIOUS SEGMENT	////////////////////	

double ComputeDistFromPrev(NDskel *skl, NDskl_seg* seg_p)
{
  int i;
  double d=0;
  double dt=0;
  NDskl_seg* seg = seg_p;

  while ((seg=seg->Prev)!=NULL)
  {
      for (i=0,d=0;i<skl->ndims;i++)
	d+= pow(seg->pos[i+skl->ndims]-seg->pos[i],2);
      dt+=sqrt(d);
  }

  return dt;
}

////////////////////	CHECK SANITY OF ND SKELETON!	////////////////////	

int NDskelCheckSanity(NDskel *skl, int periodicity)
{
    int i,j,k;
    int nbugs;
    int ndisc;
    int npass=1;

    if (skl==NULL) return -1;

    printf("* Checking skeleton sanity ... ");

    do {
	nbugs=0;
	ndisc=0;
	for (i=0;i<skl->nnodes;i++)
	{
	    //printf ("node %d\n",i);
	    NDskl_node *node;
	    node = &skl->Node[i];
	    
	    for (j=0;j<node->nnext;j++)
	    {
		NDskl_seg *seg;
		NDskl_node *nextnode;
		NDskl_seg *curseg;
		NDskl_seg *oldseg;
		double cmpval[skl->ndims];
		
		for (k=0;k<skl->ndims;k++)
		    cmpval[k] = 0.01*skl->delta[k]/skl->dims[k];

		seg = node->Seg[j];
		oldseg=seg;
		nextnode = node->Next[j];

		for (k=0;k<skl->ndims;k++)
		    if ((fabs(node->pos[k]-seg->pos[k])>cmpval[k])&&
			(fabs(node->pos[k]-seg->pos[k+skl->ndims])>cmpval[k])&&
			((!(periodicity & (1<<k)))||
			 ((fabs(fabs(node->pos[k]-seg->pos[k])-skl->delta[k])>cmpval[k])&&
			  (fabs(fabs(node->pos[k]-seg->pos[k])-skl->delta[k])<0.5*skl->delta[k])&&
			  (fabs(fabs(node->pos[k]-seg->pos[k+skl->ndims])-skl->delta[k])>cmpval[k])&&
			  (fabs(fabs(node->pos[k]-seg->pos[k+skl->ndims])-skl->delta[k])<0.5*skl->delta[k]))))
			break;

		if (k!=skl->ndims)
		{
		    if (!ndisc)
		    {
			printf ("Node discontinuity :\n");
			printf ("nodepos=[%f %f %f]\n",
				node->pos[0],node->pos[1],node->pos[2]);
			printf ("nextpos=[%f %f %f],[%f %f %f]\n",
			    seg->pos[0],seg->pos[1],seg->pos[2],
			    seg->pos[3],seg->pos[4],seg->pos[5]);
		    }
/*
		    for (k=0;k<skl->nsegs;k++)
		    {
			printf ("LOOKING FOR LOST NODE ;)");
		    }
		    */
		    nbugs++;
		    ndisc++;
		}

		//else printf ("OK\n");
		
		if ((seg->Next!=NULL)&&(seg->Prev!=NULL))
		{
		    //printf("PROBLEM 1\n");
		    nbugs++;
		}
		
		if ((seg->Next!=NULL)||(seg->Prev!=NULL))
		{
		    if (seg->Next!=NULL)
		    {
			
			for (oldseg=seg,curseg=seg->Next;curseg!=NULL;oldseg=curseg,curseg=curseg->Next)
			{

			    for (k=0;k<skl->ndims;k++)
				if (curseg->pos[k]!=oldseg->pos[k]) break;
			    if (k!=skl->ndims)
				for (k=0;k<skl->ndims;k++)
				    if (curseg->pos[k]!=oldseg->pos[k+skl->ndims]) break;
			    if (k!=skl->ndims)
				for (k=0;k<skl->ndims;k++)
				    if (curseg->pos[k+skl->ndims]!=oldseg->pos[k+skl->ndims]) break;
			    if (k!=skl->ndims)
				for (k=0;k<skl->ndims;k++)
				    if (curseg->pos[k+skl->ndims]!=oldseg->pos[k]) break;
			    if (k!=skl->ndims) 
			    {
				for (k=0;k<skl->ndims;k++)
				    if (fabs(curseg->pos[k]-oldseg->pos[k])>skl->delta[k]*0.5)
					break;

				if (k==skl->ndims)
				{
				    printf ("segment discontinuity ERROR (Next dir)\n");
				    printf ("curpos=[%f %f %f],[%f %f %f]\n",
					    curseg->pos[0],curseg->pos[1],curseg->pos[2],
					    curseg->pos[3],curseg->pos[4],curseg->pos[5]);
				    printf ("prepos=[%f %f %f],[%f %f %f]\n",
					    oldseg->pos[0],oldseg->pos[1],oldseg->pos[2],
					    oldseg->pos[3],oldseg->pos[4],oldseg->pos[5]);
				}
			    }
			    //else printf ("OK\n");
			    //printf ("seg->index = %d\n",seg->index);
			    if (curseg->Prev != oldseg)
			    {
				
				if (curseg->Prev == NULL)
				{
/*
				    //printf("Fixed NULL value Prev.\n");
				    printf ("\ncurpos=[%f %f %f],[%f %f %f]\n",
					    curseg->pos[0],curseg->pos[1],curseg->pos[2],
					    curseg->pos[3],curseg->pos[4],curseg->pos[5]);
				    printf ("\nprepos=[%f %f %f],[%f %f %f]\n",
					    oldseg->pos[0],oldseg->pos[1],oldseg->pos[2],
					    oldseg->pos[3],oldseg->pos[4],oldseg->pos[5]);
*/
				    curseg->Prev = oldseg;
				    nbugs++;
				}
				else
				{
				    //printf("Fixed non-NULL value Prev.\n");
				    curseg->Prev = oldseg;
				    nbugs++;
				}
				
			    }
			}
			
		    }
		    if (seg->Prev!=NULL)
		    {
			for (oldseg=seg,curseg=seg->Prev;curseg!=NULL;oldseg=curseg,curseg=curseg->Prev)
			{
			    for (k=0;k<skl->ndims;k++)
				if (curseg->pos[k]!=oldseg->pos[k]) break;
			    if (k!=skl->ndims)
				for (k=0;k<skl->ndims;k++)
				    if (curseg->pos[k]!=oldseg->pos[k+skl->ndims]) break;
			    if (k!=skl->ndims)
				for (k=0;k<skl->ndims;k++)
				    if (curseg->pos[k+skl->ndims]!=oldseg->pos[k+skl->ndims]) break;
			    if (k!=skl->ndims)
				for (k=0;k<skl->ndims;k++)
				    if (curseg->pos[k+skl->ndims]!=oldseg->pos[k]) break;
			    
			    if (k!=skl->ndims) 
			    {
			
				for (k=0;k<skl->ndims;k++)
				    if (fabs(curseg->pos[k]-oldseg->pos[k])>skl->delta[k]*0.5)
					break;

				if (k==skl->ndims)
				{
				    printf ("segment discontinuity ERROR (Prev dir)\n");
				    printf ("curpos=[%f %f %f],[%f %f %f]\n",
					    curseg->pos[0],curseg->pos[1],curseg->pos[2],
					    curseg->pos[3],curseg->pos[4],curseg->pos[5]);
				    printf ("prepos=[%f %f %f],[%f %f %f]\n",
					    oldseg->pos[0],oldseg->pos[1],oldseg->pos[2],
					    oldseg->pos[3],oldseg->pos[4],oldseg->pos[5]);
				    
				}
			    }

			//else printf ("OK\n");
			    //printf ("seg->index = %d\n",seg->index);
			    if (curseg->Next != oldseg)
			    {
				
				if (curseg->Next == NULL)
				{
				    //printf("Fixed NULL value Next.\n");
				    curseg->Next = oldseg;
				    nbugs++;
				}
				else
				{
				    //printf("Fixed non-NULL value Next.\n");
				    curseg->Next = oldseg;
				    nbugs++;
				}
				
			    }
			}
		    }
		}
		else {}
		
		seg=oldseg;
		for (k=0;k<nextnode->nnext;k++)
		{
		    if (nextnode->Seg[k] == seg)
			break;
		}
		if (k==nextnode->nnext)
		{
		    
		    nbugs++;
		    for (k=0;k<nextnode->nnext;k++)
		    {
			if (nextnode->Next[k] == node)
			break;
		    }

		    printf ("node %d does not lead to node %d\n",node->index,nextnode->index);
		    seg=node->Seg[j];
		    printf ("seg = %d ",seg->index);
		    for (oldseg=seg,curseg=seg->Prev;curseg!=NULL;oldseg=curseg,curseg=curseg->Prev)
			printf ("(%d->%d)",oldseg->index,curseg->index);
		    for (oldseg=seg,curseg=seg->Next;curseg!=NULL;oldseg=curseg,curseg=curseg->Next)
			printf ("(%d->%d)",oldseg->index,curseg->index);
		    printf ("= %d.\n",nextnode->Seg[k]->index);
		    
		    seg=nextnode->Seg[k];
		    printf ("seg = %d ",seg->index);
		    for (oldseg=seg,curseg=seg->Prev;curseg!=NULL;oldseg=curseg,curseg=curseg->Prev)
			printf ("(%d->%d)",oldseg->index,curseg->index);
		    for (oldseg=seg,curseg=seg->Next;curseg!=NULL;oldseg=curseg,curseg=curseg->Next)
			printf ("(%d->%d)",oldseg->index,curseg->index);
		    printf ("= %d.\n",node->Seg[j]->index);

		    exit(0);
		    
		}
		
	    }
	}
	printf ("(pass %d: %d bugs (%d disc.)) ",npass++,nbugs,ndisc);
	//exit(0);
    } while ((nbugs)&&(npass<3));

    if ((npass>1) && (nbugs))
    {
	printf (" could not fix %d bugs (%d disc).\n",nbugs,ndisc);
	if (ndisc==nbugs)
	{
	    printf (" This may be because the skeleton has been computed from an old version of rsex.\n");
	    printf (" Don't worry too much, this should be OK :)\n");
	}
    }
    else
	printf ("done.\n");


    return nbugs;
}

////////////////////	SAVE ASCII FORMAT OF SKELETON (VISUALISATION?)	////////////////////

int Save_ASCIIskel(NDskel *skl,const char *filename)
{
	
  long i,j,l;//,k
  //char tag[NDSKEL_DATA_STR_SIZE];
  FILE *f;
  long **filTab=NULL;
  int *filSegCount=NULL;
  NDskl_seg **filSegTab=NULL;
  int nfil;

	printf ("Saving %dD skeleton to ASCII file %s ...",skl->ndims,filename);fflush(0);

  int nodeDenId=getDataFieldID(skl,0,VALUE_TAG);
  int nodePairId=getDataFieldID(skl,0,PERSISTENCE_PAIR_TAG);
  
  if (nodeDenId==-1) fprintf(stderr,"\nWARNING : could not find '%s' information for nodes.\n",VALUE_TAG);
  if (nodePairId==-1) fprintf(stderr,"\nWARNING : could not find '%s' information for nodes.\n",PERSISTENCE_PAIR_TAG);


  f=fopen(filename,"w");

  // Writing HEADER DATA to file
  fprintf(f,"%s\n",NDSKEL_ASCII_TAG);		// Write tag to file
  fprintf(f,"%d\n",skl->ndims);			// Write number of dims to file
  fprintf(f,"#%s\n",skl->comment);		// Write comment to file
  fprintf(f,"BBOX [%g",skl->x0[0]);		// Specify box dimensions (x0, delta)
  for (i=1;i<skl->ndims;i++) fprintf(f,",%g",skl->x0[i]);
  fprintf(f,"] [%g",skl->delta[0]);
  for (i=1;i<skl->ndims;i++) fprintf(f,",%g",skl->delta[i]);
  fprintf(f,"]\n");
  
  // 
  nfil=getNDskelFilTab(skl,&filSegTab,&filSegCount);	

  filTab=(long**)calloc(skl->nnodes,sizeof(long*));
  for (i=0;i<skl->nnodes;i++) 
    if (skl->Node[i].nnext)
      filTab[i]=(long*)calloc(skl->Node[i].nnext,sizeof(long));
    else filTab[i]=NULL;
  

  NDskl_node *oldNode=NULL;
  NDskl_node *oldNext=NULL;
  int jNext=-1;
  int jNode=-1;
  for (i=0;i<nfil;i++)
    {
      NDskl_node *node=&skl->Node[filSegTab[i]->nodes[0]];
      NDskl_node *next=&skl->Node[filSegTab[i]->nodes[1]];

      if ((node == oldNode)&&(next==oldNext))
	{
	  jNode++;
	  jNext++;
	}
      else 
	{
	  jNode=0;
	  jNext=0;	  
	}

      for (;jNode<node->nnext;jNode++) 
	if (node->Next[jNode]==next) break;
      if (jNode==node->nnext) fprintf(stderr,"ERROR in Save_ASCIIskel: invalid skeleton file!\n");
      filTab[node->index][jNode]=i;

      for (;jNext<next->nnext;jNext++)
	if (next->Next[jNext]==node) break;
      if (jNext==next->nnext) fprintf(stderr,"ERROR in Save_ASCIIskel: invalid skeleton file!\n");
      filTab[next->index][jNext]=i;
      oldNode=node;
      oldNext=next;
    }

  fprintf(f,"[CRITICAL POINTS]\n");
 
  fprintf(f,"%d\n",skl->nnodes);	// Write number of nodes to file
  
  // Index over all nodes and write to file
  for (i=0;i<skl->nnodes;i++)
    {
      NDskl_node *node=&(skl->Node[i]);
      
      fprintf(f,"%d",node->type);
      for (j=0;j<skl->ndims;j++) fprintf(f," %g",node->pos[j]);

      if (nodeDenId>=0) fprintf(f," %g",node->data[nodeDenId]);
      else fprintf(f," 0");
      if (nodePairId>=0) fprintf(f," %d",(int)(node->data[nodePairId]));
      else fprintf(f," %d",(int)i);
      int boundary=0;
      if (node->flags&FLAG_NDNODE_INFINITE) boundary=3;
      else if (node->flags&FLAG_NDNODE_OUT) boundary=2;
      else if (node->flags&FLAG_NDNODE_BOUNDARY) boundary=1;
      fprintf(f," %d\n",boundary);
      fprintf(f," %d\n",node->nnext);
      
      for (j=0;j<node->nnext;j++)
	fprintf(f," %d %ld\n",node->Next[j]->index,(long)filTab[i][j]);
    }

  for (i=0;i<skl->nnodes;i++) free(filTab[i]);
  free(filTab);
 
  fprintf(f,"[FILAMENTS]\n");
  fprintf(f,"%d\n",nfil);
 
  for (i=0;i<nfil;i++)
    {
      NDskl_seg *seg=filSegTab[i];
      NDskl_node *node=&skl->Node[seg->nodes[0]];
      NDskl_node *next=&skl->Node[seg->nodes[1]];
     
      fprintf(f,"%ld %ld %ld\n",(long)node->index,(long)next->index,(long)filSegCount[i]+1);
      for (j=0;j<skl->ndims;j++) fprintf(f," %g",node->pos[j]);fprintf(f,"\n");
      if (seg->Next!=NULL)
	{
	  seg=seg->Next;
	  do {	    
	    for (j=0;j<skl->ndims;j++) 
	      fprintf(f," %g",seg->pos[j]);fprintf(f,"\n");
	    seg=seg->Next;
	  } while(seg!=NULL);
	}
      for (j=0;j<skl->ndims;j++) fprintf(f," %g",next->pos[j]);fprintf(f,"\n");
    }
 
  fprintf(f,"[CRITICAL POINTS DATA]\n");
  fprintf(f,"%d\n",skl->nnodedata);
  for (i=0;i<skl->nnodedata;i++)
    fprintf(f,"%s\n",skl->nodedata_info[i]);
  for (i=0;i<skl->nnodes;i++)
    {	  
      for (j=0;j<skl->nnodedata-1;j++)
	fprintf(f,"%.7g ",skl->Node[i].data[j]);
      fprintf(f,"%.7g\n",skl->Node[i].data[j]);
    }

  fprintf(f,"[FILAMENTS DATA]\n");  
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
  fprintf(f,"%d\n",nsegdata);
  for (i=0;i<nsegdata;i++)
    fprintf(f,"%s\n",tmp[i]);

  for (i=0;i<nfil;i++)
    {
      NDskl_seg *seg=filSegTab[i];
            
      for (l=0;l<nsegdata-1;l++)
	fprintf(f,"%.7g ",seg->data[pid[2*l]]);
      fprintf(f,"%.7g\n",seg->data[pid[2*l]]);

      if (seg->Next!=NULL)
	{	  
	  do {	    
	    seg=seg->Next;
	    for (l=0;l<nsegdata-1;l++)
	      fprintf(f,"%.7g ",seg->data[pid[2*l]]);
	    fprintf(f,"%.7g\n",seg->data[pid[2*l]]);
	  } while(seg->Next!=NULL);
	}
      
      for (l=0;l<nsegdata-1;l++)
	fprintf(f,"%.7g ",seg->data[pid[2*l+1]]);
      fprintf(f,"%.7g\n",seg->data[pid[2*l+1]]);
    }

  fclose(f);
  freeNDskelFilTab(&filSegTab,&filSegCount);

	printf(" done.\n");
  return 0;
}


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


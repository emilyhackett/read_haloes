#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

// Define global variables required for structure and functions

#define NDSKEL_DATA_STR_SIZE 20
#define NDSKEL_TAG "NDSKEL"
#define NDSKEL_ASCII_TAG "ANDSKEL"
#define NDSKEL_MAX_DIMS 20

#define FLAG_NDNODE_BOUNDARY (1<<0)
#define FLAG_NDNODE_OUT      (1<<1)
#define FLAG_NDNODE_INFINITE (1<<2)


////////////////////	GLOBAL STRUCTURES FOR ND SKELETONS	////////////////////

struct NDskl_seg_str {
	int nodes[2];	/* Index of the nodes at the extremity of the arc, segment
			   oriented from nodes[0] to nodes[1] */
	float *pos;	/* Points to appropriate location in segpos */
	int flags;	/* Non null if on boundary */
	int index;	/* Index of the segment in the Seg array */
	double *data;	/* Points to the nsegdata supplementary data for this
			   segment */
	struct NDskl_seg_str *Next;	/* Points to next segment in the arc
					   NULL if extremity (connected to nodes[1]) */
	struct NDskl_seg_str *Prev;	/* Points to previous segment in the arc
					   NULL if extremity (connected to nodes[0]) */
}; 
typedef struct NDskl_seg_str NDskl_seg;

struct NDskl_node_str {
	float *pos;	/* Points to appropriate location in nodepos */
	int flags;	/* Non null if on boundary */
	int nnext;	/* Number of arcs connected */
	int type;   	/* Critical index (?) */
	int index;	/* Index of the node in the Node array */
    	int *nsegs; 	/* Number of segments in each of the nnext arcs */
    	double *data;	/* Points to the nnodedata supplementary data for this 
			   segment */
    	struct NDskl_node_str **Next;	/* Points to the node at the other extremity of
					 each of the nnext arcs */  
    	struct NDskl_seg_str **Seg;	/* Points to the first segment in the arcs, 
					   starting from the current node */
}; 
typedef struct NDskl_node_str NDskl_node;

typedef struct NDskel_str {
    	char comment[80];
    
    	int ndims;	/* Number of dimensions */
    	int *dims;	/* Dimension of the underlying grid, only meaningful when
			   computed from a regular grid */
    	double *x0;	/* Origin of the bounding box */
    	double *delta;	/* Dimension of the bounding box */
    
    	int nsegs;	/* Number of segments */
    	int nnodes;	/* Number of nodes */
    
    	int nsegdata;	/* Number of additional data for segments */
    	int nnodedata;	/* Number of additional data for nodes */
    	char **segdata_info;	/* Name of additional fields for segments */
    	char **nodedata_info;	/* Name of additional fields for nodes */
    
    	float *segpos;	/* Positions of the extremities of segments, 2x ndims coordinates
			   for each segment */
    	float *nodepos;	/* Positions of the nodes (ndims coords for each segment) */
    	double *segdata;	/* Additional ddata for segments (nsegs times nsegdata
				   consecutive values */
    	double *nodedata;	/* Additional data for nodes (nnodes times nnodedata 
				   consecutive values */
    
    	NDskl_seg *Seg;		/* Segment array (contains all segs) */
    	NDskl_node *Node;	/* Nodes array (contains all nodes) */
} NDskel;



////////////////////	DEFINE FUNCTIONS IN "readNDskel.c"	////////////////////

extern char	*strReplace(char *,char *,const char *, const char *);
extern int 	NDskel_SegDataIndex(NDskel *,const char *);
extern int 	getDataFieldID(NDskel *,int, const char *);
extern void	freeNDskelFilTab(NDskl_seg ***,int **);
extern long	getNDskelFilTab(NDskel *,NDskl_seg ***,int **);
extern NDskel	*readNDskeleton(char *);
extern double	ComputeSegLen(NDskel *,NDskl_seg *);
extern double	ComputeDistToNext(NDskel *,NDskl_seg *);
extern double	ComputeDistFromPrev(NDskel *,NDskl_seg *);
extern int	NDskelCheckSanity(NDskel *,int);
extern int	Save_ASCIIskel(NDskel *,const char *);

////////////////////	DEFINE FUNCTIONS IN "flattenSkel.c"	////////////////////

extern void	FlattenSkl(NDskel *);

////////////////////	DEFINE FUNCTIONS IN "plotSkel.c"	////////////////////

extern void	ListNodePos(NDskel *,char *);
extern void	ListSegPos(NDskel *,char *);
extern void	PlotNodePos(NDskel *,char *,char *,int);
extern void	PlotSegPos(NDskel *,char *,char *,int);
extern void	ListNodeFieldVals(NDskel *,char *);
extern void	ListSegFieldVals(NDskel *,char *);

////////////////////	HEADER FILE FOR "myendian.h"	////////////////////

#ifndef __ENDIAN_SWAP
#define __ENDIAN_SWAP

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


int fread_sw(void *,size_t,size_t,FILE *,int);
size_t freadBE(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fwriteBE(const void *ptr, size_t size, size_t nmemb,FILE *stream);


int swapI(int);
float swapF(float);
double swapD(double);
void Dswap2B(void*);
void Dswap4B(void*);
void Dswap8B(void*);
void Dswap2BArr(void*,size_t);
void Dswap4BArr(void*,size_t);
void Dswap8BArr(void*,size_t);

#ifdef __cplusplus
}
#endif


#endif

////////////////////	HEADER FILE FOR "global.h"	////////////////////

#ifdef GLOBAL_DEFINITION
#define GLOBAL
#else 
#define GLOBAL extern
#endif

#ifndef VERSION_STR
#define STRINGIFY1(x)  #x
#define STRINGIFY(x)  STRINGIFY1(x)
#define VERSION_STR STRINGIFY(VER_MAJOR) "." STRINGIFY(VER_MINOR) "." STRINGIFY(VER_BUILD) 
#endif

#ifndef VALUE_TAG
#define VALUE_TAG "field_value"
#endif

#ifndef MASS_TAG
#define MASS_TAG "mass"
#endif

#ifndef PARENT_TAG
#define PARENT_TAG "parent_index"
#endif

#ifndef PARENT_LOG_TAG
#define PARENT_LOG_TAG "parent_log_index"
#endif

#ifndef PERSISTENCE_PAIR_TAG
#define PERSISTENCE_PAIR_TAG "persistence_pair"
#endif

#ifndef PERSISTENCE_TAG
#define PERSISTENCE_TAG "persistence"
#endif

#ifndef PERSISTENCE_RATIO_TAG
#define PERSISTENCE_RATIO_TAG "persistence_ratio"
#endif

#ifndef PERSISTENCE_NSIG_TAG
#define PERSISTENCE_NSIG_TAG "persistence_nsigmas"
#endif

#ifndef ROBUSTNESS_TAG
#define ROBUSTNESS_TAG "robustness"
#endif

#ifndef ROBUSTNESS_RATIO_TAG
#define ROBUSTNESS_RATIO_TAG "robustness_ratio"
#endif

#ifndef ARC_ID_TAG
#define ARC_ID_TAG "arc_id"
#endif

#ifndef FILAMENT_ID_TAG
#define FILAMENT_ID_TAG "filament_id"
#endif
        
#ifndef ORIENTATION_TAG
#define ORIENTATION_TAG "orientation"
#endif

#ifndef TYPE_TAG
#define TYPE_TAG "type"
#endif

#ifndef INDEX_TAG
#define INDEX_TAG "index"
#endif

#ifndef TRUE_INDEX_TAG
#define TRUE_INDEX_TAG "true_index"
#endif

#ifndef CELL_TAG
#define CELL_TAG "cell"
#endif

#ifndef BOUNDARY_TAG
#define BOUNDARY_TAG "boundary"
#endif

#ifndef DELAUNAY_TESSELATION_TAG
#define DELAUNAY_TESSELATION_TAG "delaunay_tesselation"
#endif

#ifndef CRITICAL_INDEX_TAG
#define CRITICAL_INDEX_TAG "critical_index"
#endif

#ifndef LENGTH_TAG
#define LENGTH_TAG "length"
#endif

#ifndef LOG_TAG
#define LOG_TAG(tag) "log_" tag
#endif

#ifndef UP_TAG
#define UP_TAG(tag) "up_" tag
#endif

#ifndef DOWN_TAG
#define DOWN_TAG(tag) "down_" tag
#endif

#ifndef SEG_P1_TAG
#define SEG_P1_TAG(tag) tag "_p1"
#endif

#ifndef SEG_P2_TAG
#define SEG_P2_TAG(tag) tag "_p2"
#endif


#ifndef SOURCE_TAG
#define SOURCE_TAG(tag) "source_" tag
#endif

GLOBAL int verbose;
GLOBAL int debug_dump;
GLOBAL int glob_num_threads;
GLOBAL int glob_num_omp_threads;

#undef GLOBAL

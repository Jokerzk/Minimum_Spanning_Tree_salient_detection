#include "common.h"
#include "FT.h"

#define k 3  

struct Tuple{
	float attrL;
	float attra;
	float attrb;
};
struct Group{
	vector<Tuple> clusters[k];
	Tuple means[k];
	float covinvert[k][3][3];
};
extern bool flag;
int getBoundaryDissimilarityMap(cv::Mat Src, int boundarysize, float* Dest);

/*functions for K_means*/
vector<Tuple>* KMeans(cv::Mat imgLab, int width, int height, int boundarysize, vector<Tuple> clusters[k], Tuple means[k]);

vector<Tuple> getSeeds(cv::Mat imgLab, int width, int height, vector<Tuple> Seeds);

float getDistLab(Tuple t1, Tuple t2);

int clusterOfTuple(Tuple means[], Tuple tuple);

void getCovmatrixInvert(int width, int height, vector<Tuple> clusters[k],float covinvert[k][3][3]);

Tuple getMeans(vector<Tuple> cluster);
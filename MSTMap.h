#include<stdio.h>
#include<stdlib.h>
#include<sstream>
#include <iostream>
#include <opencv2/opencv.hpp>

#define MAX_VERTEX_NUM 1000
#define OK 1
#define ERROR 0
#define MAX 255*255*100
using namespace std;
using namespace cv;

typedef struct Arcell
{
	double adj;
}Arcell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

typedef struct
{
	char vexs[MAX_VERTEX_NUM];//节点数组
	AdjMatrix arcs;//邻接矩阵
	int vexnum, arcnum;//图的当前节点数和弧数
}MGraph;

typedef struct Pnode//用于普利姆算法
{
	char adjvex;//节点
	double lowcost;//权值
}Pnode, Closedge[MAX_VERTEX_NUM];//记录顶点集U到V-U的代价最小的边的辅助数组定义

typedef struct Knode//用于克鲁斯卡尔算法中存储一条边及其对应的2个节点
{
	char ch1;//节点1
	char ch2;//节点2
	double value;//权值
}Knode, Dgevalue[MAX_VERTEX_NUM];


//-------------------------------------------------------------------------------
int CreateUDG(MGraph & G, Dgevalue & dgevalue);
int LocateVex(MGraph G, char ch);

int Minimum(MGraph G, Closedge closedge);

void MiniSpanTree_PRIM(MGraph G, char u);

void Sortdge(Dgevalue & dgevalue, MGraph G);

int getImageMST(cv::Mat Src, cv::Mat Dest);
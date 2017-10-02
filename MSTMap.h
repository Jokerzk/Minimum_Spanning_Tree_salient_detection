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
	char vexs[MAX_VERTEX_NUM];//�ڵ�����
	AdjMatrix arcs;//�ڽӾ���
	int vexnum, arcnum;//ͼ�ĵ�ǰ�ڵ����ͻ���
}MGraph;

typedef struct Pnode//��������ķ�㷨
{
	char adjvex;//�ڵ�
	double lowcost;//Ȩֵ
}Pnode, Closedge[MAX_VERTEX_NUM];//��¼���㼯U��V-U�Ĵ�����С�ıߵĸ������鶨��

typedef struct Knode//���ڿ�³˹�����㷨�д洢һ���߼����Ӧ��2���ڵ�
{
	char ch1;//�ڵ�1
	char ch2;//�ڵ�2
	double value;//Ȩֵ
}Knode, Dgevalue[MAX_VERTEX_NUM];


//-------------------------------------------------------------------------------
int CreateUDG(MGraph & G, Dgevalue & dgevalue);
int LocateVex(MGraph G, char ch);

int Minimum(MGraph G, Closedge closedge);

void MiniSpanTree_PRIM(MGraph G, char u);

void Sortdge(Dgevalue & dgevalue, MGraph G);

int getImageMST(cv::Mat Src, cv::Mat Dest);
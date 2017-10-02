#include"MSTMap.h"




//-------------------------------------------------------------------------------
int CreateUDG(MGraph & G, Dgevalue & dgevalue)//构造无向加权图的邻接矩阵
{
	int i, j, k;
	//cout << "请输入图中节点个数和边/弧的条数：";
	//cin >> G.vexnum >> G.arcnum;
	G.vexnum = 1200;
	G.arcnum = 2400;
	//cout << "请输入节点：";
	for (i = 0; i<G.vexnum; ++i)
		 G.vexs[i] = i;
	for (i = 0; i<G.vexnum; ++i)//初始化数组
	{
		for (j = 0; j<G.vexnum; ++j)
		{
			G.arcs[i][j].adj = MAX;
		}
	}
	//cout << "请输入一条边依附的定点及边的权值：" << endl;
	for (k = 0; k<G.arcnum; ++k)
	{
		//cin >> dgevalue[k].ch1 >> dgevalue[k].ch2 >> dgevalue[k].value;
		//dgevalue[k].ch1 = 
		i = LocateVex(G, dgevalue[k].ch1);
		j = LocateVex(G, dgevalue[k].ch2);
		G.arcs[i][j].adj = dgevalue[k].value;
		G.arcs[j][i].adj = G.arcs[i][j].adj;
	}
	return OK;
}
int LocateVex(MGraph G, char ch)//确定节点ch在图G.vexs中的位置
{
	int a;
	for (int i = 0; i<G.vexnum; i++)
	{
		if (G.vexs[i] == ch)
			a = i;
	}
	return a;
}

void MiniSpanTree_PRIM(MGraph G, char u)//普利姆算法求最小生成树
{
	int i, j, k;
	Closedge closedge;
	k = LocateVex(G, u);
	for (j = 0; j<G.vexnum; j++)
	{
		if (j != k)
		{
			closedge[j].adjvex = u;
			closedge[j].lowcost = G.arcs[k][j].adj;
		}
	}
	closedge[k].lowcost = 0;
	for (i = 1; i<G.vexnum; i++)
	{
		k = Minimum(G, closedge);
		cout << "(" << closedge[k].adjvex << "," << G.vexs[k] << "," << closedge[k].lowcost << ")" << endl;
		closedge[k].lowcost = 0;
		for (j = 0; j<G.vexnum; ++j)
		{
			if (G.arcs[k][j].adj<closedge[j].lowcost)
			{
				closedge[j].adjvex = G.vexs[k];
				closedge[j].lowcost = G.arcs[k][j].adj;
			}
		}
	}
}

int Minimum(MGraph G, Closedge closedge)//求closedge中权值最小的边，并返回其顶点在vexs中的位置
{
	int i, j;
	double k = 1000;
	for (i = 0; i<G.vexnum; i++)
	{
		if (closedge[i].lowcost != 0 && closedge[i].lowcost<k)
		{
			k = closedge[i].lowcost;
			j = i;
		}
	}
	return j;
}

void MiniSpanTree_KRSL(MGraph G, Dgevalue&dgevalue)//克鲁斯卡尔算法求最小生成树
{
	int p1, p2, i, j;
	int bj[MAX_VERTEX_NUM];//标记数组
	for (i = 0; i<G.vexnum; i++)//标记数组初始化
		bj[i] = i;
	Sortdge(dgevalue, G);//将所有权值按从小到大排序
	for (i = 0; i<G.arcnum; i++)
	{
		p1 = bj[LocateVex(G, dgevalue[i].ch1)];
		p2 = bj[LocateVex(G, dgevalue[i].ch2)];
		if (p1 != p2)
		{
			cout << "(" << dgevalue[i].ch1 << "," << dgevalue[i].ch2 << "," << dgevalue[i].value << ")" << endl;
			for (j = 0; j<G.vexnum; j++)
			{
				if (bj[j] == p2)
					bj[j] = p1;
			}
		}
	}
}

void Sortdge(Dgevalue & dgevalue, MGraph G)//对dgevalue中各元素按权值按从小到大排序
{
	int i, j;
	double temp;
	char ch1, ch2;
	for (i = 0; i<G.arcnum; i++)
	{
		for (j = i; j<G.arcnum; j++)
		{
			if (dgevalue[i].value>dgevalue[j].value)
			{
				temp = dgevalue[i].value;
				dgevalue[i].value = dgevalue[j].value;
				dgevalue[j].value = temp;
				ch1 = dgevalue[i].ch1;
				dgevalue[i].ch1 = dgevalue[j].ch1;
				dgevalue[j].ch1 = ch1;
				ch2 = dgevalue[i].ch2;
				dgevalue[i].ch2 = dgevalue[j].ch2;
				dgevalue[j].ch2 = ch2;
			}
		}
	}
}

int getImageMST(cv::Mat Src,cv::Mat Dest)
{
	int i, j;
	MGraph G;
	char u;
	Dgevalue dgevalue;
	CreateUDG(G, dgevalue);
	cout << "图的邻接矩阵为：" << endl;
	for (i = 0; i<G.vexnum; i++)
	{
		for (j = 0; j<G.vexnum; j++)
			cout << G.arcs[i][j].adj << "";
		cout << endl;
	}
	cout << "=============普利姆算法===============\n";
	cout << "请输入起始点：";
	cin >> u;
	cout << "构成最小代价生成树的边集为：\n";
	MiniSpanTree_PRIM(G, u);
	cout << "============克鲁斯科尔算法=============\n";
	cout << "构成最小代价生成树的边集为：\n";
	MiniSpanTree_KRSL(G, dgevalue);
	return 0;
}
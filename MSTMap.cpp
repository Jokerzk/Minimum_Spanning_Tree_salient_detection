#include"MSTMap.h"




//-------------------------------------------------------------------------------
int CreateUDG(MGraph & G, Dgevalue & dgevalue)//���������Ȩͼ���ڽӾ���
{
	int i, j, k;
	//cout << "������ͼ�нڵ�����ͱ�/����������";
	//cin >> G.vexnum >> G.arcnum;
	G.vexnum = 1200;
	G.arcnum = 2400;
	//cout << "������ڵ㣺";
	for (i = 0; i<G.vexnum; ++i)
		 G.vexs[i] = i;
	for (i = 0; i<G.vexnum; ++i)//��ʼ������
	{
		for (j = 0; j<G.vexnum; ++j)
		{
			G.arcs[i][j].adj = MAX;
		}
	}
	//cout << "������һ���������Ķ��㼰�ߵ�Ȩֵ��" << endl;
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
int LocateVex(MGraph G, char ch)//ȷ���ڵ�ch��ͼG.vexs�е�λ��
{
	int a;
	for (int i = 0; i<G.vexnum; i++)
	{
		if (G.vexs[i] == ch)
			a = i;
	}
	return a;
}

void MiniSpanTree_PRIM(MGraph G, char u)//����ķ�㷨����С������
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

int Minimum(MGraph G, Closedge closedge)//��closedge��Ȩֵ��С�ıߣ��������䶥����vexs�е�λ��
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

void MiniSpanTree_KRSL(MGraph G, Dgevalue&dgevalue)//��³˹�����㷨����С������
{
	int p1, p2, i, j;
	int bj[MAX_VERTEX_NUM];//�������
	for (i = 0; i<G.vexnum; i++)//��������ʼ��
		bj[i] = i;
	Sortdge(dgevalue, G);//������Ȩֵ����С��������
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

void Sortdge(Dgevalue & dgevalue, MGraph G)//��dgevalue�и�Ԫ�ذ�Ȩֵ����С��������
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
	cout << "ͼ���ڽӾ���Ϊ��" << endl;
	for (i = 0; i<G.vexnum; i++)
	{
		for (j = 0; j<G.vexnum; j++)
			cout << G.arcs[i][j].adj << "";
		cout << endl;
	}
	cout << "=============����ķ�㷨===============\n";
	cout << "��������ʼ�㣺";
	cin >> u;
	cout << "������С�����������ı߼�Ϊ��\n";
	MiniSpanTree_PRIM(G, u);
	cout << "============��³˹�ƶ��㷨=============\n";
	cout << "������С�����������ı߼�Ϊ��\n";
	MiniSpanTree_KRSL(G, dgevalue);
	return 0;
}
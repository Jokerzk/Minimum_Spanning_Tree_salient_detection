#include"BoundaryDissimilarityMap.h"
#define PROFILING


int getBoundaryDissimilarityMap(cv::Mat Src, int boundarysize, float* Dest)
{
	int width = Src.cols;
	int height = Src.rows;
	int size = width*height;
	/*获取当前图片的Lab颜色空间数据*/
	//float *imgLab = (float *)malloc(3 * height * width * sizeof(float));
	//RGBToLABF(Src, imgLab, Src.rows, Src.cols, 3);

	cv::Mat Labimg = Mat(Src.rows, Src.cols, CV_32FC3, Scalar(0));
	cv::cvtColor(Src, Labimg, CV_BGR2Lab);

	Group CurrentImg;
	KMeans(Labimg, width, height, boundarysize, CurrentImg.clusters, CurrentImg.means);

	getCovmatrixInvert(width,height,CurrentImg.clusters,CurrentImg.covinvert);//此处得到协方差矩阵的逆；
	
	/*cv::Mat BD_ori = Mat(height, width, Src.type(), Scalar(0));
	cv::Mat Cen_ori = Mat(height, width, Src.type(), Scalar(0));
	cv::Mat central = Mat(height - boundarysize * 2, width - boundarysize * 2, Src.type(), Scalar(0));
	cv::Rect centerrect = Rect(boundarysize,boundarysize, width - boundarysize * 2, height - boundarysize * 2);
	central = Src(centerrect);
	central.copyTo(Cen_ori(centerrect));
	BD_ori = Src - Cen_ori;
	cv::imshow("BDMap_ori", BD_ori);
	cv::Mat Kmeansmap = Mat(height, width, CV_32FC3, Scalar(255,255,255));
	cv::Mat LabMat = Mat(height, width, CV_32FC3, imgLab);
	int clusterloc;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (i >= boundarysize&& i <= height - boundarysize && j >= boundarysize && j < width - boundarysize)
				continue;
			double var =0,max =100000;

			for (int w = 0; w < k; w++)
			{
				var = (imgLab[(i*width + j) * 3 + 0] - CurrentImg.means[w].attrL)*(imgLab[(i*width + j) * 3 + 0] - CurrentImg.means[w].attrL) +
					  (imgLab[(i*width + j) * 3 + 1] - CurrentImg.means[w].attra)*(imgLab[(i*width + j) * 3 + 1] - CurrentImg.means[w].attra) +
					  (imgLab[(i*width + j) * 3 + 2] - CurrentImg.means[w].attrb)*(imgLab[(i*width + j) * 3 + 2] - CurrentImg.means[w].attrb);
				if (var < max)
				{
					max = var;
					clusterloc = w;
				}
			}
			if (clusterloc ==0)
			{
				Kmeansmap.data[i*width * 3 + j * 3 + 0] = 255;
				Kmeansmap.data[i*width * 3 + j * 3 + 1] = 0;
				Kmeansmap.data[i*width * 3 + j * 3 + 2] = 0;
			}
			else if (clusterloc == 1)
			{
				Kmeansmap.data[i*width * 3 + j * 3 + 0] = 0;
				Kmeansmap.data[i*width * 3 + j * 3 + 1] = 255;
				Kmeansmap.data[i*width * 3 + j * 3 + 2] = 0;
			}
			else if (clusterloc == 1)
			{
				Kmeansmap.data[i*width * 3 + j * 3 + 0] = 0;
				Kmeansmap.data[i*width * 3 + j * 3 + 1] = 0;
				Kmeansmap.data[i*width * 3 + j * 3 + 2] = 255;
			}
			else
			{
				Kmeansmap.data[i*width * 3 + j * 3 + 0] = 255;
				Kmeansmap.data[i*width * 3 + j * 3 + 1] = 255;
				Kmeansmap.data[i*width * 3 + j * 3 + 2] = 255;
			}
		}
	}
	cv::imshow("Kmeans results", Kmeansmap);*/
	if (!flag)
		return 0;
	else
	{
		/*计算马氏距离*/
		float* MahalMap = (float *)malloc(height * width * sizeof(float));
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j += 1)
			{
				float S[k];
				for (int g = 0; g < k; g++)
				{

					float submatrix[3];
					submatrix[0] = Labimg.data[(i*width + j)*3 + 0] - CurrentImg.means[g].attrL;
					submatrix[1] = Labimg.data[(i*width + j) * 3 + 1] - CurrentImg.means[g].attra;
					submatrix[2] = Labimg.data[(i*width + j) * 3 + 2] - CurrentImg.means[g].attrb;

					S[g] = sqrt(submatrix[0] * (submatrix[0] * CurrentImg.covinvert[g][0][0] + submatrix[1] * CurrentImg.covinvert[g][1][0] + submatrix[2] * CurrentImg.covinvert[g][2][0]) +
						submatrix[1] * (submatrix[0] * CurrentImg.covinvert[g][0][1] + submatrix[1] * CurrentImg.covinvert[g][1][1] + submatrix[2] * CurrentImg.covinvert[g][2][1]) +
						submatrix[2] * (submatrix[0] * CurrentImg.covinvert[g][0][2] + submatrix[1] * CurrentImg.covinvert[g][1][2] + submatrix[2] * CurrentImg.covinvert[g][2][2]));
				}
				MahalMap[i*width + j] = (CurrentImg.clusters[0].size()*S[0] + CurrentImg.clusters[1].size()*S[1] + CurrentImg.clusters[2].size()*S[2]) / (CurrentImg.clusters[0].size() + CurrentImg.clusters[1].size() + CurrentImg.clusters[2].size());
				//cout << MahalMap[i*width + j] << endl;
			}
		}
		Normalize(MahalMap, Dest, width,height,255);
	}
	//cv::Mat gray = Mat(height, width, CV_8UC1, Scalar(0));
	//for (int i = 0; i < height; i++)
	//{
	//	for (int j = 0; j < width; j++)
	//	{
	//		//cout << Dest[i*width+j] << endl;
	//		//printf("%f\n", salmat.at<float>(i, j));
	//		if (Dest[i*width+j] >= 136)
	//		{
	//			gray.data[i*width + j] = 255;
	//		}
	//		else
	//		{
	//			gray.data[i*width + j] = 0;
	//		}
	//	}
	//}
	//cv::imshow("Src segmentation on BD", gray);
	return 0;
}
 
//获得当前簇集的协方差逆矩阵
void getCovmatrixInvert(int width, int height, vector<Tuple> clusters[k], float covinvert[k][3][3])
{
	float cov[k][3][3];
	for (int  j = 0; j < k; j++)
	{
		int num = clusters[j].size();
		vector <Mat> data(k), covar(k), mean(k);
		data[j] = Mat(num, 3, CV_32FC1);
		double sumLL = 0, sumLa = 0, sumLb = 0, sumaL = 0, sumaa = 0, sumab = 0, sumbL = 0, sumba = 0, sumbb = 0;
		for (int i = 0; i < num; i++)
		{
			data[j].at<float>(i, 0) = clusters[j][i].attrL;
			data[j].at<float>(i, 1) = clusters[j][i].attra;
			data[j].at<float>(i, 2) = clusters[j][i].attrb;
		}
		calcCovarMatrix(data[j], covar[j], mean[j], CV_COVAR_NORMAL | CV_COVAR_ROWS, CV_32F);
		CvMat cvcovar= covar[j];
		if (cvDet(&cvcovar) == 0)//判断是否存在矩阵行列式为0，若存在则由马氏距离所求距离全部为0（矩阵奇异）；
			flag = false;
		CvMat *covarinvert = cvCreateMat(3, 3, CV_32FC1);
		cvInvert(&cvcovar, covarinvert);
		for (int row = 0; row<covarinvert->height; row++)
		{
			float* pData = (float*)(covarinvert->data.ptr + row*covarinvert->step);
			for (int col = 0; col<covarinvert->width; col++)
			{
				covinvert[j][row][col] = *pData;
				pData++;
			}
		}
	}
}

//计算两个元组在Lab空间内的欧几里得距离  
float getDistLab(Tuple t1, Tuple t2)
{
	return sqrt((t1.attrL - t2.attrL) * (t1.attrL - t2.attrL) + (t1.attra - t2.attra) * (t1.attra - t2.attra) + (t1.attrb - t2.attrb) * (t1.attrb - t2.attrb));
}

//根据质心，决定当前元组属于哪个簇  
int clusterOfTuple(Tuple means[], Tuple tuple)//tuple:当前元素
{
	float dist = getDistLab(means[0], tuple);
	float tmp;
	int label = 0;//标示属于哪一个簇  
	for (int i = 1; i<k; i++)
	{
		tmp = getDistLab(means[i], tuple);
		if (tmp<dist) 
		{ dist = tmp; label = i; }
	}
	return label;
}

//获得给定簇集的平方误差,用来确定收敛界限  
float getVar(vector<Tuple> clusters[], Tuple means[])
{
	float var = 0;
	for (int i = 0; i < k; i++)
	{
		vector<Tuple> t = clusters[i];
		for (int j = 0; j< t.size(); j++)
		{
			var += getDistLab(t[j], means[i]);
		}
	}
	return var;
}
//获得当前簇的均值（质心）
Tuple getMeans(vector<Tuple> cluster)
{

	int num = cluster.size();
	double meansX = 0, meansY = 0, meansZ = 0;
	Tuple t;
	for (int i = 0; i < num; i++)
	{
		meansX += cluster[i].attrL;
		meansY += cluster[i].attra;
		meansZ += cluster[i].attrb;
	}
	t.attrL = meansX / num;
	t.attra = meansY / num;
	t.attrb = meansZ / num;
	return t;
}
//获取当前图片的种子，选取左上左下右上右下四个像素作为聚类种子
vector<Tuple> getSeeds(cv::Mat imgLab, int width, int height, vector<Tuple> Seeds)
{
	int size = width * height;
	Seeds[0].attrL = imgLab.data[3 * int((width - 1) * 0.5) + 0];
	Seeds[0].attra = imgLab.data[3 * int((width - 1) * 0.5) + 1];
	Seeds[0].attrb = imgLab.data[3 * int((width - 1) * 0.5) + 2];
	Seeds[1].attrL = imgLab.data[3 * width * int((height - 1) * 0.5) + 0];
	Seeds[1].attra = imgLab.data[3 * width * int((height - 1) * 0.5) + 1];
	Seeds[1].attrb = imgLab.data[3 * width * int((height - 1) * 0.5) + 2];
	Seeds[2].attrL = imgLab.data[3 * (size - 1) + 0];
	Seeds[2].attra = imgLab.data[3 * (size - 1) + 1];
	Seeds[2].attrb = imgLab.data[3 * (size - 1) + 2];
	return Seeds;
}
vector<Tuple>* KMeans(cv::Mat imgLab, int width, int height, int boundarysize, vector<Tuple> clusters[k], Tuple means[k])
{
	int datanum = width * height - (width - boundarysize*2)*(height - boundarysize*2);
	vector<Tuple> tuples;
	for (int i = 0; i < height; i++)
	{
		Tuple temp;
		if (i < boundarysize || i >= (height - boundarysize))
		{
			for (int j = 0; j < width; j++)
			{
				temp.attrL = imgLab.data[(i*width + j) * 3 + 0];
				temp.attra = imgLab.data[(i*width + j) * 3 + 1];
				temp.attrb = imgLab.data[(i*width + j) * 3 + 2];
				tuples.push_back(temp);
			}
		}
		else
		{
			for (int j = 0; j < width; j++)
			{
				if (j<boundarysize || j >= (width - boundarysize))
				{
					temp.attrL = imgLab.data[(i*width + j) * 3 + 0];
					temp.attra = imgLab.data[(i*width + j) * 3 + 1];
					temp.attrb = imgLab.data[(i*width + j) * 3 + 2];
					tuples.push_back(temp);
				}	
			}
		}
	}
	/*此处预先指定四个边缘点为3个簇的质心*/
	vector<Tuple> seeds(k);
	seeds = getSeeds(imgLab, width, height, seeds);
	for (int i = 0; i < k; i++)
	{
		means[i].attrL = seeds[i].attrL;
		means[i].attra = seeds[i].attra;
		means[i].attrb = seeds[i].attrb;
	}
	int lable = 0;
	/*根据默认的质心给簇赋值*/ 
	for (int i = 0; i != datanum; ++i)
	{
		lable = clusterOfTuple(means, tuples[i]);
		clusters[lable].push_back(tuples[i]);
	}
	float oldVar = -1;
	float newVar = getVar(clusters, means);
	/*当新旧函数值相差不到1即准则函数值不发生明显变化时，算法终止*/
	while (abs(newVar - oldVar) >= 10) 
	{
		for (int i = 0; i < k; i++) 
		{
			means[i] = getMeans(clusters[i]); 
		}
		oldVar = newVar;
		newVar = getVar(clusters, means);
		for (int i = 0; i < k; i++)   
		{
			clusters[i].clear();
		}
		/*根据新的质心获得新的簇*/
		for (int i = 0; i != tuples.size(); ++i)
		{
			lable = clusterOfTuple(means, tuples[i]);
			clusters[lable].push_back(tuples[i]);
		}
	}
	return clusters;
}

#include "FT.h"

void RGBToLABF(cv::Mat Src, float *LABdest, const int height, const int width, const int channels)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width*channels; j += 3)
		{
			int sR = Src.data[i*width*channels + j + 0];
			int sG = Src.data[i*width*channels + j + 1];
			int sB = Src.data[i*width*channels + j + 2];

			double R = sR / 255.0;
			double G = sG / 255.0;
			double B = sB / 255.0;

			double r, g, b;

			if (R <= 0.04045)	r = R / 12.92;
			else				r = pow((R + 0.055) / 1.055, 2.4);
			if (G <= 0.04045)	g = G / 12.92;
			else				g = pow((G + 0.055) / 1.055, 2.4);
			if (B <= 0.04045)	b = B / 12.92;
			else				b = pow((B + 0.055) / 1.055, 2.4);

			double X = r*0.4124564 + g*0.3575761 + b*0.1804375;
			double Y = r*0.2126729 + g*0.7151522 + b*0.0721750;
			double Z = r*0.0193339 + g*0.1191920 + b*0.9503041;
			//------------------------
			// XYZ to LAB conversion
			//------------------------
			double epsilon = 0.008856;	//actual CIE standard
			double kappa = 903.3;		//actual CIE standard

			double Xr = 0.950456;	//reference white
			double Yr = 1.0;		//reference white
			double Zr = 1.088754;	//reference white

			double xr = X / Xr;
			double yr = Y / Yr;
			double zr = Z / Zr;

			double fx, fy, fz;
			if (xr > epsilon)	fx = pow(xr, 1.0 / 3.0);
			else				fx = (kappa*xr + 16.0) / 116.0;
			if (yr > epsilon)	fy = pow(yr, 1.0 / 3.0);
			else				fy = (kappa*yr + 16.0) / 116.0;
			if (zr > epsilon)	fz = pow(zr, 1.0 / 3.0);
			else				fz = (kappa*zr + 16.0) / 116.0;

			LABdest[i*width*channels + j + 0] = 116.0*fy - 16.0;
			LABdest[i*width*channels + j + 1] = 500.0*(fx - fy);
			LABdest[i*width*channels + j + 2] = 200.0*(fy - fz);
		}
	}
}

void GaussSmooth(float *Dataspace, float*Dataset, const int width, const int height, const int channels)
{
	vector<double> kernel = { 1.0, 2.0, 1.0 };//高斯核
	int center = int(kernel.size()) / 2;
	int sz = width*height;
	double kernelsum = 4.0;
	vector<double> tempdatal, tempdataa, tempdatab;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j += 1)
		{
			double suml(0), suma(0), sumb(0);
			for (int cc = (-center); cc <= center; cc++)
			{
				if (((j + cc)*channels >= 0) && ((j + cc)* channels < width*channels))
				{
					suml += Dataspace[i*width*channels + (j + cc)*channels + 0] * kernel[center + cc];
					suma += Dataspace[i*width*channels + (j + cc)*channels + 1] * kernel[center + cc];
					sumb += Dataspace[i*width*channels + (j + cc)*channels + 2] * kernel[center + cc];

				}
			}
			tempdatal.push_back(suml / kernelsum);
			tempdataa.push_back(suma / kernelsum);
			tempdatab.push_back(sumb / kernelsum);
		}
	}
	
	int index(0);
	for (int j = 0; j < width; j++)
	{
		for (int i = 0; i < height; i++)
		{
			double suml(0), suma(0), sumb(0);
			for (int rr = (-center); rr <= center; rr++)
			{
				if (((i + rr) >= 0) && ((i + rr) < height))
				{
					suml += tempdatal[(i + rr)*width + j] * kernel[center + rr];
					suma += tempdataa[(i + rr)*width + j] * kernel[center + rr];
					sumb += tempdatab[(i + rr)*width + j] * kernel[center + rr];
				}
			}
			Dataset[index*channels + 0] = suml / kernelsum;
			Dataset[index*channels + 1] = suma / kernelsum;
			Dataset[index*channels + 2] = sumb / kernelsum;
			index++;
		}
	}

}

void Normalize(float* input, float* output, const int width, const int height, const int normrange)
{
	double maxval(0);
	double minval(DBL_MAX);
	{int i(0);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (maxval < input[i]) maxval = input[i];
			if (minval > input[i]) minval = input[i];
			i++;
		}
	}}
	double range = maxval - minval;
	if (0 == range) range = 1;
	int i(0);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			output[i] = ((normrange*(input[i] - minval)) / range);
			i++;
		}
	}
}

void SalientBasedOnFT(cv::Mat Src, float *SaliencyMap, int Width, int Height)
{
	int X, Y, Index, CurIndex;
	float *Lab = (float *)malloc(Height * Width*3 * sizeof(float));
	float *Blur = (float *)malloc(Height * Width*3 * sizeof(float));
	float *DistMap = (float *)malloc(Height * Width * sizeof(float));
	float MeanL = 0, MeanA = 0, MeanB = 0;

	cv::Mat Labimg = Mat(Src.rows,Src.cols,CV_32FC3,Scalar(0));
	cv::cvtColor(Src, Labimg, CV_BGR2Lab);
	//RGBToLABF(Src, Lab, Height, Width,3);                //    浮点类型的数据转换

	for (Y = 0; Y < Height; Y++)
	{
		Index = Y * Width*3;
		for (X = 0; X < Width; X++)
		{
			MeanL += Labimg.data[Index]*100/255;
			MeanA += Labimg.data[Index + 1]-128;
			MeanB += Labimg.data[Index + 2]-128;
			Index += 3;
		}
	}
	MeanL /= (Width * Height);                                            //    求LAB空间的平均值
	MeanA /= (Width * Height);
	MeanB /= (Width * Height);

	//GaussianBlur(Labimg, Labimg,cv::Size(3,3),0,0);
	//GaussSmooth(Lab, Blur, Width, Height,3);                            //    use Gaussian blur to eliminate ﬁne texture details as well as noise and coding artifacts

	for (Y = 0; Y < Height; Y++) 
	{
		Index = Y * Width*3;
		CurIndex = Y * Width;
		for (X = 0; X < Width; X++)                                        //    计算像素的显著性
		{
			//SaliencyMap[Y*Width + X] = sqrt((MeanL - Blur[Index]) *  (MeanL - Blur[Index]) + (MeanA - Blur[Index + 1]) *  (MeanA - Blur[Index + 1]) + (MeanB - Blur[Index + 2]) *  (MeanB - Blur[Index + 2]));
			SaliencyMap[Y*Width + X] = (MeanL - (Labimg.data[Index] * 100 / 255)) *  (MeanL - (Labimg.data[Index] * 100 / 255)) +
									   (MeanA - (Labimg.data[Index + 1] - 128)) *  (MeanA - (Labimg.data[Index + 1] - 128)) +
								       (MeanB - (Labimg.data[Index + 2] - 128)) *  (MeanB - (Labimg.data[Index + 2] - 128));
			Index += 3;

		}
	}
	Normalize(SaliencyMap, SaliencyMap, Width, Height,255);

	//cv::Mat gray = Mat(Src.rows, Src.cols, CV_8UC1, Scalar(0));
	//for (int i = 0; i < Src.rows; i++)
	//{
	//	for (int j = 0; j < Src.cols; j++)
	//	{
	//		//cout << Dest[i*width+j] << endl;
	//		//printf("%f\n", salmat.at<float>(i, j));
	//		if (SaliencyMap[i*Src.cols + j] >= 136)
	//		{
	//			gray.data[i*Src.cols + j] = 255;
	//		}
	//		else
	//		{
	//			gray.data[i*Src.cols + j] = 0;
	//		}
	//	}
	//}
	//cv::imshow("Src segmentation on FT", gray);

	free(Lab);
	free(DistMap);

}

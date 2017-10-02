#include "common.h"

void RGBToLABF(cv::Mat Src, float *LABdest, const int height, const int width, const int channels);

void GaussSmooth(float *Dataspace, float*Dataset, const int width, const int height, const int channels);

void Normalize(float* input, float* output, const int width, const int height, const int normrange);

void SalientBasedOnFT(cv::Mat Src, float *SaliencyMap, int Width, int Height);
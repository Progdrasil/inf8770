#include "bloc.hpp"

std::vector<cv::Mat_<uchar>> matrix2blocks(const cv::Mat_<uchar> &inMat)
{
	std::vector<cv::Mat_<uchar>> outBlocks;

	outBlocks.reserve(inMat.rows / 8 * inMat.cols / 8);

	for (int k = 0; k < inMat.rows; k+=8)
	{
		for (int l = 0; l < inMat.cols; l+=8)
		{
			cv::Mat_<uchar> tmp;
			tmp.create(8, 8);
			inMat(cv::Rect(l, k, 8, 8)).copyTo(tmp);
			outBlocks.push_back(tmp);
		}
	}

	return outBlocks;
}

cv::Mat_<uchar> blocks2matrix(const std::vector<cv::Mat_<uchar>> inBlocks, const cv::Size &inImageSize)
{
	cv::Mat_<uchar> outMatrix(inImageSize);

	int nBlock = 0;
	for (int i = 0; i < outMatrix.rows; i += 8)
	{
		for (int j = 0; j < outMatrix.cols; j += 8)
		{
			cv::Mat sub = outMatrix.rowRange(i, i + 8).colRange(j, j + 8);
			inBlocks[nBlock++].copyTo(sub);
		}
	}

	return outMatrix;
}

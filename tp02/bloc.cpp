#include "bloc.hpp"

std::vector<cv::Mat_<uchar>> matrix2block(cv::Mat_<uchar> &inMat)
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

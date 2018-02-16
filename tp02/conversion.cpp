#include "conversion.hpp"

// OpenCV reads images as BGR, not RGB
void bgr2ycbcr(cv::Mat &bgrIn, cv::Mat_<uchar> &yOut, cv::Mat_<uchar> &cbOut, cv::Mat_<uchar> &crOut)
{
	cv::Size sizeIn = bgrIn.size();
	int rowsIn = sizeIn.height;
	int colsIn = sizeIn.width;

	// Conversion BGR YCrCb
	cv::Mat ycrcbIn;
    cv::cvtColor(bgrIn, ycrcbIn, cv::COLOR_BGR2YCrCb);

	yOut.create(sizeIn);
	cbOut.create(rowsIn / 2, colsIn / 2);
	crOut.create(rowsIn / 2, colsIn / 2);

	for (int i = 0; i < rowsIn; i++)
	{
		for (int j = 0; j < colsIn; j++)
		{
			yOut.at<uchar>(i, j) = ycrcbIn.at<cv::Vec3b>(i, j)[0];
			// Subsampling
			if (i % 2 == 0 && j % 2 == 0)
			{
				crOut.at<uchar>(i / 2, j / 2) = ycrcbIn.at<cv::Vec3b>(i, j)[1];
				cbOut.at<uchar>(i / 2, j / 2) = ycrcbIn.at<cv::Vec3b>(i, j)[2];
			}
		}
	}
}

void ycbcr2bgr(cv::Mat_<uchar> &yIn, cv::Mat_<uchar> &cbIn, cv::Mat_<uchar> &crIn, cv::Mat_<uchar> &bgrOut)
{
	cv::Size sizeIn = yIn.size();
	int rowsIn = sizeIn.height;
	int colsIn = sizeIn.width;

	cv::Mat ycrcbOut;

	ycrcbOut.create(sizeIn, CV_8UC3);

	for (int i = 0; i < rowsIn; i++)
	{
		for (int j = 0; j < colsIn; j++)
		{
			ycrcbOut.template at<cv::Vec3b>(i, j)[0] = yIn.template at<uchar>(i, j);

			int x, y;
			if (i % 2 != 0)
				x = (i - 1) / 2;
			else
				x = i / 2;

			if (j % 2 != 0)
				y = (j - 1) / 2;
			else
				y = j / 2;

			ycrcbOut.template at<cv::Vec3b>(i, j)[1] = crIn.template at<uchar>(x, y);
			ycrcbOut.template at<cv::Vec3b>(i, j)[2] = cbIn.template at<uchar>(x, y);
		}
	}

	// Conversion YCrCb BGR
	cv::cvtColor(ycrcbOut, bgrOut, cv::COLOR_YCrCb2BGR);
}

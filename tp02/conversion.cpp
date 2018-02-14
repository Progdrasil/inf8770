#include "conversion.hpp"

// OpenCV reads images as BGR, not RGB
void bgr2ycbcr(cv::Mat &bgrIn, cv::Mat &yOut, cv::Mat &cbOut, cv::Mat &crOut)
{
    cv::Size sizeIn = bgrIn.size();
	int rowsIn = sizeIn.height;
	int colsIn = sizeIn.width;

	// Conversion BGR YCrCb
	cv::Mat ycrcbIn;
    cv::cvtColor(bgrIn, ycrcbIn, cv::COLOR_BGR2YCrCb);

	yOut.create(sizeIn, CV_8UC1);
	cbOut.create(rowsIn / 2, colsIn / 2, CV_8UC1);
	crOut.create(rowsIn / 2, colsIn / 2, CV_8UC1);

	for (int i = 0; i < rowsIn; i++)
	{
		for (int j = 0; j < colsIn; j++)
		{
			yOut.at<uint8_t>(i, j) = ycrcbIn.at<cv::Vec3b>(i, j)[0];
			// Subsampling
			if (i % 2 == 0 && j % 2 == 0)
			{
				crOut.at<uint8_t>(i / 2, j / 2) = ycrcbIn.at<cv::Vec3b>(i, j)[1];
				cbOut.at<uint8_t>(i / 2, j / 2) = ycrcbIn.at<cv::Vec3b>(i, j)[2];
			}
		}
	}
}


void ycbcr2bgr(cv::Mat& yIn, cv::Mat& cbIn, cv::Mat& crIn, cv::Mat& bgrOut)
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
			ycrcbOut.at<cv::Vec3b>(i, j)[0] = yIn.at<uint8_t>(i, j);

			int x, y;
			if (i % 2 != 0)
				x = (i - 1) / 2;
			else
				x = i / 2;

			if (j % 2 != 0)
				y = (j - 1) / 2;
			else
				y = j / 2;

			ycrcbOut.at<cv::Vec3b>(i, j)[1] = crIn.at<uint8_t>(x, y);
			ycrcbOut.at<cv::Vec3b>(i, j)[2] = cbIn.at<uint8_t>(x, y);
		}
	}

	// Conversion YCrCb BGR
	cv::cvtColor(ycrcbOut, bgrOut, cv::COLOR_YCrCb2BGR);
}

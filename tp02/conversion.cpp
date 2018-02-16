#include "conversion.hpp"

// OpenCV reads images as BGR, not RGB
void bgr2ycbcr(const cv::Mat &bgrIn, cv::Mat_<uchar> &yOut, cv::Mat_<uchar> &cbOut, cv::Mat_<uchar> &crOut, bool subsample)
{
	cv::Size sizeIn = bgrIn.size();
	int rowsIn = sizeIn.height;
	int colsIn = sizeIn.width;

	// Conversion BGR YCrCb
	cv::Mat ycrcbIn;
    cv::cvtColor(bgrIn, ycrcbIn, cv::COLOR_BGR2YCrCb);

	yOut.create(sizeIn);
	if(subsample)
	{
		cbOut.create(rowsIn / 2, colsIn / 2);
		crOut.create(rowsIn / 2, colsIn / 2);
	}
	else
	{
		cbOut.create(rowsIn, colsIn);
		crOut.create(rowsIn, colsIn);
	}

	for (int i = 0; i < rowsIn; i++)
	{
		for (int j = 0; j < colsIn; j++)
		{
			yOut.at<uchar>(i, j) = ycrcbIn.at<cv::Vec3b>(i, j)[0];
			// Subsampling
			if (subsample && i % 2 == 0 && j % 2 == 0)
			{
				crOut.at<uchar>(i / 2, j / 2) = ycrcbIn.at<cv::Vec3b>(i, j)[1];
				cbOut.at<uchar>(i / 2, j / 2) = ycrcbIn.at<cv::Vec3b>(i, j)[2];
			}
			else if(!subsample)
			{
				crOut.at<uchar>(i, j) = ycrcbIn.at<cv::Vec3b>(i, j)[1];
				cbOut.at<uchar>(i, j) = ycrcbIn.at<cv::Vec3b>(i, j)[2];
			}
		}
	}
}

cv::Mat ycbcr2bgr(const cv::Mat_<uchar> &yIn, const cv::Mat_<uchar> &cbIn, const cv::Mat_<uchar> &crIn)
{
	cv::Mat_<cv::Vec3b> ycrcbOut;

	ycrcbOut.create(yIn.size());
	bool subsampling = (yIn.size() != cbIn.size());

	for (int i = 0; i < yIn.rows; i++)
	{
		for (int j = 0; j < yIn.cols; j++)
		{
			ycrcbOut.at<cv::Vec3b>(i, j)[0] = yIn.at<uchar>(i, j);
			if(subsampling)
			{
				int x, y;
				if (i % 2 != 0)
					x = (i - 1) / 2;
				else
					x = i / 2;

				if (j % 2 != 0)
					y = (j - 1) / 2;
				else
					y = j / 2;

				ycrcbOut.at<cv::Vec3b>(i, j)[1] = crIn.at<uchar>(x, y);
				ycrcbOut.at<cv::Vec3b>(i, j)[2] = cbIn.at<uchar>(x, y);
			}
			else
			{
				ycrcbOut.at<cv::Vec3b>(i, j)[1] = crIn.at<uchar>(i, j);
				ycrcbOut.at<cv::Vec3b>(i, j)[2] = cbIn.at<uchar>(i, j);
			}
		}
	}

	// Conversion YCrCb BGR
	cv::Mat bgrOut;
	cv::cvtColor(ycrcbOut, bgrOut, cv::COLOR_YCrCb2BGR);
	return bgrOut;
}

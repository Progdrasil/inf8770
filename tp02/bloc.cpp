#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void ycrcb2block(cv::Mat &yIn, cv::Mat &crIn, cv::Mat &cbIn, std::vector<cv::Mat> &blocksY, std::vector<cv::Mat> &blocksCr, std::vector<cv::Mat> &blocksCb);

void ycrcb2block(cv::Mat &yIn, cv::Mat &crIn, cv::Mat &cbIn, std::vector<cv::Mat> &blocksY, std::vector<cv::Mat> &blocksCr, std::vector<cv::Mat> &blocksCb)
{
	cv::Size sizeIn = yIn.size();
	int rowsIn = sizeIn.height;
	int colsIn = sizeIn.width;

	for (int k = 0; k * 8 < rowsIn; k++)
	{
		for (int l = 0; l * 8 < colsIn; l++)
		{
			cv::Point topLeft8(k * 8, l * 8);
			cv::Point bottomRight8((k + 1) * 8 - 1, (l + 1) * 8 - 1);
			cv::Rect bloc8(topLeft8, bottomRight8);

			cv::Point topLeft4(k * 4, l * 4);
			cv::Point bottomRight4((k + 1) * 4 - 1, (l + 1) * 4 - 1);
			cv::Rect bloc4(topLeft4, bottomRight4);

			cv::Mat blockY = yIn(bloc8);
			cv::Mat blockCr = crIn(bloc4);
			cv::Mat blockCb = cbIn(bloc4);

			blocksY.push_back(blockY);
			blocksCr.push_back(blockCr);
			blocksCb.push_back(blockCb);
		}
	}
}

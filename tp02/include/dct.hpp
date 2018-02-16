#ifndef DCT_HPP
#define DCT_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

std::vector<cv::Mat_<float>> block2dct(std::vector<cv::Mat_<uchar>> &inBlocks);
cv::Mat_<float> dct(cv::Mat_<uchar> inBlock);

#endif

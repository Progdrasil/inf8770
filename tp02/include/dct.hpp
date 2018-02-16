#ifndef DCT_HPP
#define DCT_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

std::vector<cv::Mat_<float>> blocks2dct(const std::vector<cv::Mat_<uchar>> &inBlocks);
std::vector<cv::Mat_<uchar>> dct2blocks(const std::vector<cv::Mat_<float>> &inDct);
cv::Mat_<float> dct(const cv::Mat_<uchar> inBlock);
cv::Mat_<uchar> invdct(const cv::Mat_<float> inDct);

#endif

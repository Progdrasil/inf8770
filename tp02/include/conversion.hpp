#ifndef CONVERSION_HPP
#define CONVERSION_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void bgr2ycbcr(const cv::Mat &bgrIn, cv::Mat_<uchar> &yOut, cv::Mat_<uchar> &cbOut, cv::Mat_<uchar> &crOut, bool subsample);
cv::Mat ycbcr2bgr(const cv::Mat_<uchar> &yIn, const cv::Mat_<uchar> &cbIn, const cv::Mat_<uchar> &crIn);

#endif

#ifndef CONVERSION_HPP
#define CONVERSION_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void bgr2ycbcr(cv::Mat &bgrIn, cv::Mat_<uchar> &yOut, cv::Mat_<uchar> &cbOut, cv::Mat_<uchar> &crOut);
void ycbcr2bgr(cv::Mat_<uchar> &yIn, cv::Mat_<uchar> &cbIn, cv::Mat_<uchar> &crIn, cv::Mat_<uchar> &bgrOut);

#endif

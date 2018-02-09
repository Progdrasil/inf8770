#ifndef CONVERSION_HPP
#define CONVERSION_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void bgr2ycbcr(cv::Mat &bgrIn, cv::Mat &yOut, cv::Mat &cbOut, cv::Mat &crOut);
void ycbcr2bgr(cv::Mat& yIn, cv::Mat& cbIn, cv::Mat& crIn, cv::Mat& bgrOut);

#endif

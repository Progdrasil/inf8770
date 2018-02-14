#ifndef CONVERSION_HPP
#define CONVERSION_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void ycrcb2block(cv::Mat &yIn, cv::Mat &crIn, cv::Mat &cbIn, std::vector<cv::Mat> &blocksY, std::vector<cv::Mat> &blocksCr, std::vector<cv::Mat> &blocksCb);

#endif

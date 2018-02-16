#ifndef BLOC_HPP
#define BLOC_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

std::vector<cv::Mat_<uchar>> matrix2blocks(const cv::Mat_<uchar> &inMat);
cv::Mat_<uchar> blocks2matrix(const std::vector<cv::Mat_<uchar>> inBlocks, const cv::Size &inImageSize);

#endif

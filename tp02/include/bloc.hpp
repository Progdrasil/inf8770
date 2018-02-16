#ifndef BLOC_HPP
#define BLOC_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

std::vector<cv::Mat_<uchar>> matrix2block(cv::Mat_<uchar> &inMat);

#endif

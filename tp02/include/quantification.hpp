#ifndef QUANT_HPP
#define QUANT_HPP

#include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/imgproc/imgproc.hpp>

std::vector<cv::Mat_<signed char>> quantification(const std::vector<cv::Mat_<float>> &inDct);
std::vector<cv::Mat_<float>> inv_quantification(const std::vector<cv::Mat_<signed char>> &inQuantif);

#endif
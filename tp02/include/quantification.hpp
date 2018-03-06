#ifndef QUANT_HPP
#define QUANT_HPP

#include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/imgproc/imgproc.hpp>

extern const cv::Mat_<int> basicQuantif;

std::vector<cv::Mat_<char>> quantification(const std::vector<cv::Mat_<float>> &inDct, int level = -1);
std::vector<cv::Mat_<float>> inv_quantification(const std::vector<cv::Mat_<char>> &inQuantif, int level = -1);
cv::Mat_<int> createQuantification(int level);

#endif
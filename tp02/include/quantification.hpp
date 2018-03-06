#ifndef QUANT_HPP
#define QUANT_HPP

#include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/imgproc/imgproc.hpp>
#include "quantifType.hpp"

extern const cv::Mat_<int> basicQuantif;
extern const cv::Mat_<int> quantifConstant;
extern const cv::Mat_<int> quantifTriDiag;

std::vector<cv::Mat_<char>> quantification(const std::vector<cv::Mat_<float>> &inDct, quantifType type, int level = 1);
std::vector<cv::Mat_<float>> inv_quantification(const std::vector<cv::Mat_<char>> &inQuantif, quantifType type, int level = 1);
cv::Mat_<int> createQuantification(int level);

#endif

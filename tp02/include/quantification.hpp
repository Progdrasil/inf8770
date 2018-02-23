#ifndef QUANT_HPP
#define QUANT_HPP

#include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/imgproc/imgproc.hpp>

extern const cv::Mat_<uchar> basicQuantif;

enum quantificationLevel {
    NoQuantification,
    BasicQuantification,
    DoubleQuantification
};

std::vector<cv::Mat_<char>> quantification(const std::vector<cv::Mat_<float>> &inDct, int level);
std::vector<cv::Mat_<float>> inv_quantification(const std::vector<cv::Mat_<char>> &inQuantif, int level);

#endif
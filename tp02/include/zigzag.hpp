#ifndef ZIGZAG_HPP
#define ZIGZAG_HPP

#include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/imgproc/imgproc.hpp>

std::vector<int> blocks2vector(const std::vector<cv::Mat_<int>> &inBlocks);
std::vector<cv::Mat_<int>> vector2blocks(const std::vector<int> &inVector, cv::Size &sizeBlock, uint nbBlocks);

#endif
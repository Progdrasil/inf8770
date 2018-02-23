#ifndef ZIGZAG_HPP
#define ZIGZAG_HPP

#include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/imgproc/imgproc.hpp>

std::vector<char> blocks2vector(const std::vector<cv::Mat_<char>> &inBlocks);
std::vector<cv::Mat_<char>> vector2blocks(const std::vector<char> &inVector, cv::Size &sizeBlock, uint nbBlocks);

#endif
#ifndef QUANT_HPP
#define QUANT_HPP

#include <iostream>
#include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/imgproc/imgproc.hpp>

extern const cv::Mat_<int> basicQuantif;
extern const cv::Mat_<int> quantifConstant;
extern const cv::Mat_<int> quantifTriDiag;

enum quantifType {
	BASIC,
	LEVEL,
	CONSTANT,
	TRI_DIAGONAL
};

std::istream& operator>>(std::istream& in, quantifType& type)
{
    std::string token;
    in >> token;
    if (token == "basic")
        type = quantifType::BASIC;
    else if (token == "level")
        type = quantifType::LEVEL;
	else if (token == "constant")
		type = quantifType::CONSTANT;
	else if (token == "tri-diagonal")
		type = quantifType::TRI_DIAGONAL;
	else
        in.setstate(std::ios_base::failbit);
    return in;
}

std::vector<cv::Mat_<char>> quantification(const std::vector<cv::Mat_<float>> &inDct, quantifType type, int level = -1);
std::vector<cv::Mat_<float>> inv_quantification(const std::vector<cv::Mat_<char>> &inQuantif, quantifType type, int level = -1);
cv::Mat_<int> createQuantification(int level);

#endif

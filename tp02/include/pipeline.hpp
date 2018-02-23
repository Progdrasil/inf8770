#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <boost/filesystem.hpp>
#include <iostream>

#include "conversion.hpp"
#include "bloc.hpp"
#include "dct.hpp"
#include "quantification.hpp"
#include "zigzag.hpp"

namespace fs = boost::filesystem;

int code(fs::path path, bool subsampling, uint quantifLevel, std::vector<cv::Mat_<char>> * quantif, std::vector<cv::Size> * oSize, std::vector<uint> * lineSizes);
int decode(std::vector<cv::Mat_<char>> quantif, uint quantifLevel, std::vector<cv::Size> oSize, std::vector<uint> lineSizes);
void dispImg(std::string message, cv::Mat &image);
template <typename T> std::vector<T> linearise(std::vector<T> y, std::vector<T> cb, std::vector<T> cr, std::vector<uint> * lineSizes);
template <typename T> void delinearise(std::vector<T> line, std::vector<T> * y, std::vector<T> * cb, std::vector<T> * cr, std::vector<uint> lineSizes);

#endif

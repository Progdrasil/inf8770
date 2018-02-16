#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <boost/filesystem.hpp>
#include <iostream>

#include "conversion.hpp"
#include "bloc.hpp"
#include "dct.hpp"

namespace fs = boost::filesystem;

int code(fs::path path, fs::path save, bool subsampling);
int decode(fs::path path, fs::path save);
void dispImg(std::string message, cv::Mat &image);

#endif

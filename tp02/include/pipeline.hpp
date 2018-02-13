#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <boost/filesystem.hpp>
#include <iostream>

#include "conversion.hpp"
#include "bloc.hpp"

namespace fs = boost::filesystem;

int code(fs::path path, fs::path save);
int decode(fs::path path, fs::path save);
void dispImg(cv::Mat &image);

#endif

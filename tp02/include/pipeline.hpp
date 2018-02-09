#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <boost/filesystem.hpp>
#include <iostream>

#include "conversion.hpp"

namespace fs = boost::filesystem;

void code(fs::path path, fs::path save);
void decode(fs::path path, fs::path save);

#endif

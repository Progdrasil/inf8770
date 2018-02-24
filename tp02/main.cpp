/*******************************************************************************
	TP01: comparaison codage Huffman et LZW

	Auteurs:
		- Rene Leveille
		- Thibault Noilly

	Cree le: 12/01/2018
	Modifie le: 26/01/2018
*******************************************************************************/


#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include "pipeline.hpp"

using namespace std;
namespace po = boost::program_options;
namespace fs = boost::filesystem;

int main(int argc, char *argv[]) {
	fs::path path;
	uint quantifLevel;

	// Declare the supported options
	po::options_description desc{
		"USAGE: tp [options] <path>\n\n"
		"Options for homemade JPEG encoder"
	};
	desc.add_options()
	("help,h", "Shows this useful help message")
	("path,p", po::value<fs::path>(&path), "Path to the file to code/decode")
	("noSubsampling,y", "Deactivate YCbCr 4:2:0 subsampling")
	("quantification,q", po::value<uint>(&quantifLevel)->default_value(1), "Specify level of quantification to use")
	;

	po::positional_options_description p;
	p.add("path", -1);

	po::variables_map vm;

	try {
		po::store(po::command_line_parser(argc, argv).
					options(desc).positional(p).run(), vm);
	}
	catch (boost::exception &) {
		cout << "Invalid Option!!" << endl;
		cout << desc << endl;
		return -1;
	}
	po::notify(vm);

	if (vm.count("help")) {
		cout << desc << endl;
		return 0;
	}
	else if (vm.count("path")) {
		bool subsampling = ! vm.count("noSubsampling");
		std::vector<cv::Mat_<char>> quantif;
		std::vector<cv::Size> ycbcrSize;
		std::vector<uint> lineSizes;
		int codeRes = code(path, subsampling, quantifLevel, quantif, &ycbcrSize, &lineSizes);
		if (codeRes != 0) {
			return codeRes;
		}

		// int decodeRes = decode(quantif, quantifLevel, ycbcrSize, lineSizes);
		// if (decodeRes != 0) {
		// 	return decodeRes;
		// }
	}
	else {
		cout << "ERROR: Must specify the path to the input file!" << endl;
		cout << desc << endl;
		return -1;
	}

	return 0;
}

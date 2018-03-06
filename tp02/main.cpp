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
#include "quantifType.hpp"
#include "pipeline.hpp"

using namespace std;
namespace po = boost::program_options;
namespace fs = boost::filesystem;

float calculatePSNR(fs::path, fs::path);

int main(int argc, char *argv[])
{
	fs::path path;
	int quantifLevel;
	int nbQuantif;
	quantifType qtype;

	// Declare the supported options
	po::options_description desc{
		"USAGE: tp [options] <path>\n\n"
		"Options for homemade JPEG encoder"};
	desc.add_options()
		("help,h", "Shows this useful help message")
		("path,p", po::value<fs::path>(&path), "Path to the file to code/decode")
		("noSubsampling,y", "Deactivate YCbCr 4:2:0 subsampling")
		("quantification,q", po::value<int>(&quantifLevel)->default_value(1), "Specify level of quantification to use")
		("multiple-quantification,m", po::value<int>(&nbQuantif)->default_value(1), "Specify number of quantifications done")
		("quantification-type,t", po::value<quantifType>(&qtype)->default_value(quantifType::BASIC),"Specify the type of quantification matrix: basic, level, constant, tri-diagonal")
		;

	po::positional_options_description p;
	p.add("path", -1);

	po::variables_map vm;

	try
	{
		po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
	}
	catch (boost::exception &)
	{
		cout << "Invalid Option!!" << endl;
		cout << desc << endl;
		return -1;
	}
	po::notify(vm);

	if (vm.count("help"))
	{
		cout << desc << endl;
		return 0;
	}
	else if (vm.count("path"))
	{
		bool subsampling = !vm.count("noSubsampling");
		uint8_t *huffmanData = nullptr;
		std::vector<cv::Size> ycbcrSize;
		std::vector<uint> lineSizes;
		std::vector<int> compressionData;

		fs::path newPath = path;
		for (int q = 0; q < nbQuantif; q++)
		{
			int codeRes = code(newPath, subsampling, quantifLevel, qtype, &huffmanData, ycbcrSize, lineSizes, compressionData);
			if (codeRes != 0)
			{
				return codeRes;
			}

			fs::path savedPath("./results/results" + to_string(q) + "_q"
					+ to_string(quantifLevel) + "_" + path.filename().string());

			int decodeRes = decode(savedPath, &huffmanData, quantifLevel, qtype, ycbcrSize, lineSizes, compressionData);
			if (decodeRes != 0)
			{
				return decodeRes;
			}

			std::cout << "=================================" << std::endl;
			std::cout << "Comparing " + path.string() + " and " + savedPath.string() << std::endl;
			float psnr = calculatePSNR(path, savedPath);
			std::cout << "PSNR output : " + to_string(psnr) << std::endl;
			std::cout << "=================================" << std::endl;

			newPath = savedPath;
		}

		// Wait for a keystroke in the window
		cv::waitKey(0);
	}
	else
	{
		cout << "ERROR: Must specify the path to the input file!" << endl;
		cout << desc << endl;
		return -1;
	}

	return 0;
}

float calculatePSNR(fs::path inPath, fs::path outPath)
{
	cv::Mat inImage = cv::imread(inPath.string(), CV_LOAD_IMAGE_COLOR);
	cv::Mat outImage = cv::imread(outPath.string(), CV_LOAD_IMAGE_COLOR);

	if (!inImage.data || !outImage.data)
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}

	cv::Mat s1;
	absdiff(inImage, outImage, s1); // |I1 - I2|

	dispImg("Decompression error", s1);

	s1.convertTo(s1, CV_32F); // cannot make a square on 8 bits
	s1 = s1.mul(s1);		  // |I1 - I2|^2

	cv::Scalar s = sum(s1); // sum elements per channel

	double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels

	if (sse <= 1e-10) // for small values return zero
		return 0;
	else
	{
		double mse = sse / (double)(inImage.channels() * inImage.total());
		double psnr = 10.0 * log10((255 * 255) / mse);
		return psnr;
	}
}

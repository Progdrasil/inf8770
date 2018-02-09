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
#include <string>
#include <iostream>
#include "conversion.hpp"

using namespace std;
namespace po = boost::program_options;
namespace fs = boost::filesystem;

void code(fs::path path, fs::path save) {
	save /= path.filename();
	save.replace_extension(".jpeg");

	cout << "Will read file: " << path.string()
		 << " and save its compressed form as " << save.string() << endl;
}

void decode(fs::path path, fs::path save) {
	cout << "Will read compressed file: " << path.string()
		 << " and display it" << endl;
}

int main(int argc, char *argv[]) {
	fs::path path, save;

	// Declare the supported options
	po::options_description desc{"Options for homemade JPEG encoder \n"
									"tp02 [options] <path>"
								};
	desc.add_options()
		("help,h", "Shows the help message")
		("path,p", po::value<fs::path>(&path), "Path to the file to code/decode")
		("save,s", po::value<fs::path>(&save)->default_value("./images/"), "Path to save the compressed file, default is './images/'")
		("decode,d", "decode specified file and show comparison")
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

	if (vm.count("path")) {
		if (vm.count("decode")) {
			decode(path, save);
		}
		else {
			code(path, save);
		}
	}
	else {
		cout << "Must specify the path to the input file!" << endl;
		cout << desc << endl;
		return -1;
	}

	return 0;
}

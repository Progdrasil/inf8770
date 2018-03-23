/*******************************************************************************
	TP03: Decomposition de prises de vue

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

	// Declare the supported options
	po::options_description desc{
		"USAGE: tp [options] <path>\n\n"
		"Options for homemade scene identifyer"};
	desc.add_options()
		("help,h", "Shows this useful help message")
		("path,p", po::value<fs::path>(&path), "Path to the file to analyse")
		;

	po::positional_options_description p;
	p.add("path", -1);

	po::variables_map vm;

	try {
		po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
	} catch (boost::exception &) {
		cout << "Invalid Option!!" << endl;
		cout << desc << endl;
		return -1;
	}
	po::notify(vm);

	if (vm.count("help")) {
		cout << desc << endl;
		return 0;
	} else if (vm.count("path")) {
		cout << "The code monkey's are working hard to "
				"analyse the file located here: " << path.string() << endl;
		return open(path);
	} else {
		cout << "ERROR: Must specify the path to the input file!" << endl;
		cout << desc << endl;
		return -1;
	}

	return 0;
}

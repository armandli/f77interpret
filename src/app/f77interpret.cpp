#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    po::options_description desc("Usage: f77interpret [options] [file]");
    desc.add_options()
        ("help,h", "show help message")
        ("include-path,I", po::value<std::vector<std::string>>()->composing(), "include search path (may be specified multiple times)")
        ("file", po::value<std::string>(), "FORTRAN 77 source file to interpret");

    po::positional_options_description pos;
    pos.add("file", 1);

    po::variables_map vm;
    po::store(
        po::command_line_parser(argc, argv)
            .options(desc)
            .positional(pos)
            .run(),
        vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 0;
    }

    std::vector<std::string> include_paths;
    if (vm.count("include-path")) {
        include_paths = vm["include-path"].as<std::vector<std::string>>();
    }

    if (vm.count("file")) {
        std::string path = vm["file"].as<std::string>();
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "error: cannot open file '" << path << "'\n";
            return 1;
        }
        // TODO: pass file stream and include_paths to interpreter
    } else {
        // Interactive mode
        std::string line;
        while (std::getline(std::cin, line)) {
            // TODO: pass line and include_paths to interpreter
        }
    }

    return 0;
}

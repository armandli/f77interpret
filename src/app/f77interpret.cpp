#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include <alias.h>
#include <config.h>

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
  po::options_description desc("Usage: f77interpret [options] [file]");
  desc.add_options()
    ("help,h", "show help message")
    ("include-path,I",
      po::value<s::vector<s::string>>()->composing(),
      "include search path (may be specified multiple times)")
    ("file", po::value<s::string>(), "FORTRAN 77 source file to interpret");

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
    s::cout << desc << "\n";
    return 0;
  }

  f77i::Config config;
  if (vm.count("include-path")) {
    config.include_paths = vm["include-path"].as<s::vector<s::string>>();
  }
  if (vm.count("file")) {
    config.source_file = vm["file"].as<s::string>();
  }

  if (not config.source_file.empty()) {
    s::ifstream file(config.source_file);
    if (not file.is_open()) {
      s::cerr << "error: cannot open file '" << config.source_file << "'\n";
      return 1;
    }
    // TODO: pass file stream and config to interpreter
  } else {
    // Interactive mode
    s::string line;
    while (s::getline(s::cin, line)) {
      // TODO: pass line and config to interpreter
    }
  }

  return 0;
}

#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include <alias.h>
#include <config.h>
#include <interactive_source_file.h>
#include <token_debug_printer.h>

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
  po::options_description desc("Usage: f77interpret [options] [file]");
  desc.add_options()
    ("help,h", "show help message")
    ("include-path,I",
      po::value<s::vector<s::string>>()->composing(),
      "include search path (may be specified multiple times)")
    ("file", po::value<s::string>(), "FORTRAN 77 source file to interpret")
    ("codepoint",
      po::value<s::string>()->default_value("ASCII"),
      "character encoding: ASCII (default), UTF8, CODEPAGE")
    ("debug-tokenizer", "tokenize input and print tokens (REPL mode only)");

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
  {
    s::string cp = vm["codepoint"].as<s::string>();
    if (cp == "UTF8") {
      config.codepoint = f77i::Codepoint::UTF8;
    } else if (cp == "CODEPAGE") {
      config.codepoint = f77i::Codepoint::CODEPAGE;
    } else if (cp == "ASCII") {
      config.codepoint = f77i::Codepoint::ASCII;
    } else {
      s::cerr << "error: unknown codepoint '" << cp
        << "' (valid: ASCII, UTF8, CODEPAGE)\n";
      return 1;
    }
  }
  if (vm.count("debug-tokenizer")) {
    config.debug_tokenizer = true;
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
    f77i::InteractiveSourceFile isf(config);
    f77i::TokenDebugPrinter printer;
    s::string line;
    while (s::getline(s::cin, line)) {
      s::size_t prev = isf.tokens().size();
      isf.append(line);
      if (config.debug_tokenizer) {
        const auto& tokens = isf.tokens();
        for (s::size_t i = prev; i < tokens.size(); ++i) {
          printer.print(s::cout, tokens[i]);
        }
      }
      // TODO: pass isf and config to interpreter
    }
  }

  return 0;
}

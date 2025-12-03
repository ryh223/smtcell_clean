#include "SMTCellLib.hpp"
#include <iostream>
#include <string>
#include <vector>

void printUsage(const char* progName) {
    std::cerr << "\nUsage: " << progName << " --CDL <path> --config <path> --rule <path> [--out <path>] [--cell <name>]\n"
              << "\nRequired arguments:\n"
              << "  --CDL <path>       : Path to the CDL file.\n"
              << "  --config <path>    : Path to the configuration file.\n"
              << "  --rule <path>      : Path to the rule file.\n"
              << "\nOptional arguments:\n"
              << "  --out <path>       : Output directory path. (Default: ./result)\n"
              << "  --cell <name>      : Specific cell name to process. (Default: all cells in CDL)\n\n";
}

int main(int argc, char *argv[]) {
    auto const workdir = std::filesystem::current_path();
    std::string out_path = "./result"; // Default value
    std::string CDL_path = "";
    std::string config_path = "";
    std::string rule_path = "";
    std::string cell_name = ""; // Default value

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--CDL" && i + 1 < argc) {
            CDL_path = argv[++i];
        } else if (arg == "--config" && i + 1 < argc) {
            config_path = argv[++i];
        } else if (arg == "--rule" && i + 1 < argc) {
            rule_path = argv[++i];
        } else if (arg == "--out" && i + 1 < argc) {
            out_path = argv[++i];
        } else if (arg == "--cell" && i + 1 < argc) {
            cell_name = argv[++i];
        } else {
            std::cerr << "Error: Unknown or incomplete argument '" << arg << "'\n";
            printUsage(argv[0]);
            return 1;
        }
    }

    // Validate required arguments
    if (CDL_path.empty() || config_path.empty() || rule_path.empty()) {
        std::cerr << "Error: Missing one or more required arguments (--CDL, --config, --rule).\n";
        printUsage(argv[0]);
        return 1;
    }

    auto config = std::make_unique<const dbConfig>(config_path);
    auto rule = std::make_unique<const ruleParameter>(rule_path, config->getMM_Parameter());

    std::vector<std::unique_ptr<schematic>> cellSchematics;

    auto parser = std::make_unique<Parser>();
    parser->parseCDL(CDL_path, config->getNumFin(), cellSchematics);
}
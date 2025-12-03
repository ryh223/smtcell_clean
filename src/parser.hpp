#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "schematic.hpp"

class Parser {
  public:
    Parser() = default;

    void parseCDL(const std::string &filename, int FinNum, std::vector<std::unique_ptr<schematic>> &schematics_out);

  private:
    void checkCDL(const std::string &filename, int FinNum);
};

#include "parser.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <sstream>

namespace {

std::string trim(const std::string &text) {
  auto begin = text.find_first_not_of(" \t\r\n");
  if (begin == std::string::npos) {
    return "";
  }
  auto end = text.find_last_not_of(" \t\r\n");
  return text.substr(begin, end - begin + 1);
}

std::vector<std::string> tokenize(const std::string &line) {
  std::istringstream iss(line);
  std::vector<std::string> tokens;
  std::string token;
  while (iss >> token) {
    tokens.push_back(token);
  }
  return tokens;
}

bool startsWithIgnoreCase(const std::string &text, const std::string &prefix) {
  if (text.size() < prefix.size()) {
    return false;
  }
  for (size_t i = 0; i < prefix.size(); ++i) {
    if (std::toupper(static_cast<unsigned char>(text[i])) != std::toupper(static_cast<unsigned char>(prefix[i]))) {
      return false;
    }
  }
  return true;
}

std::string toLower(std::string text) {
  std::transform(text.begin(), text.end(), text.begin(),
                 [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  return text;
}

int extractNfin(const std::vector<std::string> &tokens) {
  for (auto tok : tokens) {
    tok = toLower(tok);
    if (tok.find("nfin") == std::string::npos) {
      continue;
    }

    auto eqPos = tok.find('=');
    if (eqPos == std::string::npos || eqPos + 1 >= tok.size()) {
      continue;
    }

    auto start = eqPos + 1;
    auto end = start;
    while (end < tok.size() && std::isdigit(static_cast<unsigned char>(tok[end]))) {
      ++end;
    }
    if (end > start) {
      return std::stoi(tok.substr(start, end - start));
    }
  }

  return -1;
}

bool isPowerPinName(const std::string &name) { return name == "VDD" || name == "VSS"; }

std::string makeNetName(const std::string &rawName, const std::unordered_set<std::string> &ioPins,
                        const std::unordered_set<std::string> &powerPins) {
  if (ioPins.count(rawName) > 0 || powerPins.count(rawName) > 0) {
    return "net_" + rawName;
  }
  return rawName;
}

} // namespace

void Parser::parseCDL(const std::string &filename, int FinNum, std::vector<std::unique_ptr<schematic>> &schematics_out) {
  checkCDL(filename, FinNum);

  std::ifstream infile(filename);
  if (!infile.is_open()) {
    std::cerr << "Error: cannot open CDL file " << filename << "\n";
    std::exit(EXIT_FAILURE);
  }

  std::unique_ptr<schematic> current;
  std::unordered_set<std::string> ioPins;
  std::unordered_set<std::string> powerPins;
  std::string rawLine;
  int lineNumber = 0;

  while (std::getline(infile, rawLine)) {
    ++lineNumber;
    auto line = trim(rawLine);
    if (line.empty() || line[0] == '*') {
      continue;
    }

    if (startsWithIgnoreCase(line, ".SUBCKT")) {
      if (current) {
        schematics_out.push_back(std::move(current));
      }

      ioPins.clear();
      powerPins.clear();

      auto tokens = tokenize(line);
      if (tokens.size() < 2) {
        std::cerr << "Error: malformed .SUBCKT at line " << lineNumber << "\n";
        std::exit(EXIT_FAILURE);
      }

      auto cellName = tokens[1];
      current = std::make_unique<schematic>(cellName);

      for (size_t i = 2; i < tokens.size(); ++i) {
        auto pinName = tokens[i];
        bool isPower = isPowerPinName(pinName);
        bool isIO = !isPower;
        if (isPower) {
          powerPins.insert(pinName);
        } else {
          ioPins.insert(pinName);
        }

        auto netName = "net_" + pinName;
        auto net = current->getOrCreateNet(netName, isPower, isIO);
        current->addPin(pinName, net, nullptr, "", isIO, isPower, true);
      }
      continue;
    }

    if (startsWithIgnoreCase(line, ".ENDS")) {
      if (current) {
        schematics_out.push_back(std::move(current));
      }
      current.reset();
      ioPins.clear();
      powerPins.clear();
      continue;
    }

    if (!current) {
      continue;
    }

    auto tokens = tokenize(line);
    if (tokens.empty()) {
      continue;
    }

    if (tokens[0].empty() || (tokens[0][0] != 'M' && tokens[0][0] != 'm')) {
      continue;
    }

    if (tokens.size() < 6) {
      std::cerr << "Error: malformed instance line at " << lineNumber << "\n";
      std::exit(EXIT_FAILURE);
    }

    const auto &instName = tokens[0];
    const auto &drainNetName = tokens[1];
    const auto &gateNetName = tokens[2];
    const auto &sourceNetName = tokens[3];
    const auto &typeToken = tokens[5];

    auto instanceType = toLower(typeToken).find("pmos") != std::string::npos ? "pmos" : "nmos";
    int nfin = extractNfin(tokens);
    if (nfin <= 0) {
      std::cerr << "Error: missing nfin for " << instName << " at line " << lineNumber << "\n";
      std::exit(EXIT_FAILURE);
    }

    int finger = nfin / FinNum;
    auto inst = current->addInstance(instName, instanceType, FinNum, nfin);

    int pinCount = finger * 2 + 1;
    for (int i = 0; i < pinCount; ++i) {
      bool isGate = (i % 2 == 1);
      bool isSource = ((i / 2) % 2 == 0);
      auto pinType = isGate ? Pin::GATE : (isSource ? Pin::SOURCE : Pin::DRAIN);
      auto rawNetName = isGate ? gateNetName : (isSource ? sourceNetName : drainNetName);

      bool isPower = false;
      bool isIO = false;
      auto netName = makeNetName(rawNetName, ioPins, powerPins);
      auto net = current->getOrCreateNet(netName, isPower, isIO);

      auto pinName = "pin" + instName + "_" + std::to_string(i);
      current->addPin(pinName, net, inst, pinType, isIO, isPower, false);
    }
  }

  if (current) {
    schematics_out.push_back(std::move(current));
  }
}

void Parser::checkCDL(const std::string &filename, int FinNum) {
  if (FinNum <= 0) {
    std::cerr << "Error: FinNum must be positive\n";
    std::exit(EXIT_FAILURE);
  }

  std::ifstream infile(filename);
  if (!infile.is_open()) {
    std::cerr << "Error: cannot open CDL file " << filename << "\n";
    std::exit(EXIT_FAILURE);
  }

  std::string rawLine;
  int lineNumber = 0;
  while (std::getline(infile, rawLine)) {
    ++lineNumber;
    auto line = trim(rawLine);
    if (line.empty() || line[0] == '*') {
      continue;
    }

    if (line[0] != 'M' && line[0] != 'm') {
      continue;
    }

    auto tokens = tokenize(line);
    int nfin = extractNfin(tokens);
    if (nfin <= 0) {
      std::cerr << "Error: missing nfin for " << (!tokens.empty() ? tokens[0] : "instance") << " at line " << lineNumber << "\n";
      std::exit(EXIT_FAILURE);
    }

    if (nfin % FinNum != 0) {
      std::cerr << "Error: nfin of " << (!tokens.empty() ? tokens[0] : "instance") << " (" << nfin
                << ") is not a multiple of FinNum (" << FinNum << ") at line " << lineNumber << "\n";
      std::exit(EXIT_FAILURE);
    }
  }
}

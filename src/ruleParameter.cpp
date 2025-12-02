#include "ruleParameter.hpp"
#include <cstdlib> // For exit()
#include <iostream>
#include <fstream>

ruleParameter::ruleParameter(const std::string &config_path, int metal_num) {
  std::ifstream config_file(config_path);
  if (!config_file.is_open()) {
    std::cerr << "Error: Could not open rule file: " << config_path << std::endl;
    exit(1);
  }
  nlohmann::json config = nlohmann::json::parse(config_file);
  config_file.close();

  checkParameter(metal_num, config);

  // Parse required parameters
  unit_ = std::stoi(config["unit"]["value"].get<std::string>());
  CPP_ = std::stoi(config["CPP"]["value"].get<std::string>());
  FinWidth_ = std::stoi(config["FinWidth"]["value"].get<std::string>());
  FinPitch_ = std::stoi(config["FinPitch"]["value"].get<std::string>());
  GateWidth_ = std::stoi(config["GateWidth"]["value"].get<std::string>());

  // Parse Metal parameters (M1 to M(metal_num-1))
  for (int i = 1; i < metal_num; ++i) {
    M_P_.push_back(std::stoi(config["M" + std::to_string(i) + "P"]["value"].get<std::string>()));
    M_offset_.push_back(std::stoi(config["M" + std::to_string(i) + "_offset"]["value"].get<std::string>()));
    M_width_.push_back(std::stoi(config["M" + std::to_string(i) + "_width"]["value"].get<std::string>()));
    std::string lower_v_en_key = "V" + std::to_string(i -1) + "_M" + std::to_string(i) + "_EN";
    if (config.contains(lower_v_en_key)) {
      V_EN_[lower_v_en_key] = config[lower_v_en_key]["value"].get<std::vector<std::vector<int>>>();
    }
    std:: string upper_v_en_key = "V" + std::to_string(i) + "_M" + std::to_string(i) + "_EN";
    if (config.contains(upper_v_en_key)) {
      V_EN_[upper_v_en_key] = config[upper_v_en_key]["value"].get<std::vector<std::vector<int>>>();
    }
  }
}

void ruleParameter::checkParameter(int metal_num, const nlohmann::json &config) {
  const std::vector<std::string> required_params = {"unit", "CPP", "FinWidth", "FinPitch", "GateWidth"};
  for (const auto &param : required_params) {
    if (!config.contains(param)) {
      std::cerr << "Error: Required parameter '" << param << "' not found in rule file." << std::endl;
      exit(1);
    }
  }

  for (int i = 1; i < metal_num; ++i) {
    if (!config.contains("M" + std::to_string(i) + "P") || !config.contains("M" + std::to_string(i) + "_offset") || !config.contains("M" + std::to_string(i) + "_width")) {
      std::cerr << "Error: Parameters for M" << i << " are missing." << std::endl;
      exit(1);
    }
    std::string lower_v_en_key = "V" + std::to_string(i - 1) + "_M" + std::to_string(i) + "_EN";
    if (!config.contains(lower_v_en_key)) {
      std::cerr << "Error: Parameter for " << lower_v_en_key << " is missing." << std::endl;
      exit(1);
    }
    std::string upper_v_en_key = "V" + std::to_string(i) + "_M" + std::to_string(i) + "_EN";
    if (i < metal_num -1 && !config.contains(upper_v_en_key)) {
      std::cerr << "Error: Parameter for " << upper_v_en_key << " is missing." << std::endl;
      exit(1);
    }
  }
}

// Getters
int ruleParameter::getUnit() const { return unit_; }
int ruleParameter::getCPP() const { return CPP_; }
int ruleParameter::getFinWidth() const { return FinWidth_; }
int ruleParameter::getFinPitch() const { return FinPitch_; }
int ruleParameter::getGateWidth() const { return GateWidth_; }

const std::vector<int> &ruleParameter::getM_P() const { return M_P_; }
const std::vector<int> &ruleParameter::getM_offset() const { return M_offset_; }
const std::vector<int> &ruleParameter::getM_width() const { return M_width_; }
const std::map<std::string, std::vector<std::vector<int>>> &ruleParameter::getV_EN() const { return V_EN_; }

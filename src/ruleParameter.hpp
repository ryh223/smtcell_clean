#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

class ruleParameter {
public:
  ruleParameter(const std::string &config_path, int metal_num);
  ~ruleParameter() = default;

  // Getters
  int getUnit() const;
  int getCPP() const;
  int getFinWidth() const;
  int getFinPitch() const;
  int getGateWidth() const;

  const std::vector<int> &getM_P() const;
  const std::vector<int> &getM_offset() const;
  const std::vector<int> &getM_width() const;
  const std::map<std::string, std::vector<std::vector<int>>> &getV_EN() const;

private:
  void checkParameter(int metal_num, const nlohmann::json &config);

  // Required parameters
  int unit_;
  int CPP_;
  int FinWidth_;
  int FinPitch_;
  int GateWidth_;

  // Metal and Via parameters
  std::vector<int> M_P_;
  std::vector<int> M_offset_;
  std::vector<int> M_width_;
  std::map<std::string, std::vector<std::vector<int>>> V_EN_;
};


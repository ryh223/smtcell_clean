#include "dbConfig.h"

dbConfig::dbConfig(const std::string &config_path, std::string infile) {
  std::ifstream config_file(config_path);
  nlohmann::json config = nlohmann::json::parse(config_file);
  config_file.close();
  
  // parse into variables
  NumFin_ = config["NumFin"]["value"];
  
  NmuTrack_ = config["NmuTrack"]["value"];

  Z3Seed_ = config["z3_seed"]["value"];

  BoundaryCondition_ = config["BoundaryCondition"]["value"];

  SON_ = config["SON"]["value"];

  DoublePowerRail_ = config["DoublePowerRail"]["value"];

  MM_Parameter_ = config["MM_Parameter"]["value"];

  EXT_Parameter_ = config["EXT_Parameter"]["value"];

  BCP_Parameter_ = config["BCP_Parameter"]["value"];

  MPL_Parameter_ = config["MPL_Parameter"]["value"];

  XOL_Mode_ = config["XOL_Mode"]["value"];

  NDE_Parameter_ = config["NDE_Parameter"]["value"];

  Partition_Parameter_ = config["Partition_Parameter"]["value"];

  ML_Parameter_ = config["ML_Parameter"]["value"];

  Local_Parameter_ = config["Local_Parameter"]["value"];

  tolerance_Parameter_ = config["tolerance_Parameter"]["value"];

  BS_Parameter_ = config["BS_Parameter"]["value"];

  objpart_Parameter_ = config["objpart_Parameter"]["value"];

  XOL_Parameter_ = config["XOL_Parameter"]["value"];


  // Special Condition for DFF cells
  if (infile.find("DFF") != std::string::npos) {
    Partition_Parameter_ = 2;
    BCP_Parameter_ = 1;
    NDE_Parameter_ = 0;
    BS_Parameter_ = 0;
    std::cout << "\na     *** DFF Cell Detected. Forcing the following constraint ... ***";
    std::cout << "\na     *** [INFO] Disable NDE, BS; Partitioning = 2; BCP = 1; ***";
  } else if (Partition_Parameter_ == 2) 
  {
    std::cout << "\n    *** Partitioning = 2; BCP = 1; ***";
    BCP_Parameter_ = 1;
    NDE_Parameter_ = 0;
    BS_Parameter_ = 0;
  }
  

  dbConfig::checkParameter();
}

void dbConfig::checkParameter() {
  if (MPL_Parameter_ < 2 || MPL_Parameter_ > 4) {
    std::cout << "[ERROR] MPL_Parameter is not valid!\n";
    exit(-1);
  }

  std::cout << "\n";

  if (XOL_Mode_ == 0) {
    XOL_Parameter_ = 2;
  } else {
    XOL_Parameter_ = 4;
  }
}

void dbConfig::checkPartition_Parameter(int sizeOfPartition) {
  if (Partition_Parameter_ != 0) {
    if (sizeOfPartition == 0) {
      std::cout << "a     [WARNING] Partition_Parameter is not valid! Partition size is 0!\n";
      Partition_Parameter_ = 0;
      NDE_Parameter_ = 1;
      BS_Parameter_ = 1;
    }
  }
}
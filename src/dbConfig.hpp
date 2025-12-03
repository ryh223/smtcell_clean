#pragma once
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

class dbConfig {
  /*
    Configuration for Design Rule, Flow Constraints, Constraint Simplification
    and Graph Settings.
  */
public:
  dbConfig(const std::string &config_path, std::string infile = "");

  void checkParameter();

  // disable partition when info is not provided to avoid error
  void checkPartition_Parameter(int sizeOfPartition);

  int getNumFin() const { return NumFin_; }

  int getNumTrack() const { return NumTrack_; }

  int getZ3Seed() const { return Z3Seed_; }

  int getBoundaryCondition() const { return BoundaryCondition_; }

  int getSON() const { return SON_; }

  int getDoublePowerRail() const { return DoublePowerRail_; }

  int getMM_Parameter() const { return MM_Parameter_; }

  int getEXT_Parameter() const { return EXT_Parameter_; }

  int getBCP_Parameter() const { return BCP_Parameter_; }

  int getMPL_Parameter() const { return MPL_Parameter_; }

  int getXOL_Mode() const { return XOL_Mode_; }

  int getNDE_Parameter() const { return NDE_Parameter_; }

  int getPartition_Parameter() const { return Partition_Parameter_; }

  int getML_Parameter() const { return ML_Parameter_; }

  int getLocal_Parameter() const { return Local_Parameter_; }

  int getTolerance_Parameter() const { return tolerance_Parameter_; }

  int getBS_Parameter() const { return BS_Parameter_; }

  int getObjpart_Parameter() const { return objpart_Parameter_; }

  int getXOL_Parameter() const { return XOL_Parameter_; }

private:

  int NumFin_;

  int NumTrack_;

  // User defined Design Parameteres
  int Z3Seed_;
  // 0: Fixed, 1: Extensible
  int BoundaryCondition_;
  // 0: Disable, 1: Enable
  int SON_;
  // 0: Disable, 1: Enable
  int DoublePowerRail_;
  // 3: Maximum Number of MetalLayer(num_metal + 1)
  int MM_Parameter_;
  // 0 only
  int EXT_Parameter_;
  // 0: Disable 1: Enable BCP(Default) //if use new_h_assign 
  int BCP_Parameter_; 
  // ARGV[6], 3: (Default) Minimum Pin Opening
  int MPL_Parameter_;
  // ARGV[7], 0: SDB, 1:DDB, 2:(Default)SDB/DDB mixed
  int XOL_Mode_;
  // ARGV[8], 0: Disable(Default) 1: Enable NDE //multi height S/D share
  int NDE_Parameter_; 
  // ARGV[9], 0: Disable(Default) 1. Cell Partitioning
  int Partition_Parameter_;
  // ARGV[10], 5: (Default) Min Metal Length for avoiding adjacent signal // crosstalk mitigation by special net constraint
  // routing (only works with special net setting)
  int ML_Parameter_;
  // ARGV[11], 0: Disable(Default) 1: Localization for
  // Internal node within same diffusion region
  int Local_Parameter_;
  // ARGV[12], Localization Offset Margin, Integer
  int tolerance_Parameter_;
  // ARGV[13], 0: Disable(Default) 1: Enable BS(Breaking Symmetry)
  int BS_Parameter_;
  // ARGV[14], 0: Disable(Default) 1. Objective Partitioning //?
  int objpart_Parameter_;
  // Depends on XOL_Mode
  int XOL_Parameter_;

};
#define FMT_HEADER_ONLY
#pragma once
#define FMT_HEADER_ONLY
#include <boost/multiprecision/integer.hpp> // for returning bit length
#include <fmt/core.h>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "graph.hpp"
#include "obj.hpp"
#include "dbConfig.hpp"
#include "ruleParameter.hpp"

class SMTCell {
    public:
        SMTCell(const dbConfig& config, const ruleParameter& ruleParam);
        ~SMTCell() = default;

    private:
        void initTrackInfo()


        // config and rule param
        const dbConfig& config_;
        const ruleParameter& ruleParam_;

        //Track info
        int TrackNum_;
        std::vector<std::map<int, int>> level_SMTGridPitch_;
        std::map<int, int> SMTGridOffset_;
        std::map<std::string, std::vector<int>> FETPin_to_MetalTracks_;
   
};
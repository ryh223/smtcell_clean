#include "SMTCell.hpp"

SMTCell::SMTCell(const dbConfig& dbConfig, const ruleParameter& ruleParameter)
    : dbConfig_(dbConfig), ruleParameter_(ruleParameter) {
    initTrackInfo();
}

SMTCell::initTrackInfo() {
    TrackNum_ = dbconfig_.getTrackNum();

    std::map<int, int> init_SMTCellGridPitch;
    layer_num_ = dbConfig_.getMM_Parameter();
    for (int layer = 1; layer <= layer_num_; ++layer) {
        if (layer == 1) {
            init_SMTCellGridPitch[layer] = ruleParameter_.getCPP();
        }
        else {
            init_SMTCellGridPitch[layer] = ruleParameter_.getM_P().at(layer - 2);
        }
    }

    if (TrackNum_ % 2 == 0) {
        std::vector<int> p_tracks;
        for (int i = 0; i < TrackNum_ / 2; ++i) {
            p_tracks.push_back(i);
        }
        FETPin_to_MetalTracks_["P"] = p_tracks;
        std::vector<int> n_tracks;
        for (int i = TrackNum_ / 2; i < TrackNum_; ++i) {
            n_tracks.push_back(i);
        }
        FETPin_to_MetalTracks_["N"] = n_tracks;
    }
    else {
        std::vector<int> p_tracks;
        for (int i = 0; i < TrackNum_ / 2; ++i) {
            p_tracks.push_back(i);
        }
        FETPin_to_MetalTracks_["P"] = p_tracks;
        std::vector<int> n_tracks;
        for (int i = TrackNum_ / 2 + 1; i < TrackNum_; ++i) {
            n_tracks.push_back(i);
        }
        FETPin_to_MetalTracks_["N"] = n_tracks;
    }

}
#include "SMTCell.hpp"

SMTCell::SMTCell(const dbConfig* dbConfig, const ruleParameter* ruleParameter)
    : config_(dbConfig), ruleParam_(ruleParameter) {
    initTrackInfo();
}

void SMTCell::initTrackInfo() {
    TrackNum_ = config_->getNumTrack();

    std::map<int, int> init_SMTCellGridPitch;
    int layer_num_ = config_->getMM_Parameter();
    for (int layer = 1; layer <= layer_num_; ++layer) {
        if (layer == 1) {
            init_SMTCellGridPitch[layer] = ruleParam_->getCPP();
        }
        else {
            init_SMTCellGridPitch[layer] = ruleParam_->getM_P().at(layer - 2);
        }
    }

    if (TrackNum_ % 2 == 0) {
        std::vector<int> n_tracks;
        for (int i = 0; i < TrackNum_ / 2; ++i) {
            n_tracks.push_back(i);
        }
        FETPin_to_MetalTracks_["N"] = n_tracks;
        std::vector<int> p_tracks;
        for (int i = TrackNum_ / 2; i < TrackNum_; ++i) {
            p_tracks.push_back(i);
        }
        FETPin_to_MetalTracks_["P"] = p_tracks;
    }
    else {
        std::vector<int> n_tracks;
        for (int i = 0; i < TrackNum_ / 2; ++i) {
            n_tracks.push_back(i);
        }
        FETPin_to_MetalTracks_["N"] = n_tracks;
        std::vector<int> p_tracks;
        for (int i = TrackNum_ / 2 + 1; i < TrackNum_; ++i) {
            p_tracks.push_back(i);
        }
        FETPin_to_MetalTracks_["N"] = p_tracks;
    }

}
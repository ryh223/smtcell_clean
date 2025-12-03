#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class Pin;
class Net;
class Instance;

class schematic {
  public:
    schematic() = default;
    explicit schematic(std::string cellName);
    ~schematic();

    Instance *addInstance(const std::string &instanceName, const std::string &instanceType, int finNum, int nfin);
    Net *getOrCreateNet(const std::string &netName, bool isPowerNet = false, bool isIONet = false);
    Pin *addPin(const std::string &pinName, Net *net, Instance *instance, const std::string &pinType, bool isIOPin, bool isPowerPin,
                bool forceSink = false);

    const std::string &getCellName() const { return cellName_; }
    const std::vector<std::unique_ptr<Instance>> &getInstances() const { return instances_; }
    const std::vector<std::unique_ptr<Net>> &getNets() const { return nets_; }
    const std::vector<std::unique_ptr<Pin>> &getPins() const { return pins_; }
    const std::vector<Pin *> &getIOPins() const { return IOPins_; }
    const std::vector<Pin *> &getPowerPins() const { return powerPins_; }
    const std::vector<Net *> &getPowerNets() const { return powerNets_; }
    const std::vector<Net *> &getIONets() const { return IONets_; }

  private:
    void addPowerNet(Net *net);
    void addIONet(Net *net);

    std::string cellName_;
    std::vector<std::unique_ptr<Instance>> instances_;
    std::vector<std::unique_ptr<Net>> nets_;
    std::vector<std::unique_ptr<Pin>> pins_;
    std::vector<Pin *> IOPins_;
    std::vector<Pin *> powerPins_;
    std::vector<Net *> powerNets_;
    std::vector<Net *> IONets_;
    std::unordered_map<std::string, Net *> netLookup_;
};

class Pin {
  public:
    static const std::string SOURCE_NET;
    static const std::string SINK_NET;

    static const std::string SOURCE;
    static const std::string DRAIN;
    static const std::string GATE;

    Pin(const std::string &pinName, Net *net, Instance *instance, const std::string &pinType, const std::string &pinIO, bool ifIOPin,
        bool ifPowerPin);
    ~Pin() = default;

    const std::string &getName() const { return pinName_; }
    Net *getNet() const { return net_; }
    Instance *getInstance() const { return instance_; }
    const std::string &getPinType() const { return pinType_; }
    const std::string &getPinIO() const { return pinIO_; }
    bool isIOPin() const { return ifIOPin_; }
    bool isPowerPin() const { return ifPowerPin_; }

  private:
    std::string pinName_;
    Net *net_;
    Instance *instance_;
    std::string pinType_;
    std::string pinIO_;
    bool ifIOPin_ = false;
    bool ifPowerPin_ = false;
};

class Instance {
  public:
    static const std::string INSTANCE_TYPE;

    Instance(const std::string &instanceName, const std::string &instanceType, int finNum, int nfin);
    ~Instance() = default;

    const std::string &getName() const { return instanceName_; }
    const std::string &getType() const { return instanceType_; }
    int getFinNum() const { return finNum_; }
    int getFinger() const { return finger; }
    void addPin(Pin *pin);

  private:
    std::string instanceName_;
    std::vector<Pin *> pins_;
    std::string instanceType_;
    int finNum_ = 0;
    int finger = 1;
    int partitionID_ = -1;
};

class Net {
  public:
    explicit Net(const std::string &netName);
    ~Net() = default;

    const std::string &getName() const { return netName_; }
    void addPin(Pin *pin, bool isSource);
    void markPowerNet();
    void markIONet();
    bool isPowerNet() const { return ifPowerNet_; }
    bool isIONet() const { return ifIONet_; }
    bool hasSource() const { return sourcePin_ != nullptr; }
    Pin *getSourcePin() const { return sourcePin_; }
    const std::vector<Pin *> &getPins() const { return pins_; }
    const std::vector<Pin *> &getSinkPins() const { return sinkPins_; }

  private:
    std::string netName_;
    std::vector<Pin *> pins_;
    std::vector<Pin *> sinkPins_;
    Pin *sourcePin_ = nullptr;
    bool ifPowerNet_ = false;
    bool ifIONet_ = false;
};

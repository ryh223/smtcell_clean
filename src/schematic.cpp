#include "schematic.hpp"

const std::string Pin::SOURCE_NET = "SOURCE_NET";
const std::string Pin::SINK_NET = "SINK_NET";

const std::string Pin::SOURCE = "SOURCE";
const std::string Pin::DRAIN = "DRAIN";
const std::string Pin::GATE = "GATE";

const std::string Instance::INSTANCE_TYPE = "FET";

schematic::schematic(std::string cellName) : cellName_(std::move(cellName)) {}

schematic::~schematic() = default;

Instance *schematic::addInstance(const std::string &instanceName, const std::string &instanceType, int finNum, int nfin) {
  auto inst = std::make_unique<Instance>(instanceName, instanceType, finNum, nfin);
  auto *ptr = inst.get();
  instances_.push_back(std::move(inst));
  return ptr;
}

Net *schematic::getOrCreateNet(const std::string &netName, bool isPowerNet, bool isIONet) {
  auto it = netLookup_.find(netName);
  if (it != netLookup_.end()) {
    if (isPowerNet) {
      it->second->markPowerNet();
      addPowerNet(it->second);
    }
    if (isIONet) {
      it->second->markIONet();
      addIONet(it->second);
    }
    return it->second;
  }

  auto net = std::make_unique<Net>(netName);
  auto *netPtr = net.get();
  nets_.push_back(std::move(net));
  netLookup_[netName] = netPtr;

  if (isPowerNet) {
    netPtr->markPowerNet();
    addPowerNet(netPtr);
  }
  if (isIONet) {
    netPtr->markIONet();
    addIONet(netPtr);
  }

  return netPtr;
}

Pin *schematic::addPin(const std::string &pinName, Net *net, Instance *instance, const std::string &pinType, bool isIOPin,
                      bool isPowerPin, bool forceSink) {
  if (net == nullptr) {
    return nullptr;
  }

  std::string pinIO = Pin::SOURCE_NET;
  if (forceSink || net->hasSource()) {
    pinIO = Pin::SINK_NET;
  }

  auto pin = std::make_unique<Pin>(pinName, net, instance, pinType, pinIO, isIOPin, isPowerPin);
  auto *pinPtr = pin.get();
  pins_.push_back(std::move(pin));

  net->addPin(pinPtr, pinIO == Pin::SOURCE_NET);
  if (instance) {
    instance->addPin(pinPtr);
  }
  if (isIOPin) {
    IOPins_.push_back(pinPtr);
  }
  if (isPowerPin) {
    powerPins_.push_back(pinPtr);
  }

  return pinPtr;
}

void schematic::addPowerNet(Net *net) {
  if (std::find(powerNets_.begin(), powerNets_.end(), net) == powerNets_.end()) {
    powerNets_.push_back(net);
  }
}

void schematic::addIONet(Net *net) {
  if (std::find(IONets_.begin(), IONets_.end(), net) == IONets_.end()) {
    IONets_.push_back(net);
  }
}

Pin::Pin(const std::string &pinName, Net *net, Instance *instance, const std::string &pinType, const std::string &pinIO,
         bool ifIOPin, bool ifPowerPin)
    : pinName_(pinName), net_(net), instance_(instance), pinType_(pinType), pinIO_(pinIO), ifIOPin_(ifIOPin),
      ifPowerPin_(ifPowerPin) {}

Instance::Instance(const std::string &instanceName, const std::string &instanceType, int finNum, int nfin)
    : instanceName_(instanceName), instanceType_(instanceType), finNum_(finNum) {
  if (finNum_ > 0) {
    finger = nfin / finNum_;
  } else {
    finger = 0;
  }
}

void Instance::addPin(Pin *pin) { pins_.push_back(pin); }

Net::Net(const std::string &netName) : netName_(netName) {}

void Net::addPin(Pin *pin, bool isSource) {
  if (isSource && sourcePin_ == nullptr) {
    sourcePin_ = pin;
  } else {
    sinkPins_.push_back(pin);
  }

  pins_.push_back(pin);
}

void Net::markPowerNet() { ifPowerNet_ = true; }

void Net::markIONet() { ifIONet_ = true; }

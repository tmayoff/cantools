#ifndef ACTUATOR_HPP_
#define ACTUATOR_HPP_

#include <cinttypes>
#include <sstream>
#include <string>

//
#include <nlohmann/json.hpp>

//
#include <CAN/CAN.hpp>
#include <CAN/CANQueue.hpp>

int32_t ConvertRange(float num) {
  float nAngle = num / 180;
  return nAngle * INT32_MAX;
}

struct Actuator {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Actuator, nodeID);

  uint32_t nodeID;

  float position;
  float lastPosition;

  Actuator() {}
  Actuator(uint32_t _nodeID) : nodeID(_nodeID) {}

  void UpdatePhysicalPosition() {
    std::vector<uint8_t> data(sizeof(int32_t));
    auto val = ConvertRange(position);
    std::memcpy(data.data(), &val, sizeof(int32_t));
    data.emplace(data.begin(), nodeID);
    data.emplace(data.begin(), CAN::Intents::MOVE);

    CANQueue::GetInstance().Queue({0x1F4, data});
  }

  std::string ToString() {
    std::stringstream ss;
    ss << "Node ID: " << std::to_string(nodeID);
    return ss.str();
  }
};

#endif  // ACTUATOR_HPP_

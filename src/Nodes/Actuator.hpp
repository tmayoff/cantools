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
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Actuator, nodeID, position, lowerLimit,
                                 upperLimit);

  uint32_t nodeID;

  float position = 0;

  float lowerLimit = 0;
  float upperLimit = 0;

  Actuator() {}
  Actuator(uint32_t _nodeID) : nodeID(_nodeID) {}

  void Update() {
    if (position != lastPosition) {
      UpdatePhysicalPosition();
      lastPosition = position;
    }
  }

  void SetLimits() {
    auto lower = CAN::Serialize<int32_t>({ConvertRange(lowerLimit)});
    lower.emplace(lower.begin(), nodeID);
    lower.emplace(lower.begin(), CAN::Intents::SET_LIMIT);

    auto upper = CAN::Serialize<int32_t>({ConvertRange(upperLimit)});
    upper.emplace(upper.begin(), nodeID);
    upper.emplace(upper.begin(), CAN::Intents::SET_LIMIT);

    SetTorque(false);
    CANQueue::GetInstance().Queue({0x1F4, lower});
    CANQueue::GetInstance().Queue({0x1F4, upper});
    SetTorque(true);
  }

  void SetTorque(bool enabled) {
    std::vector<uint8_t> torque;
    torque.emplace(torque.begin(), enabled ? 0x01 : 0x00);
    torque.emplace(torque.begin(), nodeID);
    torque.emplace(torque.begin(), CAN::Intents::SET_TORQUE);

    CANQueue::GetInstance().Queue({0x1F4, torque});
  }

  void UpdatePhysicalPosition() {
    std::vector<uint8_t> data(sizeof(int32_t));
    auto val = ConvertRange(position);
    std::memcpy(data.data(), &val, sizeof(int32_t));
    data.emplace(data.begin(), nodeID);
    data.emplace(data.begin(), CAN::Intents::SINGLE_MOVE);

    CANQueue::GetInstance().Queue({0x1F4, data});
  }

  std::string ToString() {
    std::stringstream ss;
    ss << "Node ID: " << std::to_string(nodeID);
    return ss.str();
  }

 private:
  float lastPosition;
  float lastLowerLimit;
  float lastUpperLimit;
};

#endif  // ACTUATOR_HPP_

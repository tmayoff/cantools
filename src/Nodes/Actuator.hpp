#ifndef ACTUATOR_HPP_
#define ACTUATOR_HPP_

#include <cinttypes>
#include <sstream>
#include <string>

struct Actuator {
  uint32_t nodeID;

  Actuator() {}
  Actuator(uint32_t _nodeID) : nodeID(_nodeID) {}

  std::string ToString() {
    std::stringstream ss;
    ss << "Node ID: " << std::to_string(nodeID);
    return ss.str();
  }
};

#endif  // ACTUATOR_HPP_
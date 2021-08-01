/**
 * @file CAN.hpp
 * @author Tyler Mayoff
 * @brief
 * @version 0.1
 * @date 2021-04-18
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef AVATAR_CORE_INCLUDE_EMBEDDED_CAN_HPP_
#define AVATAR_CORE_INCLUDE_EMBEDDED_CAN_HPP_

// C system headers
#include <stdint.h>

// C++ system headers
#include <cstring>
#include <functional>
#include <vector>

namespace CAN {
enum Intents { MOVE = 0x15 };

struct Data {
  unsigned int nodeID;
  std::vector<uint8_t> data;
};
typedef std::function<void(Data)> CANCallback;

template <typename T>
std::vector<T> Deserialize(std::vector<uint8_t> data, int dataCount) {
  std::vector<T> retVal;

  int offset = 0;
  int typeSize = sizeof(T);

  for (int i = 0; i < dataCount; i++) {
    offset += i * typeSize;

    T val;
    memcpy(&val, data.data() + offset, typeSize);
    retVal.push_back(val);
  }
  return retVal;
}

template <typename T>
std::vector<uint8_t> Serialize(std::vector<T> data) {
  int typeSize = sizeof(T);
  int kDataArraySize = typeSize * data.size();
  auto buffer = std::vector<uint8_t>(kDataArraySize);

  int offset = 0;

  for (uint64_t i = 0; i < data.size(); i++) {
    offset += i * typeSize;
    std::memcpy(buffer.data() + offset, &data[i], typeSize);
  }
  return buffer;
}
}  // namespace CAN

#endif  // AVATAR_CORE_INCLUDE_EMBEDDED_CAN_HPP_

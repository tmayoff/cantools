/**
 * @file CANSocket.hpp
 * @author Tyler Mayoff
 * @brief Socket object for the CAN Protocol
 * @version 0.1
 * @date 2021-04-13
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef AVATAR_CORE_INCLUDE_EMBEDDED_CANSOCKET_HPP_
#define AVATAR_CORE_INCLUDE_EMBEDDED_CANSOCKET_HPP_

// C system headers

#include <cstdint>

// C++ system headers

#include <mutex>  // NOLINT
#include <string>
#include <vector>

// Other headers
#include <CAN/CAN.hpp>

class CANSocket {
 public:
  CANSocket() {}
  explicit CANSocket(std::string interfaceName);
  ~CANSocket();

  void Close();
  void Send(unsigned int id, std::vector<uint8_t> data);
  void SendFD(unsigned int id, std::vector<uint8_t> data);
  CAN::Data Recv();

  bool opened = false;

 private:
  int sockfd;

  std::mutex mtx;
};

#endif  // AVATAR_CORE_INCLUDE_EMBEDDED_CANSOCKET_HPP_

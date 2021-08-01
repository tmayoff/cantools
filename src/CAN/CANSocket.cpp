/**
 * @file CANSocket.cpp
 * @author Tyler Mayoff
 * @brief
 * @version 0.1
 * @date 2021-04-14
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <CAN/CANSocket.hpp>

// C system headers
#include <errno.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <signal.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

// C++ system headers
#include <cstring>

//
#include <SocketExceptions.hpp>

CANSocket::CANSocket(std::string interfaceName) {
  signal(SIGPIPE, SIG_IGN);
  opened = false;

  sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
  if (sockfd < 0) {
    throw SocketExceptions::SocketException(strerror(errno));
  }

  static const int canfd_on = 1;
  setsockopt(sockfd, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &canfd_on,
             sizeof(canfd_on));

  // Retreive Interface Index for Interface name
  struct ifreq ifr = {};
  snprintf(ifr.ifr_name, IFNAMSIZ - 1, interfaceName.c_str());
  ifr.ifr_name[IF_NAMESIZE - 1] = '\0';
  ifr.ifr_ifindex = if_nametoindex(ifr.ifr_name);
  if (!ifr.ifr_ifindex) {
    throw SocketExceptions::InterfaceNotFoundException(strerror(errno));
  }

  // Bind the socket to the CAN interface
  struct sockaddr_can addr = {};
  addr.can_family = AF_CAN;
  addr.can_ifindex = ifr.ifr_ifindex;

  if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    throw SocketExceptions::SocketException(strerror(errno));
  }

  opened = true;
}

CANSocket::~CANSocket() {
  Close();
  opened = false;
}

void CANSocket::Close() {
  opened = false;
  shutdown(sockfd, SHUT_RDWR);
  close(sockfd);
}

void CANSocket::Send(unsigned int id, std::vector<uint8_t> data) {
  struct can_frame frame = {};
  frame.can_id = id;
  if (data.size() == 0) frame.can_id |= CAN_RTR_FLAG;
  frame.can_dlc = data.size();
  std::memcpy(frame.data, data.data(), data.size());

  auto numBytes = write(sockfd, &frame, CAN_MTU);
  if (numBytes < 0) {
    throw SocketExceptions::SendException(strerror(errno));
  } else if (numBytes != CAN_MTU) {
    throw SocketExceptions::CanFrameNotFullException();
  }
}

void CANSocket::SendFD(unsigned int id, std::vector<uint8_t> data) {
  struct canfd_frame frame = {};
  frame.can_id = id;
  if (data.size() == 0) frame.can_id |= CAN_RTR_FLAG;
  frame.len = data.size();
  std::memcpy(frame.data, data.data(), data.size());

  auto numBytes = write(sockfd, &frame, CANFD_MTU);
  if (numBytes < 0) {
    throw SocketExceptions::SendException(strerror(errno));
  }
}

CAN::Data CANSocket::Recv() {
  int numBytes;
  struct can_frame frame;

  numBytes = recv(sockfd, &frame, CANFD_MTU, 0);
  if (numBytes < 0) {
    throw SocketExceptions::ReadException(strerror(errno));
  } else if (numBytes == 0) {
    throw SocketExceptions::SocketClosedException(strerror(errno));
  } else if (numBytes != CAN_MTU && numBytes != CANFD_MTU) {
    throw SocketExceptions::CanFrameNotFullException();
  }

  return {frame.can_id,
          std::vector<uint8_t>(frame.data, frame.data + frame.can_dlc)};
}

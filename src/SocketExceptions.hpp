///  @file SocketExceptions.hpp
///  @author Tyler Mayoff
///  @brief
///  @version 0.1
///  @date 2021-06-24
///
///  @copyright Copyright (c) 2021
///

#ifndef COMMON_SOCKETEXCEPTIONS_HPP_
#define COMMON_SOCKETEXCEPTIONS_HPP_

#include <stdexcept>
#include <string>

namespace SocketExceptions {
struct SocketException : std::runtime_error {
  explicit SocketException(const std::string& msg) : std::runtime_error(msg) {}
  SocketException() : std::runtime_error("Socket Exception") {}
};

struct NotListeningException : SocketException {
  explicit NotListeningException(const std::string& msg)
      : SocketException(msg) {}
  NotListeningException() : SocketException("Socket not listening") {}
};

struct ConnectionFailedException : SocketException {
  explicit ConnectionFailedException(const std::string& msg)
      : SocketException(msg) {}
  ConnectionFailedException() : SocketException("Failed to connect") {}
};

struct ReadException : SocketException {
  explicit ReadException(const std::string& msg) : SocketException(msg) {}
  ReadException() : SocketException("Read failed") {}
};

struct MessageLengthOutOfRangeException : ReadException {
  MessageLengthOutOfRangeException()
      : ReadException("Incoming message length out of range") {}
};

struct SocketClosedException : SocketException {
  explicit SocketClosedException(const std::string& msg)
      : SocketException(msg) {}
  SocketClosedException() : SocketException("Socket Closed") {}
};

struct SendException : SocketException {
  explicit SendException(const std::string& msg) : SocketException(msg) {}
  SendException() : SocketException("Error Sending") {}
};

struct MessageTooBigException : SocketException {
  MessageTooBigException() : SocketException("Message too big") {}
};

struct EmptyMessageException : SocketException {
  EmptyMessageException() : SocketException("Empty Message") {}
};

struct InterfaceNotFoundException : SocketException {
  explicit InterfaceNotFoundException(const std::string& msg)
      : SocketException(msg) {}
  InterfaceNotFoundException() : SocketException("Interface Not Found") {}
};

struct CanFrameNotFullException : SocketException {
  CanFrameNotFullException() : SocketException("Can Frame Not Full") {}
};

}  // namespace SocketExceptions
#endif  // COMMON_SOCKETEXCEPTIONS_HPP_

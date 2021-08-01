#ifndef CANQUEUE_HPP_
#define CANQUEUE_HPP_

// C++ system headers
#include <functional>
#include <queue>
#include <thread>

//
#include <CAN/CAN.hpp>
#include <CAN/CANSocket.hpp>
#include <Singleton.hpp>

typedef std::function<void(CAN::Data)> CANEventCallback;
typedef class CANQueue : public Singleton<CANQueue> {
  friend class Singleton<CANQueue>;

 public:
  CANQueue();
  ~CANQueue();

  void SetCANEventCallback(CANEventCallback cb);

  void Queue(CAN::Data data);

 private:
  void SendLoop();
  void ReadLoop();

 private:
  CANEventCallback OnCANReceive;

  bool running;

  CANSocket socket;

  std::queue<CAN::Data> canQueue;

  std::thread sendThread;
  std::thread readThread;
};

#endif  // CANQUEUE_HPP_

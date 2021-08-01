#include <CAN/CANQueue.hpp>

//
#include <unistd.h>

//
#include <SocketExceptions.hpp>

CANQueue::CANQueue() : socket("can0") {
  sendThread = std::thread(std::bind(&CANQueue::SendLoop, this));
  readThread = std::thread(std::bind(&CANQueue::ReadLoop, this));
}

CANQueue::~CANQueue() {
  socket.Close();
  if (sendThread.joinable()) sendThread.join();
  if (readThread.joinable()) readThread.join();
}

void CANQueue::SetCANEventCallback(CANEventCallback cb) { OnCANReceive = cb; }

void CANQueue::Queue(CAN::Data data) { canQueue.push(data); }

void CANQueue::SendLoop() {
  while (socket.opened) {
    if (canQueue.size() == 0) continue;
    auto d = canQueue.front();
    canQueue.pop();
    socket.Send(d.nodeID, d.data);
    usleep(10000);
  }
}

void CANQueue::ReadLoop() {
  while (socket.opened) {
    try {
      auto d = socket.Recv();
      if (OnCANReceive) OnCANReceive(d);
    } catch (std::exception &e) {
      // printf("%s\n", e.what());
    }
  }
}

#ifndef APPLICATIONEVENTS_HPP
#define APPLICATIONEVENTS_HPP

#include <cinttypes>

#include <Events/Event.hpp>

class WindowResizeEvent : public Event {
public:
  WindowResizeEvent(uint32_t width, uint32_t height)
      : width(width), height(height) {}

  inline uint32_t GetWidth() const { return width; }
  inline uint32_t GetHeight() const { return height; }

  //   std::string ToString() const override {
  //     std::stringstream ss;
  //     ss << "WindowResizeEvent: " << width << ", " << height;
  //     return ss.str();
  //   }

  static EventType GetStaticType() { return EventType::WindowResize; }
  EventType GetEventType() const override { return GetStaticType(); }
  //   const char *GetName() const override { return "WindowResize"; }

  //   int GetCategoryFlags() const override { return EventCategoryApplication;
  //   }

private:
  uint32_t width, height;
};

class WindowCloseEvent : public Event {
public:
  EventType GetEventType() const override { return GetStaticType(); }
  static EventType GetStaticType() { return EventType::WindowClose; }
};

#endif // APPLICATIONEVENTS_HPP_

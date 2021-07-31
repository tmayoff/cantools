#include <Events/Event.hpp>

class MouseMovedEvent : public Event {
 public:
  MouseMovedEvent(float x, float y) : mouseX(x), mouseY(y) {}

  inline float GetX() const { return mouseX; }
  inline float GetY() const { return mouseY; }

  //   std::string ToString() const override {
  //     std::stringstream ss;
  //     ss << "MouseMovedEvent: " << mouseX << ", " << mouseY;
  //     return ss.str();
  //   }

  static EventType GetStaticType() { return EventType::MouseMoved; }
  EventType GetEventType() const override { return GetStaticType(); }
  //   const char *GetName() const override { return "MouseMoved"; }

  //   int GetCategoryFlags() const override {
  //     return EventCategoryMouse | EventCategoryInput;
  //   }

 private:
  float mouseX, mouseY;
};

class MouseScrolledEvent : public Event {
 public:
  MouseScrolledEvent(float xOffset, float yOffset)
      : xOffset(xOffset), yOffset(yOffset) {}

  inline float GetXOffset() const { return xOffset; }
  inline float GetYOffset() const { return yOffset; }

  static EventType GetStaticType() { return EventType::MouseScrolled; }
  EventType GetEventType() const override { return GetStaticType(); }
  //   const char *GetName() const override { return "MouseScrolled"; }

  //   int GetCategoryFlags() const override {
  //     return EventCategoryMouse | EventCategoryInput;
  //   }

 private:
  float xOffset, yOffset;
};

class MouseButtonEvent : public Event {
 public:
  inline int GetMouseButton() const { return button; }

  //   int GetCategoryFlags() const override {
  //     return EventCategoryMouse | EventCategoryInput;
  //   }

 protected:
  explicit MouseButtonEvent(int button) : button(button) {}

  int button;
};

class MouseButtonPressedEvent : public MouseButtonEvent {
 public:
  explicit MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

  static EventType GetStaticType() { return EventType::MouseButtonPressed; }
  EventType GetEventType() const override { return GetStaticType(); }
  //   const char *GetName() const override { return "MouseButtonPressed"; }
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
 public:
  explicit MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

  static EventType GetStaticType() { return EventType::MouseButtonReleased; }
  EventType GetEventType() const override { return GetStaticType(); }
  //   const char *GetName() const override { return "MouseButtonReleased"; }
};

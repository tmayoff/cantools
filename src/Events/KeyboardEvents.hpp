#ifndef KEYBOARDEVENTS_HPP
#define KEYBOARDEVENTS_HPP

#include <sstream>
#include <string>

#include <Events/Event.hpp>

class KeyEvent : public Event {
public:
  inline int GetKeyCode() const { return keyCode; }

  //   int GetCategoryFlags() const override {
  //     return EventCategoryKeyboard | EventCategoryInput;
  //   }

protected:
  explicit KeyEvent(int keycode) : keyCode(keycode) {}

  int keyCode;
};

class KeyPressedEvent : public KeyEvent {
public:
  KeyPressedEvent(int keycode, int repeatCount)
      : KeyEvent(keycode), repeatCount(repeatCount) {}

  inline int GetRepeatCount() const { return repeatCount; }

  //   std::string ToString() const override {
  //     std::stringstream ss;
  //     ss << "KeyPressedEvent: " << keyCode << " (" << repeatCount << "
  //     repeats)"; return ss.str();
  //   }

  static EventType GetStaticType() { return EventType::KeyPressed; }
  EventType GetEventType() const override { return GetStaticType(); }
  //   const char *GetName() const override { return "KeyPressed"; }

private:
  int repeatCount;
};

class KeyReleasedEvent : public KeyEvent {
public:
  explicit KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

  //   std::string ToString() {
  //     std::stringstream ss;
  //     ss << "KeyReleasedEvent: " << keyCode;
  //     return ss.str();
  //   }

  static EventType GetStaticType() { return EventType::KeyReleased; }
  EventType GetEventType() const override { return GetStaticType(); }
  //   const char *GetName() const override { return "KeyReleased"; }
};

class KeyTypedEvent : public Event {
public:
  explicit KeyTypedEvent(const char *key) : c(key) {}

  const char *GetKey() const { return c; }

  std::string ToString() {
    std::stringstream ss;
    ss << "KeyTyped: " << c;
    return ss.str();
  }

  static EventType GetStaticType() { return EventType::KeyTyped; }
  EventType GetEventType() const override { return GetStaticType(); }
  //   const char *GetName() const override { return "KeyTyped"; }

  //   int GetCategoryFlags() const override {
  //     return EventCategoryKeyboard | EventCategoryInput;
  //   }

private:
  const char *c;
};

#endif // KEYBOARDEVENTS_HPP_

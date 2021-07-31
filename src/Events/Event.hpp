#ifndef EVENT_HPP_
#define EVENT_HPP_

enum class EventType {
  NONE = 0,
  WindowClose,
  WindowResize,
  KeyPressed,
  KeyReleased,
  KeyTyped,
  MouseMoved,
  MouseScrolled,
  MouseButtonPressed,
  MouseButtonReleased
};

class Event {
  friend class EventDispatcher;

 public:
  virtual ~Event() = default;

  virtual EventType GetEventType() const = 0;
};

class EventDispatcher {
  friend class Event;

 public:
  explicit EventDispatcher(Event &event) : event(event) {}

  template <typename T, typename F>
  void Dispatch(const F &func) {
    if (event.GetEventType() == T::GetStaticType())
      func(static_cast<T &>(event));
  }

 private:
  Event &event;
};

#endif  // EVENT_HPP_

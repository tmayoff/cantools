#ifndef EVENT_HPP_
#define EVENT_HPP_

enum class EventType { NONE = 0, WINDOW_CLOSE };

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

  template <typename T, typename F> void Dispatch(const F &func) {
    if (event.GetEventType() == T::GetStaticType())
      func(static_cast<T &>(event));
  }

private:
  Event &event;
};

class WindowCloseEvent : public Event {
public:
  EventType GetEventType() const override { return GetStaticType(); }
  static EventType GetStaticType() { return EventType::WINDOW_CLOSE; }
};

#endif // EVENT_HPP_

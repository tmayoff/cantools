#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>

#include <functional>

#include <Events/Event.hpp>

typedef std::function<void(Event &)> EventCallback;

class Window {
public:
  Window();
  ~Window();

  void Update();

  void BindOnEvent(EventCallback cb);
  SDL_Window *GetNativeWindow();
  const SDL_GLContext &GetGLContext();

private:
  SDL_Window *window;
  SDL_GLContext glContext;

  EventCallback OnEventCallback;
};

#endif // WINDOW_HPP

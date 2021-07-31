#ifndef APPLICATION_HPP_
#define APPLICATION_HPP_

#include <Event.hpp>
#include <Window.hpp>

class Application {
public:
  Application();
  ~Application();

  void MainLoop();

  void OnEvent(Event &e);

private:
  void DrawDockSpace();

private:
  bool running = true;

  Window window;
};

#endif // APPLICATION_HPP_

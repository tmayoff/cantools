#ifndef APPLICATION_HPP_
#define APPLICATION_HPP_

// C++ system headers
#include <memory>
#include <vector>
// ==================

#include <Events/ApplicationEvents.hpp>
#include <Events/Event.hpp>
#include <Events/KeyboardEvents.hpp>
#include <Layer.hpp>
#include <Window.hpp>

class Application {
 public:
  Application();
  ~Application();

  void MainLoop();

  void OnEvent(Event &e);

  void PushLayer(Layer *layer);

 private:
  void DrawDockSpace();

 private:
  bool running = true;
  Window window;

  std::vector<std::shared_ptr<Layer>> layerStack;
};

#endif  // APPLICATION_HPP_

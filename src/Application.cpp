#include <Application.hpp>

// C libraries
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

//
#include <Events/ApplicationEvents.hpp>

#define BIND_EVENT_CB(fn) std::bind(&fn, this, std::placeholders::_1)

Application::Application() {
  window.BindOnEvent(BIND_EVENT_CB(Application::OnEvent));

  //   Initialize ImGui
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImGui::StyleColorsDark();

  ImGui_ImplSDL2_InitForOpenGL(window.GetNativeWindow(), window.GetGLContext());
  ImGui_ImplOpenGL3_Init("#version 410");
}

Application::~Application() {}

void Application::MainLoop() {
  while (running) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    DrawDockSpace();

    for (auto l : layerStack) l->OnUpdate();

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    window.Update();
  }
}

void Application::OnEvent(Event &e) {
  EventDispatcher dispatcher(e);
  dispatcher.Dispatch<WindowCloseEvent>(
      [this](WindowCloseEvent &e) { running = false; });

  dispatcher.Dispatch<KeyPressedEvent>(
      BIND_EVENT_CB(Application::OnKeyPressedEvent));
  dispatcher.Dispatch<KeyReleasedEvent>(
      BIND_EVENT_CB(Application::OnKeyReleasedEvent));
  dispatcher.Dispatch<KeyTypedEvent>(
      BIND_EVENT_CB(Application::OnKeyTypedEvent));
}

void Application::PushLayer(Layer *layer) { layerStack.emplace_back(layer); }

void Application::DrawDockSpace() {
  ImGuiWindowFlags windowFlags =
      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

  const ImGuiViewport *viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);
  windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoBringToFrontOnFocus |
                 ImGuiWindowFlags_NoNavFocus;

  static bool open = true;
  ImGui::Begin("Main DockSpace", &open, windowFlags);

  ImGuiIO &io = ImGui::GetIO();
  ImGuiID dockspaceID = ImGui::GetID("Main_DockSpace");
  ImGui::DockSpace(dockspaceID, {0.0f, 0.0f}, 0);

  ImGui::End();
}

void Application::OnKeyTypedEvent(KeyTypedEvent &e) {
  ImGuiIO &io = ImGui::GetIO();
  io.AddInputCharactersUTF8(e.GetKey());
}

void Application::OnKeyPressedEvent(KeyPressedEvent &e) {
  ImGuiIO &io = ImGui::GetIO();
  io.KeysDown[e.GetKeyCode()] = true;
  io.KeyCtrl =
      io.KeysDown[SDL_SCANCODE_LCTRL] || io.KeysDown[SDL_SCANCODE_RCTRL];
  io.KeyAlt = io.KeysDown[SDL_SCANCODE_LALT] || io.KeysDown[SDL_SCANCODE_RALT];
  io.KeyShift =
      io.KeysDown[SDL_SCANCODE_LSHIFT] || io.KeysDown[SDL_SCANCODE_RSHIFT];
}

void Application::OnKeyReleasedEvent(KeyReleasedEvent &e) {
  ImGuiIO &io = ImGui::GetIO();
  io.KeysDown[e.GetKeyCode()] = false;
  io.KeyCtrl =
      io.KeysDown[SDL_SCANCODE_LCTRL] || io.KeysDown[SDL_SCANCODE_RCTRL];
  io.KeyAlt = io.KeysDown[SDL_SCANCODE_LALT] || io.KeysDown[SDL_SCANCODE_RALT];
  io.KeyShift =
      io.KeysDown[SDL_SCANCODE_LSHIFT] || io.KeysDown[SDL_SCANCODE_RSHIFT];
}

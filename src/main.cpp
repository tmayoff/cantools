#include <SDL2/SDL.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include <Application.hpp>

static bool running = true;

static Application app;

int main() {
  app.MainLoop();

  //   IMGUI_CHECKVERSION();
  //   ImGui::CreateContext();
  //   ImGuiIO &io = ImGui::GetIO();
  //   io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  //   //   io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  //   ImGui::StyleColorsDark();

  //   ImGui_ImplSDL2_InitForOpenGL(window, glContext);
  //   ImGui_ImplOpenGL3_Init("#version 410");

  //   while (running) {
  //     SDL_Event event;
  //     while (SDL_PollEvent(&event)) {
  //       ImGui_ImplSDL2_ProcessEvent(&event);
  //       if (event.type == SDL_QUIT)
  //         running = false;
  //       if (event.type == SDL_WINDOWEVENT &&
  //           event.window.event == SDL_WINDOWEVENT_CLOSE &&
  //           event.window.windowID == SDL_GetWindowID(window))
  //         running = false;
  //     }

  //     ImGui_ImplOpenGL3_NewFrame();
  //     ImGui_ImplSDL2_NewFrame();
  //     ImGui::NewFrame();

  //     ImGuiWindowFlags windowFlags =
  //         ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

  //     const ImGuiViewport *viewport = ImGui::GetMainViewport();
  //     ImGui::SetNextWindowPos(viewport->WorkPos);
  //     ImGui::SetNextWindowSize(viewport->WorkSize);
  //     ImGui::SetNextWindowViewport(viewport->ID);
  //     windowFlags |= ImGuiWindowFlags_NoTitleBar |
  //     ImGuiWindowFlags_NoCollapse |
  //                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
  //                    ImGuiWindowFlags_NoBringToFrontOnFocus |
  //                    ImGuiWindowFlags_NoNavFocus;

  //     static bool open = true;
  //     ImGui::Begin("Main DockSpace", &open, windowFlags);

  //     ImGuiIO &io = ImGui::GetIO();
  //     ImGuiID dockspaceID = ImGui::GetID("Main_DockSpace");
  //     ImGui::DockSpace(dockspaceID, {0.0f, 0.0f}, 0);

  //     ImGui::End();

  //     ImGui::Render();
  //     glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
  //     glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  //     glClear(GL_COLOR_BUFFER_BIT);
  //     ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  //     SDL_GL_SwapWindow(window);
  //   }

  //   SDL_DestroyWindow(window);
}

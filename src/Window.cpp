#include <GL/glew.h>
#include <imgui_impl_sdl.h>

#include <Events/ApplicationEvents.hpp>
#include <Events/KeyboardEvents.hpp>
#include <Events/MouseEvent.hpp>
#include <Window.hpp>

Window::Window() {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  window = SDL_CreateWindow(
      "CAN Tools", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
      SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
          SDL_WINDOW_ALLOW_HIGHDPI);
  glContext = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, glContext);
  SDL_GL_SetSwapInterval(1);

  glewInit();
}

Window::~Window() {
  SDL_GL_DeleteContext(glContext);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Window::Update() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
      case SDL_WINDOWEVENT: {
        switch (e.window.event) {
          case SDL_WINDOWEVENT_RESIZED: {
            // TODO(tyler) Handle resize;
            // windowData.Width = e.window.data1;
            // windowData.Height = e.window.data2;
            WindowResizeEvent event(e.window.data1, e.window.data2);
            OnEventCallback(event);
            break;
          }
          case SDL_WINDOWEVENT_CLOSE: {
            WindowCloseEvent event;
            OnEventCallback(event);
            break;
          }
        }
        break;
      }
      case SDL_TEXTINPUT: {
        KeyTypedEvent event(e.text.text);
        OnEventCallback(event);
        break;
      }
      case SDL_KEYUP: {
        KeyReleasedEvent event(e.key.keysym.scancode);
        OnEventCallback(event);
        break;
      }
      case SDL_KEYDOWN: {
        KeyPressedEvent event(e.key.keysym.scancode, e.key.repeat);
        OnEventCallback(event);
        break;
      }
      case SDL_MOUSEMOTION: {
        MouseMovedEvent event(e.motion.x, e.motion.y);
        OnEventCallback(event);
        break;
      }
      case SDL_MOUSEWHEEL: {
        MouseScrolledEvent event(e.wheel.x, e.wheel.y);
        OnEventCallback(event);
        break;
      }
      case SDL_MOUSEBUTTONUP: {
        MouseButtonReleasedEvent event(e.button.button - 1);
        OnEventCallback(event);
        break;
      }
      case SDL_MOUSEBUTTONDOWN: {
        MouseButtonPressedEvent event(e.button.button - 1);
        OnEventCallback(event);
        break;
      }
      case SDL_QUIT: {
        WindowCloseEvent event;
        OnEventCallback(event);
        break;
      }
    }
    ImGui_ImplSDL2_ProcessEvent(&e);
  }

  SDL_GL_SwapWindow(window);
}

void Window::BindOnEvent(EventCallback cb) { OnEventCallback = cb; }

SDL_Window *Window::GetNativeWindow() { return window; }

const SDL_GLContext &Window::GetGLContext() { return glContext; }

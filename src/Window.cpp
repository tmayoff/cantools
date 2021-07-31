#include <Window.hpp>

#include <GL/glew.h>

#include <Event.hpp>

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
  SDL_GL_SwapWindow(window);

  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      WindowCloseEvent event;
      OnEventCallback(event);
    }
    if (event.type == SDL_WINDOWEVENT &&
        event.window.event == SDL_WINDOWEVENT_CLOSE) {
      WindowCloseEvent event;
      OnEventCallback(event);
    }
  }
}

void Window::BindOnEvent(EventCallback cb) { OnEventCallback = cb; }

SDL_Window *Window::GetNativeWindow() { return window; }

const SDL_GLContext &Window::GetGLContext() { return glContext; }

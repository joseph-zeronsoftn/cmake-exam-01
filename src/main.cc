#include <iostream>
#include <memory>

#include "SDL.h"
#include "SDL_ttf.h"
#include "sdlpp.h"

extern "C" {
extern unsigned char _binary_font_ttf_start[];
extern unsigned char _binary_font_ttf_end[];
extern unsigned char _binary_font_ttf_size[];
}

int ui_app() {
  int program_done = 0;
  int current_progress = 0;

  const int screen_width = 400;
  const int screen_height = 80;

  ::sdlpp::SDL_WindowPtr win;
  ::sdlpp::SDL_RendererPtr ren;

  ::sdlpp::SDL_SurfacePtr screen_surface;
  ::sdlpp::SDL_SurfacePtr main_surface;
  ::sdlpp::SDL_SurfacePtr progress_surface;
  ::sdlpp::SDL_SurfacePtr text_surface;

  ::sdlpp::TTF_FontPtr default_font;

  SDL_Rect screen_rect = { 0, 0, screen_width, screen_height };
  SDL_Rect progress_rect = { 0, 0, screen_width, screen_height };

  win.reset(SDL_CreateWindow("Test Application", 100, 100, screen_width, screen_height, SDL_WINDOW_SHOWN));
  if (!win) {
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    return EXIT_FAILURE;
  }

  ren.reset(SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(win.get())));
  if (!ren) {
    std::cerr << "SDL_CreateSoftwareRenderer Error" << SDL_GetError() << std::endl;
    return EXIT_FAILURE;
  }
  {
    std::shared_ptr<::sdlpp::MemoryFile> font_file = ::sdlpp::MemoryFile::create(_binary_font_ttf_start, (size_t)&_binary_font_ttf_size);
    font_file->enableAutoDelete(true);
    default_font.reset(TTF_OpenFontRW(font_file->getRWOps(), 1, 16));
  }
  if (!default_font) {
    std::cerr << "TTF_OpenFont Error" << TTF_GetError() << std::endl;
    return EXIT_FAILURE;
  }

  screen_surface.reset(SDL_GetWindowSurface(win.get()));
  main_surface.reset(SDL_CreateRGBSurface(0, screen_width, screen_height, 32, 0, 0, 0, 0));
  progress_surface.reset(SDL_CreateRGBSurface(0, progress_rect.w, progress_rect.h, 32, 0, 0, 0, 0));
  SDL_FillRect(progress_surface.get(), NULL, SDL_MapRGB(main_surface->format, 0x30, 0xff, 0x30));
  text_surface.reset(TTF_RenderUTF8_Blended(default_font.get(), "Hello World", {0, 0, 0, 255}));

  while (!program_done) {
    SDL_Event event;
    SDL_FillRect(main_surface.get(), NULL, SDL_MapRGB(main_surface->format, 0xdd, 0xdd, 0xdd));

    {
      SDL_Rect progress_bar = {0};
      progress_bar.x = 0;
      progress_bar.y = 0;
      progress_bar.w = (int)(screen_width * (current_progress / 100.0f));
      progress_bar.h = progress_rect.h;
      SDL_BlitSurface(progress_surface.get(), &progress_bar, main_surface.get(), &progress_bar);
    }

    {
      ::sdlpp::SDL_SurfacePtr progress_text_surface(TTF_RenderUTF8_Blended(default_font.get(), "HELLO WORLD", {0, 0, 0, 255}));
      SDL_Rect text_rect = {
          0, 0,
          text_surface->w,
          text_surface->h
      };
      SDL_BlitSurface(text_surface.get(), &screen_rect, main_surface.get(), &text_rect);
    }

    // Handle events on queue
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        // IGNORE
        program_done = 1;
      }
    }

    SDL_BlitSurface(main_surface.get(), &screen_rect, screen_surface.get(), &screen_rect);
    SDL_UpdateWindowSurface(win.get());
    SDL_Delay(100);

    current_progress = (current_progress + 1) % 100;
  }

  return EXIT_SUCCESS;
}

int main(int argc, char* args[])
{
  int rc;

  if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return EXIT_FAILURE;
  }
  if (TTF_Init() != 0) {
    std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
    return EXIT_FAILURE;
  }

  rc = ui_app();

  TTF_Quit();
  SDL_Quit();

  return rc;
}

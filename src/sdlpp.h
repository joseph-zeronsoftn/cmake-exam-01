//
//  All rights reserved ZeronsoftN.
//

#ifndef _SRC_SDLPP_H_
#define _SRC_SDLPP_H_

#include <memory>
#include <SDL.h>
#include <SDL_ttf.h>

extern "C" {
typedef struct _sdlpp_memory_file_rwops {
  SDL_RWops rwops;
  void* cxx_obj;
} _sdlpp_memory_file_rwops_t;
}

namespace sdlpp {

namespace deleter {

struct SDL_WindowDeleter {
  void operator()(SDL_Window* p);
};
struct SDL_RendererDeleter {
  void operator()(SDL_Renderer* p);
};
struct SDL_SurfaceDeleter {
  void operator()(SDL_Surface* p);
};
struct TTF_FontDeleter {
  void operator()(TTF_Font* p);
};

} // namespace deleter

typedef std::unique_ptr<SDL_Window, deleter::SDL_WindowDeleter> SDL_WindowPtr;
typedef std::unique_ptr<SDL_Renderer, deleter::SDL_RendererDeleter> SDL_RendererPtr;
typedef std::unique_ptr<SDL_Surface, deleter::SDL_SurfaceDeleter> SDL_SurfacePtr;

typedef std::unique_ptr<TTF_Font, deleter::TTF_FontDeleter> TTF_FontPtr;

class MemoryFile {
 private:
  std::weak_ptr<MemoryFile> self_weak_;
  std::shared_ptr<MemoryFile> self_shared_;

  _sdlpp_memory_file_rwops_t rwops_;
  const unsigned char *ptr_;
  size_t size_;

  size_t pos_;

  MemoryFile(void* ptr, size_t size);

  static Sint64 SDLCALL c_size(struct SDL_RWops * context);
  static Sint64 SDLCALL c_seek(struct SDL_RWops * context, Sint64 offset, int whence);
  static size_t SDLCALL c_read(struct SDL_RWops * context, void *ptr, size_t size, size_t maxnum);
  static size_t SDLCALL c_write(struct SDL_RWops * context, const void *ptr, size_t size, size_t num);
  static int SDLCALL c_close(struct SDL_RWops * context);

 public:
  static std::shared_ptr<MemoryFile> create(void* ptr, size_t size);

  ~MemoryFile();
  void enableAutoDelete(bool auto_delete);
  SDL_RWops* getRWOps();
};

} // namespace sdlpp

#endif //_SRC_SDLPP_H_

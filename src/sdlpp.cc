//
//  All rights reserved ZeronsoftN.
//

#include "sdlpp.h"

#include <cstring>

namespace sdlpp {

namespace deleter {

void SDL_WindowDeleter::operator()(SDL_Window *p) {
  SDL_DestroyWindow(p);
}

void SDL_RendererDeleter::operator()(SDL_Renderer *p) {
  SDL_DestroyRenderer(p);
}

void SDL_SurfaceDeleter::operator()(SDL_Surface *p) {
  SDL_FreeSurface(p);
}

void TTF_FontDeleter::operator()(TTF_Font *p) {
  TTF_CloseFont(p);
}

} // namespace deleter

MemoryFile::MemoryFile(void *ptr, size_t size) {
  ptr_ = (const unsigned char*)ptr;
  size_ = size;
  pos_ = 0;
  std::memset(&rwops_, 0, sizeof(rwops_));
  rwops_.cxx_obj = this;
  rwops_.rwops.seek = c_seek;
  rwops_.rwops.read = c_read;
  rwops_.rwops.write = c_write;
  rwops_.rwops.size = c_size;
  rwops_.rwops.close = c_close;
}

MemoryFile::~MemoryFile() {
}

void MemoryFile::enableAutoDelete(bool auto_delete) {
  if (auto_delete) {
    self_shared_ = self_weak_.lock();
  } else {
    self_shared_.reset();
  }
}

SDL_RWops* MemoryFile::getRWOps() {
  return (SDL_RWops*)&rwops_;
}

std::shared_ptr<MemoryFile> MemoryFile::create(void *ptr, size_t size) {
  auto instance = std::shared_ptr<MemoryFile>(new MemoryFile(ptr, size));
  instance->self_weak_ = instance;
  return instance;
}

Sint64 SDLCALL MemoryFile::c_size(struct SDL_RWops * context) {
  MemoryFile* self = (MemoryFile*)((_sdlpp_memory_file_rwops_t *)context)->cxx_obj;
  return self->size_;
}
Sint64 SDLCALL MemoryFile::c_seek(struct SDL_RWops * context, Sint64 offset, int whence) {
  MemoryFile* self = (MemoryFile*)((_sdlpp_memory_file_rwops_t *)context)->cxx_obj;
  size_t new_pos = self->pos_;
  switch (whence) {
    case RW_SEEK_SET:
      new_pos = offset;
      break;
    case RW_SEEK_CUR:
      new_pos += offset;
      break;
    case RW_SEEK_END:
      new_pos = self->size_ + offset;
      break;
    default:
      return -1;
  }
  if (new_pos > self->size_) {
    return -1;
  }
  self->pos_ = new_pos;
  return new_pos;
}
size_t SDLCALL MemoryFile::c_read(struct SDL_RWops * context, void *ptr, size_t size, size_t maxnum) {
  MemoryFile* self = (MemoryFile*)((_sdlpp_memory_file_rwops_t *)context)->cxx_obj;
  size_t requested_size = size * maxnum;
  size_t available_size = self->size_ - self->pos_;
  if (available_size > requested_size)
    available_size = requested_size;
  std::memcpy(ptr, self->ptr_ + self->pos_, available_size);
  self->pos_ += available_size;
  return available_size;
}
size_t SDLCALL MemoryFile::c_write(struct SDL_RWops * context, const void *ptr, size_t size, size_t num) {
  MemoryFile* self = (MemoryFile*)((_sdlpp_memory_file_rwops_t *)context)->cxx_obj;
  return 0;
}
int SDLCALL MemoryFile::c_close(struct SDL_RWops * context) {
  MemoryFile* self = (MemoryFile*)((_sdlpp_memory_file_rwops_t *)context)->cxx_obj;
  self->self_shared_.reset();
  return 0;
}

} // namespace sdlpp

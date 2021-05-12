include(FetchContent)

set(SDL2_SOURCE_URL "https://www.libsdl.org/release/SDL2-2.0.14.tar.gz")
set(SDL2_SOURCE_HASH "SHA256=d8215b571a581be1332d2106f8036fcb03d12a70bae01e20f424976d275432bc")

set(SDL_TTF_SOURCE_URL "https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.15.tar.gz")
set(SDL_TTF_SOURCE_HASH "SHA256=a9eceb1ad88c1f1545cd7bd28e7cbc0b2c14191d40238f531a15b01b1b22cd33")

set(FREETYPE_SOURCE_URL "https://download.savannah.gnu.org/releases/freetype/freetype-2.10.4.tar.gz")
set(FREETYPE_SOURCE_HASH "SHA256=5eab795ebb23ac77001cfb68b7d4d50b5d6c7469247b0b01b2c953269f658dac")

set(SDL_SHARED OFF)
set(SDL_STATIC ON)

if (TARGET_IS_LINUX)
    set(X11_SHARED OFF CACHE BOOL "X11_SHARED" FORCE)
    set(VIDEO_X11 ON CACHE BOOL "VIDEO_X11" FORCE)
    set(VIDEO_OPENGL OFF CACHE BOOL "VIDEO_OPENGL" FORCE)
    set(VIDEO_OPENGLES OFF CACHE BOOL "VIDEO_OPENGLES" FORCE)
endif()

message("FetchContent: SDL2: Start")
FetchContent_Declare(
        sdl2
        URL      ${SDL2_SOURCE_URL}
        URL_HASH ${SDL2_SOURCE_HASH}
)
FetchContent_GetProperties(sdl2)
if(NOT sdl2_POPULATED)
    FetchContent_Populate(sdl2)
    add_subdirectory(${sdl2_SOURCE_DIR} ${sdl2_BINARY_DIR})
endif()
message("FetchContent: SDL2: Done")

# 이곳을 수정하시오
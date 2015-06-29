#ifndef PLATFORM_HEADERS_H
#define PLATFORM_HEADERS_H
#ifdef __APPLE__
    #include <SDL2/SDL_opengl.h>
    #include <SDL2/SDL.h>
#else
    #include <SDL_opengl.h>
    #include <SDL.h>
#endif
#endif

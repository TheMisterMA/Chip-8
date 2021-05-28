#include <SDL2/SDL.h>
#include <iostream>

class PlatformInterface
{
private:
    SDL_Window *window{};
    SDL_Renderer *renderer{};
    SDL_Texture *texture{};

public:
    PlatformInterface(const char *title,
             size_t windowWidth,
             size_t windowHeight,
             size_t textureWidth,
             size_t textureHeight);

    ~PlatformInterface();

    void Update(void const *buffer,
                int pitch);

    bool ProcessInput(uint8_t *keys);
};

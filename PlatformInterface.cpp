#include "PlatformInterface.h"

/***
 *  PlatformInterface:
 *      Constructor, initiates the SDL with the given sizes.
 ***/
PlatformInterface::PlatformInterface(const char *title,
                   size_t windowWidth,
                   size_t windowHight,
                   size_t textureWidth,
                   size_t textureHight)
{
    //  Initiates SDL.
    SDL_Init(SDL_INIT_VIDEO);

    //  Creates a window out of the window sizes.
    this->window = SDL_CreateWindow(this->title,
                                    0,
                                    0,
                                    windowWidth,
                                    windowHeight,
                                    SDL_WINDOW_SHOWN);

    //  Creates a renderer out of the window.
    this->renderer = SDL_CreateRenderer(this->window,
                                        -1,
                                        SDL_RENDERER_ACCELERATED);

    //  Creates a texture out of the renderer with the texture sizes.
    this->texture = SDL_CreateTexture(this->renderer,
                                      SDL_PIXELFORMAT_RGBA8888,
                                      SDL_TEXTUREACCESS_STREAMING,
                                      textureWidth,
                                      textureHight);
}

/***
 *  ~PlatformInterface:
 *      Destructor, destroys texture, then the renderer and finally the window sequentially.
 *      In reversed order than how they were made. Finally it quits SDL.
 ***/
PlatformInterface::~PlatformInterface()
{
    SDL_DestroyTexture(this->texture);
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}

/***
 *  Update:
 *      Updates the graphics of the window shown to the
 *      user accourding to the buffer and the pitch given.
 *
 *  @param buffer   The matrix buffer with the boolean values indicating which pixel is on or off.
 *  @param pitch    The number of bytes in a row of pixel data, including padding between lines.
*/
void PlatformInterface::Update(void const *buffer, int pitch)
{
    //  Update the texture accourding the new buffer.
    SDL_UpdateTexture(this->texture,
                      nullptr,
                      buffer,
                      pitch);

    //  Clearing the old renderer.
    SDL_RenderClear(this->renderer);

    //  Copying the renderer out of the new texture.
    SDL_RenderCopy(this->renderer,
                   this->texture,
                   nullptr,
                   nullptr);

    //  Update the Renderer.
    SDL_RenderPresent(this->renderer);
}

/***
 *  ProcessInput:
 *      This function will communicate with the machine this program runs in,
 *      and will update the keys buffer for a matching key being pressed.
 *
 *  @param keys A key buffer of boolean values indicating which keys are being pressed.
 *
 *  @return     This function will return true if the user pressed Escape key to quit the program.
 ***/
bool PlatformInterface::ProcessInput(uint8_t *keys)
{
    bool quit = false;
    SDL_Event event;

    //  Will itterate over the events, all while there is a pending event.
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        //  Will quit if the event type is quit.
        case SDL_QUIT:
        {
            quit = true;
        }
        break;

        //  In the event of a key being pressed, it raises the bit of the matching index in the given buffer.
        case SDL_KEYDOWN:
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
            {
                quit = true;
            }
            break;

            case SDLK_x:
            {
                keys[0] = 1;
            }
            break;

            case SDLK_1:
            {
                keys[1] = 1;
            }
            break;

            case SDLK_2:
            {
                keys[2] = 1;
            }
            break;

            case SDLK_3:
            {
                keys[3] = 1;
            }
            break;

            case SDLK_q:
            {
                keys[4] = 1;
            }
            break;

            case SDLK_w:
            {
                keys[5] = 1;
            }
            break;

            case SDLK_e:
            {
                keys[6] = 1;
            }
            break;

            case SDLK_a:
            {
                keys[7] = 1;
            }
            break;

            case SDLK_s:
            {
                keys[8] = 1;
            }
            break;

            case SDLK_d:
            {
                keys[9] = 1;
            }
            break;

            case SDLK_z:
            {
                keys[0xA] = 1;
            }
            break;

            case SDLK_c:
            {
                keys[0xB] = 1;
            }
            break;

            case SDLK_4:
            {
                keys[0xC] = 1;
            }
            break;

            case SDLK_r:
            {
                keys[0xD] = 1;
            }
            break;

            case SDLK_f:
            {
                keys[0xE] = 1;
            }
            break;

            case SDLK_v:
            {
                keys[0xF] = 1;
            }
            break;
            }
        }
        break;

        //  After the key released, it zeros the matching bit in the buffer.
        case SDL_KEYUP:
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_x:
            {
                keys[0] = 0;
            }
            break;

            case SDLK_1:
            {
                keys[1] = 0;
            }
            break;

            case SDLK_2:
            {
                keys[2] = 0;
            }
            break;

            case SDLK_3:
            {
                keys[3] = 0;
            }
            break;

            case SDLK_q:
            {
                keys[4] = 0;
            }
            break;

            case SDLK_w:
            {
                keys[5] = 0;
            }
            break;

            case SDLK_e:
            {
                keys[6] = 0;
            }
            break;

            case SDLK_a:
            {
                keys[7] = 0;
            }
            break;

            case SDLK_s:
            {
                keys[8] = 0;
            }
            break;

            case SDLK_d:
            {
                keys[9] = 0;
            }
            break;

            case SDLK_z:
            {
                keys[0xA] = 0;
            }
            break;

            case SDLK_c:
            {
                keys[0xB] = 0;
            }
            break;

            case SDLK_4:
            {
                keys[0xC] = 0;
            }
            break;

            case SDLK_r:
            {
                keys[0xD] = 0;
            }
            break;

            case SDLK_f:
            {
                keys[0xE] = 0;
            }
            break;

            case SDLK_v:
            {
                keys[0xF] = 0;
            }
            break;
            }
        }
        break;
        }
    }

    return quit;
}

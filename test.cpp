#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* argv[]) {
    char* filename = argv[1];

    struct stat sb;
    int size = 0;

    if (stat(filename, &sb) != -1) {
        size = sb.st_size;
    }

    ifstream file(filename, ios::binary | ios::ate);
    file.seekg(0, ios::beg);

    vector<char> buffer(size);
    if (file.read(buffer.data(), size)) {
        for (int i = 0; i < size; ++i) {
            cout << buffer[i];
        }
    }

    return 0;
}

/*
void sdl2Test() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Error Initializing SDL: %s\n", SDL_GetError());
    }

    SDL_Window* win = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);

    uint32_t render_flags = SDL_RENDERER_ACCELERATED;

    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

    SDL_Surface* surface = IMG_Load("img.png");

    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);

    SDL_FreeSurface(surface);

    SDL_Rect dest;

    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);

    dest.w /= 6;
    dest.h /= 6;

    dest.x = (1000 - dest.w) / 2;
    dest.y = (1000 - dest.h) / 2;

    int close = 0;
    int speed = 500;

    while (!close) {
        SDL_Event event;
        // SDL_PumpEvents();
        // SDL_RenderSetLogicalSize(rend, 1000, 1000);
        // SDL_SetRenderDrawColor(rend, 0xF2, 0x84, 0x82, 255);

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    close = 1;
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_W:
                        case SDL_SCANCODE_UP:
                            dest.y -= speed / 30;
                            break;
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            dest.x -= speed / 30;
                            break;
                        case SDL_SCANCODE_S:
                        case SDL_SCANCODE_DOWN:
                            dest.y += speed / 30;
                            break;
                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                            dest.x += speed / 30;
                            break;
                        default:
                            break;
                    }
            }
        }

        if (dest.x + dest.w > 1000) dest.x = 1000 - dest.w;
        if (dest.x < 0) dest.x = 0;
        if (dest.y + dest.h > 1000) dest.y = 1000 - dest.h;
        if (dest.y < 0) dest.y = 0;

        SDL_RenderClear(rend);
        SDL_RenderCopy(rend, tex, NULL, &dest);
        SDL_RenderPresent(rend);
        SDL_Delay(1000 / 60);
    }

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);

    SDL_Quit();
}
*/
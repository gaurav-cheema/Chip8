#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
 
#include "chip8.cpp"
#include "graphics.cpp"

using namespace std;

int main(int argc, char** argv) {
    int windowScale = 12;

    char* fileName = "test_opcode.ch8";

    chip8 chip8;
    chip8.init();
    chip8.loadROM(fileName);
    
    Platform platform("CHIP-8 Emulator", VIDEO_WIDTH * windowScale,
                      VIDEO_HEIGHT * windowScale, VIDEO_WIDTH, VIDEO_HEIGHT);
    int texturePitch = sizeof(chip8.screen[0]) * VIDEO_WIDTH;

    bool quit = false;
    auto currentTime = chrono::high_resolution_clock::now();

    while (!quit) {
        quit = platform.ProcessInput(chip8.keyStroke);

        // SDL_GL_SetSwapInterval(1);
        auto newTime = chrono::high_resolution_clock::now();
        float deltaTime = chrono::duration<float, chrono::milliseconds::period>(
                              newTime - currentTime)
                              .count();
        // std::cerr << "Delta Time: " << deltaTime << endl;

        if (deltaTime <= 3) {
            continue;
        }

        currentTime = newTime;
        chip8.execute();
        platform.Update(chip8.screen, texturePitch);
    }
    return 0;
}

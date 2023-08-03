#include "chip8.h"

#include <ncurses.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "SDL2/SDL.h"

const bool DEBUG = false;

using namespace std;

void chip8::init() {
    if (DEBUG) {
        cout << "chip8 :: INIT start" << endl;
    }

    pc = MEMORY_START_ADDRESS;
    sp = 0;
    I = 0;
    opcode = 0;

    for (int i = 0; i < 80; i++) {
        mem[0x50 + i] = font[i];
    }

    if (DEBUG) {
        cout << "chip8 :: INIT end" << endl;
    }
}

void chip8::loadROM(char* fileAddress) {
    if (DEBUG) {
        cout << "chip8 :: Load ROM start" << endl;
    }

    struct stat sb;
    int size = 0;

    if (stat(fileAddress, &sb) != -1) {
        size = sb.st_size;
    } else {
        return;
    }

    ifstream file(fileAddress, ios::binary | ios::ate);
    file.seekg(0, ios::beg);

    vector<char> buffer(size);
    if (file.read(buffer.data(), size)) {
        for (int i = 0; i < size; ++i) {
            mem[MEMORY_START_ADDRESS + i] = buffer[i];
        }
    }

    // FILE* binFile = fopen(fileAddress, "r");

    // if (binFile == NULL) {
    //     std::cerr << "File Error Detected!" << std::endl;
    // }

    // fseek(binFile, 0, SEEK_END);
    // int size = ftell(binFile);
    // fseek(binFile, 0, SEEK_SET);

    // int size = binFile.size();
    // char buffer[MEMORY_SIZE - MEMORY_START_ADDRESS]{0};
    // while (!feof(binFile)) {
    //     fread(buffer, sizeof(buffer), 1, binFile);
    // }

    // for (int i = 0; i < MEMORY_SIZE - MEMORY_START_ADDRESS; i++) {
    //     mem[MEMORY_START_ADDRESS + i] = buffer[i];
    // }

    // delete[] buffer;

    if (DEBUG) {
        cout << "chip8 :: Load ROM end" << endl;
    }
}

void chip8::fetchOpcode() {
    opcode = mem[pc] << 8 | mem[pc + 1];
    pc += 2;
}

void chip8::decode() {
    x = (opcode & 0x0F00) >> 8;
    y = (opcode & 0x00F0) >> 4;
    n = opcode & 0x000F;
    nn = opcode & 0x00FF;
    nnn = opcode & 0x0FFF;
}

void chip8::execute() {
    if (DEBUG) {
        cout << "chip8 :: Execute Cycle start" << endl;
        printf("Opcode fetched: %d /n", opcode);
    }

    fetchOpcode();
    decode();
    uint8_t nib;

    switch (opcode & 0xF000) {
        case 0x0000: {
            nib = opcode & 0x00FF;
            switch (nib) {
                case 0x00E0: {
                    c00E0();
                } break;
                case 0x00EE: {
                    c00EE();
                    break;
                }
                default:
                    cerr << "CRITICAL: opcode 0 error!" << std::endl;
                    break;
            }
            break;
        }

        case 0x1000:
            c1NNN();
            break;

        case 0x2000:
            c2NNN();
            break;

        case 0x3000:
            c3XNN();
            break;

        case 0x4000:
            c4XNN();
            break;

        case 0x5000:
            c5XY0();
            break;

        case 0x6000:
            c6XNN();
            break;

        case 0x7000:
            c7XNN();
            break;

        case 0x8000: {
            nib = opcode & 0x000F;
            switch (nib) {
                case 0x0000:
                    c8XY0();
                    break;
                case 0x0001:
                    c8XY1();
                    break;
                case 0x0002:
                    c8XY2();
                    break;
                case 0x0003:
                    c8XY3();
                    break;
                case 0x0004:
                    c8XY4();
                    break;
                case 0x0005:
                    c8XY5();
                    break;
                case 0x0006:
                    c8XY6();
                    break;
                case 0x0007:
                    c8XY7();
                    break;
                case 0x000E:
                    c8XYE();
                    break;

                default:
                    cerr << "CRITICAL: opcode 8 error!" << endl;
                    break;
            }
            break;
        }

        case 0x9000:
            c9XY0();
            break;

        case 0xA000:
            cANNN();
            break;

        case 0xB000:
            cBNNN();
            break;

        case 0xC000:
            cCXNN();
            break;

        case 0xD000:
            cDXYN();
            break;

        case 0xE000: {
            nib = opcode & 0x000F;
            switch (nib) {
                case 0xE:
                    cEX9E();
                    break;
                case 0x1:
                    cEXA1();
                    break;

                default:
                    cerr << "CRITICAL: opcode E error!" << endl;
                    break;
            }
            break;
        }

        case 0xF000: {
            nib = (opcode & 0x00FF);
            switch (nib) {
                case 0x0007:
                    cFX07();
                    break;
                case 0x000A:
                    cFX0A();
                    break;
                case 0x0015:
                    cFX15();
                    break;
                case 0x0018:
                    cFX18();
                    break;
                case 0x001E:
                    cFX1E();
                    break;
                case 0x0029:
                    cFX29();
                    break;
                case 0x0033:
                    cFX33();
                    break;
                case 0x0055:
                    cFX55();
                    break;
                case 0x0065:
                    cFX65();
                    break;
                default:
                    cerr << "CRITICAL: opcode F error!" << endl;
                    break;
            }
            break;
        }

        default:
            std::cout << "CRITICAL: Unknown Opcode!" << std::endl;
            break;
    }

    if (delayTimer > 0) --delayTimer;
    if (soundTimer > 0) {
        --soundTimer;
        cout << '\a';
        // beep();
    }

    if (DEBUG) {
        cout << "chip8 :: Execute Cycle end" << endl;
    }
}

void chip8::c00E0() { memset(&screen, 0, sizeof(screen)); }

void chip8::c00EE() {
    pc = stack[sp];
    --sp;
}

void chip8::c1NNN() { pc = opcode & 0x0FFF; }

void chip8::c2NNN() {
    sp++;
    stack[sp] = pc;
    pc = opcode & 0x0FFF;
}

void chip8::c3XNN() {
    if (V[x] == nn) {
        pc += 2;
    }
}

void chip8::c4XNN() {
    if (V[x] != nn) {
        pc += 2;
    }
}

void chip8::c5XY0() {
    if (V[x] == V[y]) {
        pc += 2;
    }
}

void chip8::c6XNN() { V[x] = nn; }

void chip8::c7XNN() { V[x] += nn; }

void chip8::c8XY0() { V[x] = V[y]; }

void chip8::c8XY1() { V[x] |= V[y]; }

void chip8::c8XY2() { V[x] &= V[y]; }

void chip8::c8XY3() { V[x] ^= V[y]; }

void chip8::c8XY4() {
    uint16_t sum = V[x] + V[y];

    V[0xF] = sum > 0xFF;
    V[x] = sum & 0xFF;
}

void chip8::c8XY5() {
    uint16_t sum = V[x] - V[y];

    V[0xF] = sum > 0;
    V[x] = sum & 0xFF;
}

void chip8::c8XY6() {
    V[0xF] = V[x] & 1;
    V[x] = V[x] >> 1;
}

void chip8::c8XY7() {
    uint16_t sum = V[y] - V[x];

    V[0xF] = sum > 0;
    V[x] = sum & 0xFF;
}

void chip8::c8XYE() {
    V[0xF] = V[x] >> 7;
    V[x] <<= 1;
}

void chip8::c9XY0() {
    if (V[x] != V[y]) {
        pc += 2;
    }
}

void chip8::cANNN() { I = nnn; }

void chip8::cBNNN() { pc = V[0] + nnn; }

void chip8::cCXNN() {
    srand(time(NULL));
    uint8_t random = rand();
    V[x] = random & nn;
}

//! Not my implementation of DXYN
//! Credit to an Unknown author
void chip8::cDXYN() {
    // Wrap if going beyond screen boundaries
    uint8_t xPos = V[x] % 64;
    uint8_t yPos = V[y] % 32;

    V[0xF] = 0;

    for (unsigned int row = 0; row < n; ++row) {
        uint8_t spriteByte = mem[I + row];

        for (unsigned int col = 0; col < 8; ++col) {
            uint8_t spritePixel = spriteByte & (0x80u >> col);
            uint32_t* screenPixel = &screen[(yPos + row) * 64 + (xPos + col)];

            // Sprite pixel is on
            if (spritePixel) {
                // Screen pixel also on - collision
                if (*screenPixel == 0xFFFFFFFF) {
                    V[0xF] = 1;
                }

                // Effectively XOR with the sprite pixel
                *screenPixel ^= 0xFFFFFFFF;
            }
        }
    }
}

void chip8::cEX9E() {
    if (keyStroke[V[x]]) {
        pc += 2;
    }
}

void chip8::cEXA1() {
    if (!keyStroke[V[x]]) {
        pc += 2;
    }
}

void chip8::cFX07() { V[x] = delayTimer; }

void chip8::cFX0A() {
    bool halt = true;
    for (int i = 0; i < KEYCOUNT; ++i) {
        if (keyStroke[i]) {
            V[x] = i;
            halt = false;
        }
    }
    if (halt) pc -= 2;
}

void chip8::cFX15() { delayTimer = V[x]; }
void chip8::cFX18() { soundTimer = V[x]; }
void chip8::cFX1E() { I += V[x]; }

void chip8::cFX29() {
    uint8_t num = V[x];
    I = num;
    // I = FONTS_START_ADDRESS + (5 * num);
}

void chip8::cFX33() {
    uint8_t num = V[x];

    for (int i = 0; i <= 2; ++i) {
        mem[I - i + 2] = num % 10;
        num /= 10;
    }
}

void chip8::cFX55() {
    for (int i = 0; i <= x; ++i) {
        mem[I + i] = V[i];
    }
}

void chip8::cFX65() {
    for (int i = 0; i <= x; ++i) {
        V[i] = mem[i + I];
    }
}

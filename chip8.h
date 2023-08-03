//* Memory Macros
#define MEMORY_START_ADDRESS 0x200
#define MEMORY_SIZE 0x1000
#define FONTS_START_ADDRESS 0xF00

//* Video Macros
#define VIDEO_WIDTH 64
#define VIDEO_HEIGHT 32

#define KEYCOUNT 16

#include <cstdint>
#include <fstream>
#include <iostream>

class chip8 {

   public:
    void init();
    void loadROM(char* fileAddress);
    void fetchOpcode();
    void decode();
    void execute();

    uint8_t keyStroke[16]{};
    uint32_t screen[VIDEO_WIDTH * VIDEO_HEIGHT];

    private:
    void c00E0();  // cls
    void c00EE();  // ret

    void c1NNN();  // jmp
    void c2NNN();  // call
    void c3XNN();  // skip if Vx == NN
    void c4XNN();  // skip if Vx != NN
    void c5XY0();  // skip if Vx == Vy
    void c6XNN();  // set Vx = NN
    void c7XNN();  // add Vx += NN

    void c8XY0();  // set Vx = Vy
    void c8XY1();  // set Vx |= Vy
    void c8XY2();  // set Vx &= Vy
    void c8XY3();  // set Vx ^= Vy
    void c8XY4();  // add Vx += Vy
    void c8XY5();  // sub Vx -= Vy
    void c8XY6();  // set Vx >>= 1
    void c8XY7();  // sub Vx = Vy - Vx
    void c8XYE();  // set Vx <<= 1

    void c9XY0();  // skip if Vx != Vy
    void cANNN();  // set I = NNN
    void cBNNN();  // jmp NNN+V0
    void cCXNN();  // set Vx = rand() & NN

    void cDXYN();  // draw inst

    void cEX9E();  // skip if key == Vx
    void cEXA1();  // skip if key != Vx

    void cFX07();  // set Vx = delay_timer
    void cFX0A();  // await Vx = key
    void cFX15();  // set delay_timer = Vx
    void cFX18();  // set sound_timer = Vx
    void cFX1E();  // add I += Vx
    void cFX29();  // set I = sprite_addr[Vx]
    void cFX33();  // set bin form Vx
    void cFX55();  // store regs
    void cFX65();  // restore regs

    uint8_t V[16]{};        // GENERAL PURPOSE - VF should not be used by prog
    uint8_t mem[0x1000]{};  // 4096 byte memory
    uint16_t stack[16]{};   // stack frame depth - 16 levels

    uint16_t I;       // ADDRESS REG - use only lowest 12 bits
    uint16_t pc;      // PROGRAM COUNTER
    uint16_t sp = 0;  // points to top of stack
    uint16_t opcode;

    // while >0, decrement by one at 60Hz
    uint8_t delayTimer;
    uint8_t soundTimer;

    uint8_t font[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
        0x20, 0x60, 0x20, 0x20, 0x70,  // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
        0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
        0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
        0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
        0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
        0xF0, 0x80, 0xF0, 0x80, 0x80   // F
    };

    uint8_t x;
    uint8_t y;
    uint8_t n;
    uint8_t nn;
    uint16_t nnn;
};

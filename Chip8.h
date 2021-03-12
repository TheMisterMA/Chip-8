//  Author:     The Mister M.A.
//  Created at: 17.02.21

#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <chrono>
#include <random>

class Chip8
{
private:
    static const unsigned int BASE_REG_AMOUNT = 16;
    static const unsigned int RAM_SIZE_IN_BYTES = 4096;
    static const unsigned int STACK_LEVELS = 16;
    static const unsigned int VIDEO_WIDTH = 64;
    static const unsigned int VIDEO_HIGHT = 32;

    //  The starting address, which the `Program Counter` would be initialize to
    static const unsigned int START_ADDRESS = 0x200;

    //  Characters Font hight and the number of each character.
    static const unsigned int AMOUNT_OF_CHARS = 16;
    static const unsigned int FONT_HIGHT = 5;
    static const unsigned int FONTSET_START_ADDRESS = 0x50;

    const uint8_t fontset[AMOUNT_OF_CHARS * FONT_HIGHT] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F

    };

    uint8_t registers[BASE_REG_AMOUNT]{};
    uint8_t memory[RAM_SIZE_IN_BYTES]{};
    uint16_t index{};
    uint16_t pc{};
    uint16_t stack[STACK_LEVELS]{};
    uint8_t sp{};
    uint8_t delayTimer{};
    uint8_t soundTimer{};
    uint8_t keypad[AMOUNT_OF_CHARS]{};
    uint32_t video[VIDEO_WIDTH * VIDEO_HIGHT]{};
    uint16_t opcode;

    //  Randomness seeds
    std::default_random_engine randGen;
    std::uniform_int_distribution<uint8_t> randByte;
public:
    Chip8(const char *file_path);
    ~Chip8();

    //  Loads the binary values in the file to the Memory attribute.
    void LoadROM(const char *file_path);
};

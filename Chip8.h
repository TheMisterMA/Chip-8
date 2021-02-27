//  Author:     The Mister M.A.
//  Created at: 17.02.21

#include <cstdint>

class Chip8
{
private:
    static const unsigned int BASE_REG_AMOUNT   = 16;
    static const unsigned int RAM_SIZE_IN_BYTES = 4096;
    static const unsigned int STACK_LEVELS      = 16;
    static const unsigned int AMOUNT_OF_CHARS   = 16;
    static const unsigned int VIDEO_WIDTH       = 64;
    static const unsigned int VIDEO_HIGHT       = 32;

    //  The starting address, which the `Program Counter` would be initialize to
    static const unsigned int START_ADDRESS     = 0x200;

    uint8_t     registers[BASE_REG_AMOUNT]{};
	uint8_t     memory[RAM_SIZE_IN_BYTES]{};
	uint16_t    index{};
	uint16_t    pc{};
	uint16_t    stack[STACK_LEVELS]{};
	uint8_t     sp{};
	uint8_t     delayTimer{};
	uint8_t     soundTimer{};
	uint8_t     keypad[AMOUNT_OF_CHARS]{};
	uint32_t    video[VIDEO_WIDTH * VIDEO_HIGHT]{};
	uint16_t    opcode;

public:
    Chip8(char const* filePath);
    ~Chip8();
};
